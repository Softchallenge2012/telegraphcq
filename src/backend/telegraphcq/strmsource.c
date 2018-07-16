/*
 * sockstate.c
 *
 * Routines for reading CSV tuples directly off a socket in the backend.
 *
 * Author: Fred Reiss <phred@cs.berkeley.edu>
 */


#include "postgres.h"
#include "nodes/execnodes.h"

#include "telegraphcq/strmsource.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>


/*
 * Read the next batch of tuples from the socket, if possible.
 *
 * Returns:
 *      0 on success, 
 *      1 on no data, 
 *      -1 on socket closed.
 */
int fillReadBuf(SockState *ss) {
    struct pollfd pfd;
    int ret = -1;
    size_t space_rem;
    int nread;


    Assert(ss->fd != -1);

    space_rem = SCANSOCK_READ_BUFSZ - ss->readBufUsed;

    /* SPECIAL CASE: No space left */
    if (0 == space_rem) {
        return 0;
    }
    /* END SPECIAL CASE */

    /* Check to see if we can read on the socket. */  
    pfd.fd = ss->fd;
    pfd.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;
    pfd.revents = 0x0;

    ret = poll(&pfd, 1, 0);
    if (-1 == ret) {
        elog(ERROR, "poll() error: %s", strerror(errno));
    }

    if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
        goto SenderIsGone;
    } 

    if (!(pfd.revents & (POLLIN))) {
        /* Socket not readable */
        return 1;
    }

    /* Now we're ready to read data. */
    nread = recv(ss->fd, (ss->readBuf) + ss->readBufUsed, space_rem, 0x0);

    /*
    elog(LOG, "fillReadBuf(%d): nread is %d; errno is %d", 
            ss->fd, nread, errno);
            */

    if (nread <= 0) {
        goto SenderIsGone; 
    }

    /*elog(LOG, "Read %d bytes on socket %d", nread, ss->fd);*/
    ss->readBufUsed += nread;
    /*
    elog(LOG, "Buffer now goes from %d to %d",
            ss->readBufPos, ss->readBufUsed);
            */
    return 0;

/* Poor man's exception handler... */
SenderIsGone:
    elog(LOG, "Read socket %d closed.", ss->fd);
    close(ss->fd);
    ss->fd = -1;
    return -1;
}


/*
 * Pull the next line out the buffer, in place.  Returns NULL if we don't have
 * a line available.
 */
char * getNextLine(SockState *ss)
{
    int i;
    
    /* SPECIAL CASE: No data.  Reset the buffer. */
    if (ss->readBufPos >= ss->readBufUsed) {
        ss->readBufPos = 0;
        ss->readBufUsed = 0;
        return NULL;
    }
    /* END SPECIAL CASE */

    /* Look for the first newline. */
    for (i = ss->readBufPos; i < ss->readBufUsed; i++) {
        if ('\n' == ss->readBuf[i]) {
            char * ret;

            ss->readBuf[i] = '\0';

            /* Check for \r\n */
            if (i > 0 && '\r' == ss->readBuf[i-1]) {
                ss->readBuf[i-1] = '\0'; 
            }
            
            ret = (ss->readBuf) + ss->readBufPos;
            ss->readBufPos = i + 1; 

            /*
            elog(LOG, "Moved read position to %d of %d", 
                    ss->readBufPos, ss->readBufUsed);
                    */

            /*elog(LOG, "getNextLine(%d): Got line '%s'", ss->fd, ret);*/

            return ret;
        }
    }

    /* If we get here, we didn't find a newline.  Move our partial line to the
     * beginning of the buffer. */

    if (0 == ss->readBufPos && SCANSOCK_READ_BUFSZ == ss->readBufUsed) {
        elog(ERROR, "Got a line longer than buffer size (%d bytes)",
                SCANSOCK_READ_BUFSZ);
    }

    memmove(ss->readBuf, (ss->readBuf) + ss->readBufPos,
            (size_t)(ss->readBufUsed - ss->readBufPos));
    
    /* Now we need to correct our offsets into the buffer. */
    ss->readBufUsed -= ss->readBufPos;
    ss->readBufPos = 0;

    return NULL;
}

/* 
 * Attempt a non-blocking write, if the socket is available.
 *
 * Returns false on success, true if the socket wasn't available.
 */
bool lossyWrite(SockState *ss, const char *buf, size_t buflen) 
{
    if (-1 == ss->fd) {
        return true;
    }

    /* Right now, we only write what can be sent in a single call */
    Assert(buflen < 4096);

    /* TODO: Handle partial writes! */

    int ret = send(ss->fd, buf, buflen, MSG_DONTWAIT);

    if (ret == buflen) {
        return false;
    } else {
        return true;
    }

}


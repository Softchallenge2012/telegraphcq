// HttpBasicConnection.java
// $Id: HttpBasicConnection.java,v 1.2 2003/07/17 05:19:13 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1996.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.protocol.http;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;

import java.net.InetAddress;
import java.net.Socket;

import org.w3c.www.mime.MimeParser;
import org.w3c.www.mime.MimeParserFactory;

import org.w3c.www.http.HttpStreamObserver;

class HttpBasicConnection extends HttpConnection implements HttpStreamObserver
{
    private static final boolean debug = false;

    /**
     * The physical socket underlying the connection.
     */
    private Socket       socket = null;
    /**
     * The MIME parser to read input from the connection.
     */
    MimeParser   parser = null;
    /**
     * THe socket output stream, when available.
     */
    OutputStream output = null;
    /**
     * The socket input stream when available.
     */
    InputStream  input  = null;
    /**
     * The MimeParser factory to use to create Reply instances.
     */
    MimeParserFactory reply_factory = null;
    /**
     * The thread that owns the connection, for checking assertions.
     */
    Thread th = null;
    /**
     * The target INET address of this connection.
     */
    InetAddress inetaddr = null;
    /**
     * The target port number for this connection.
     */
    int         port     = -1;
    /**
     * The Timout on the underlying socket
     */
    int        timeout   = 300000;
    /**
     * All connections are associated with a uniq identifier, for debugging.
     */
    protected int id = -1;

    /**
     * Print this connection into a String.
     * @return A String containing the external representation for the 
     * connection.
     */

    public String toString() {
	return inetaddr + ":" + port +"["+id+"]";
    }

    /**
     * The entity stream we observe has reached its end.
     * Notify the server that it can now reuse the connection safely for some
     * other pending requests.
     * @param in The stream that has reached its end of file.
     */

    public synchronized void notifyEOF(InputStream in) {
	markIdle(false);
    }

    /**
     * The entity stream we were to observe refuse to be observed.
     * The connection will not be reusable, so we should detach it from the 
     * managing server, without closing it, since the entity reader will
     * close it itself.
     * @param in The stream that has been closed.
     */

    public synchronized void notifyFailure(InputStream in) {
	markIdle(true);
    }

    /**
     * The entity stream we observe has been closed.
     * After making sure the entire entity has been read, we can safely hand
     * out the connection to the server, for later reuse.
     * @param in The stream that has been closed.
     */

    public synchronized void notifyClose(InputStream in) {
	try {
	    byte buffer[] = new byte[1024];
	    while (in.read(buffer) > 0)
		;
	    markIdle(false);
	} catch (IOException ex) {
	    markIdle(true);
	}
    }

    /**
     * Close this connection to terminate it.
     * This method will only close the streams, and free all the data
     * structures that it keeps.
     */

    public synchronized void close() {
	if ( socket != null ) {
	    // Close the socket:
	    try {
		socket.close();
	    } catch (IOException ex) {
	    }
	    socket = null;
	    // Mark all data as invalid:
	    output = null;
	    input  = null;
	    parser = null;
	    cached = false;
	}
	return;
    }

    /**
     * Mark this connection as being used.
     * The server, which keeps track of idle connections, has decided to use
     * this connection to run some request. Mark this connection as used
     * and unregister it from the server's list of idle connections.
     * <p>Some assumptions are checked before handing out the connection
     * for use, which can throw an RuntimeException.
     * @return A boolean, <strong>true</strong> if the connection can be used
     * or reused, <strong>false</strong> otherwise (the connection was detected
     * idle, and destroy itself).
     * @exception RuntimeException If the connection is in an invalid state.
     */

    public synchronized boolean markUsed() {
	cached = false;
	if ( debug )
	    System.out.println(this+ " used !");
	if ( th != null )
	    throw new RuntimeException(this+" already used by "+th);
	th = Thread.currentThread();
	// FIXME check if socket is alive, set to null if not
	// if ( ! socket.isAlive()) 
	//    socket = null;
	if ( socket != null ) {
	    cached = true;
	}
	if ( socket == null ) {
	    try {
		socket = new Socket(inetaddr, port);
		socket.setSoTimeout(timeout);
		output = new BufferedOutputStream(socket.getOutputStream());
		input  = new BufferedInputStream(socket.getInputStream());
		parser = new MimeParser(input, reply_factory);
	    } catch (Throwable ex) {
		// Close that connection (cleanup):
		close(); 
		// Mark that connection as dead:
		((HttpBasicServer) server).unregisterConnection(this);
		((HttpBasicServer) server).deleteConnection(this);
		return false;
	    }
	} 
	((HttpBasicServer) server).unregisterConnection(this);
	return true;
    }

    /**
     * The connection is now idle again.
     * Mark the connection as idle, and register it to the server's list of 
     * idle connection (if this connection can be reused). If the connection
     * cannot be reused, detach it from the server and forget about it (the
     * caller will close it by closing the entity stream).
     * @param close Should this connection be physically closed (it is not
     * reusable), or should we try to keep track of it for later reuse.
     * @exception RuntimeException If the connection is in an invalid state.
     */

    public synchronized void markIdle(boolean close) {
	// Has this connection already been marked idle ?
	if ( th == null ) 
	    return;
	if ( debug )
	    System.out.println(this+" idle !"+close);
	// Check consistency:
	// if ( Thread.currentThread() != th )
	//    throw new RuntimeException(this +
	//	 		         " th mismatch " +
	//		 	         th + 
	//			         "/" +
	//			         Thread.currentThread());
	// Ok, mark idle for good:
	th = null;
	if ( close ) {
	    if ( debug ) 
		System.out.println(this+" closing !");
	    close();
	    ((HttpBasicServer) server).deleteConnection(this);
	} else {
	    // Notify the server that a new connection is available:
	    ((HttpBasicServer) server).registerConnection(this);
	}
    }

    /**
     * Some data available on input, while writing to the server.
     * This callback gets called when the client is emitting data to the
     * server and the server has sent us something before we actually sent 
     * all our bytes.
     * <p>Take any appropriate action.
     */

    public void notifyInputAvailable(InputStream in) {
	return;
    }

    /**
     * Get the MIME parser to read from this connection.
     * All access to the connection's input stream should go through the MIME
     * parser to ensure buffering coherency.
     * @return A MimeParser instance suitable to parse the reply input stream.
     * @exception RuntimeException If the connection was not connected.
     */

    public MimeParser getParser() {
	if ( parser == null )
	    throw new RuntimeException("getParser while disconnected.");
	return parser;
    }

    /**
     * Get the connection output stream.
     * @return The output stream to send data on this connection.
     * @exception RuntimeException If the connection was not previously opened.
     */

    public OutputStream getOutputStream() {
	if ( output == null )
	    throw new RuntimeException("getOutputStream while disconnected.");
	return output;
    }

    /**
     * Create a new connection.
     * To be used only by HttpServer instances.
     */

    HttpBasicConnection(HttpServer server
			, int id
			, InetAddress addr
			, int port
			, int timeout
			, MimeParserFactory reply_factory)
	throws IOException
    {
	this.server   = server;
	this.inetaddr = addr;
	this.port     = port;
	this.id       = id;
	this.timeout  = timeout;
	this.reply_factory = reply_factory;
    }

}



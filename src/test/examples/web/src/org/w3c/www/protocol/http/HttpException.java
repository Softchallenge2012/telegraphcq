// HttpException.java
// $Id: HttpException.java,v 1.2 2003/07/17 05:19:13 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1996.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.protocol.http;

/**
 * Exception thrown when processing a request failed.
 */

public class HttpException extends Exception {
    Request   request = null;
    Exception exception = null;

    /**
     * Get the original cause for this exception.
     * HttpException can be used to wrap up transport layer problems (such
     * as IOException or other SocketException, etc). In that case, this method
     * will return the original exception that occured.
     * @return An Exception instance, or <strong>null</strong>.
     */

    public final Exception getException() {
	return exception;
    }

    /**
     * Get the request that triggered this exception.
     * @return A Request instance.
     */

    public final Request getRequest() {
	return request;
    }

    public HttpException(Request request, String msg) {
	super(msg);
	this.request = request;
    }

    public HttpException(Request request, Exception ex) {
	super(ex.getMessage());
	this.request   = request;
	this.exception = ex;
    }

    public HttpException(Exception ex, String msg) {
	super(msg);
	this.exception = ex;
    }
}

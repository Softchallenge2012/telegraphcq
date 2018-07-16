// HttpExtException.java
// $Id: HttpExtException.java,v 1.2 2003/07/17 05:19:11 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1998.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.http;

/**
 * @version $Revision: 1.2 $
 * @author  Benoît Mahé (bmahe@w3.org)
 */
public class HttpExtException extends RuntimeException {

    protected HttpExtException(String msg) {
	super(msg);
    }

    protected HttpExtException() {
	super();
    }
}

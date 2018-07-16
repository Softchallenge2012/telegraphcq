// HttpServerState.java
// $Id: HttpServerState.java,v 1.2 2003/07/17 05:19:13 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1996.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.protocol.http ;

import java.util.Vector;

class HttpServerState {
    HttpServer server = null;
    Vector     conns  = null;

    private static final boolean debug = true;

    final HttpServer getServer() {
	return server;
    }

    synchronized boolean notEnoughConnections() {
	return (conns == null) || (conns.size() == 1);
    }

    void registerConnection(HttpConnection conn) {
	if ( conns == null )
	    conns = new Vector(4);
	conns.addElement(conn);
    }

    void unregisterConnection(HttpConnection conn) {
	if ( conns != null )
	    conns.removeElement(conn);
    }

    void deleteConnection(HttpConnection conn) {
	if ( conns != null ) 
	    conns.removeElement(conn);
    }

    synchronized boolean hasConnection() {
	return (conns != null) && (conns.size() > 0);
    }

    synchronized HttpConnection getConnection() {
	if ((conns != null) && (conns.size() > 0)) {
	    HttpConnection conn = (HttpConnection) conns.elementAt(0);
	    conns.removeElementAt(0);
	    conn.cached = true;
	    return conn;
	}
	return null;
    }

    public String toString() {
	String tostring = "";
	if (conns == null)
	    tostring = "null";
	else if (conns.size() == 0)
	    tostring = "empty";
	else {
	    for (int i = 0 ; i < conns.size() ; i++) {
		tostring += "["+
		    ((HttpConnection) conns.elementAt(i)).toString()+
		    "]";
	    }
	}
	return tostring;
    }

    HttpServerState(HttpServer server) {
	this.server = server;
    }
}



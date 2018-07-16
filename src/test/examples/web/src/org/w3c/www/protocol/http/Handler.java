// Handler.java
// $Id: Handler.java,v 1.2 2003/07/17 05:19:13 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1996.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.protocol.http ;

import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;

import java.io.IOException;

public class Handler extends URLStreamHandler {

    protected URLConnection openConnection (URL u)
	throws IOException 
    {
	return new HttpURLConnection(u);
    }

}

// CookieFilter.java
// $Id: CookieFilter.java,v 1.2 2003/07/17 05:19:14 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1996.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.protocol.http.cookies;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.io.Writer;

import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import java.net.URL;

import org.w3c.util.ObservableProperties;

import org.w3c.www.protocol.http.HttpException;
import org.w3c.www.protocol.http.HttpManager;
import org.w3c.www.protocol.http.PropRequestFilter;
import org.w3c.www.protocol.http.PropRequestFilterException;
import org.w3c.www.protocol.http.Reply;
import org.w3c.www.protocol.http.Request;

import org.w3c.www.http.HttpCookie;
import org.w3c.www.http.HttpCookieList;
import org.w3c.www.http.HttpFactory;
import org.w3c.www.http.HttpMessage;
import org.w3c.www.http.HttpRequestMessage;
import org.w3c.www.http.HttpSetCookie;
import org.w3c.www.http.HttpSetCookieList;

class DomainNode {

    Hashtable nodes       = null; // < String , DomainNode >
    HttpSetCookie cookies [] = null;
    int nbcookies         = 0;

    protected boolean sameCookies(HttpSetCookie c1, HttpSetCookie c2) {
	if (c2.getPath() == null && c1.getPath() == null)
	    return (c1.getName().equals(c2.getName()));
	else  if (c2.getPath() == null || c1.getPath() == null)
	    return false;
	else return ((c1.getName().equals(c2.getName())) &&
		     (c1.getPath().equals(c2.getPath())));
    }

    protected void addCookie(HttpSetCookie cookie) {
	int i = 0;
			long now = (new Date().getTime())/1000;
			long maxage = cookie.getMaxAge();
			//			System.out.println("now=" + now + ", maxage=" + cookie.getMaxAge());
	while (i < nbcookies) {
		if(cookies[i]==null) break;
		//		System.out.println("checking to see if " + DomainTree.cookie2String(cookie) + "and " + DomainTree.cookie2String(cookies[i]) + "  answer is: " + sameCookies(cookie, cookies[i]));
	    if (sameCookies(cookie,cookies[i])) {
			// need to check for expiration here
			// and remove if required

			if((maxage > 0) && ( maxage < now))
				{
					System.out.println("expiring a cookie: " + cookies[i].getName() + "=" + cookies[i].getValue());
					cookies[i]=null;
					for(int j=i+1; j < nbcookies; j++)
						{
							cookies[j-1] = cookies[j];
							cookies[j]=null;
						}
					nbcookies--;
				}
			else
				{
					//					System.out.println("found a cookie that already exists");
					cookies[i] = cookie;
				}
			return;
	    }
	    i++;
	}
	if((maxage > 0) && (maxage < now))
		{
			System.out.println("expiring first cookie: " + cookie.getName() + "=" + cookie.getValue());
			return;
		}
			
	if (nbcookies < cookies.length) {
	    cookies[nbcookies++] = cookie;
	} else {
	    HttpSetCookie ncookies [] = new HttpSetCookie[cookies.length +1];
	    System.arraycopy(cookies,0,ncookies,0,cookies.length);
	    ncookies[nbcookies++] = cookie;
	    cookies = ncookies;
	}
    }

    protected void sync(FileWriter writer) 
	throws IOException
    {
	if (nbcookies > 0) {
	    for (int i = 0; i < nbcookies; i++) {
		if (cookies[i].getMaxAge() > 0)
		    writer.write(DomainTree.cookie2String(cookies[i]));
	    }
	}
	Enumeration e = nodes.elements();
	DomainNode node = null;
	while (e.hasMoreElements()) {
	    node = (DomainNode)e.nextElement();
	    node.sync(writer);
	}
    }

    DomainNode() {
	nodes     = new Hashtable(2);
	cookies   = new HttpSetCookie[1];
	nbcookies = 0;
    }

}

class DomainTree {

    Hashtable nodes        = null; // < String , DomainNode >

    protected static String cookie2String(HttpSetCookie cookie) {
	Date date = new Date();
	return (cookie.getDomain()+ //0
		"\t"+
		String.valueOf(cookie.getSecurity()).toUpperCase()+ //1
		"\t"+
		cookie.getPath()+ //2
		"\t"+
		(cookie.getMaxAge()+(date.getTime()/1000))+ //3
		"\t"+
		cookie.getVersion()+ //4
		"\t"+
		cookie.getName()+ //5
		"\t"+
		cookie.getValue()+ //6
		"\n");
    }

    protected HttpSetCookie string2Cookie(String cookie[]) {
	HttpSetCookie cook = new HttpSetCookie();
	cook.setDomain(cookie[0]);
	cook.setSecurity(Boolean.getBoolean(cookie[1].toLowerCase()));
	cook.setPath(cookie[2]);
	long expire = Long.parseLong(cookie[3]);
	long now = (new Date()).getTime();
	cook.setMaxAge((int)(expire - (now/1000)));
	//	System.out.println("sting2Cookie maxage is " + cook.getMaxAge());
	cook.setVersion(Integer.parseInt(cookie[4]));
	cook.setName(cookie[5]);
	cook.setValue(cookie[6]);
	return cook;
    }

    protected synchronized void loadCookies(File file) 
	throws FileNotFoundException
    {
	try {
	    FileReader reader = new FileReader(file);
	    String cookie[] = new String[8];
	    int i = 0;
	    int ch;
	    StringBuffer buffer = new StringBuffer(30);
	    while ((ch = reader.read()) != -1) {
		switch (ch) {
		case '#':
		    while ((ch = reader.read()) != '\n')
			if (ch == -1) return;
		    break;
		case '\n':
		    if (i > 0) {
			cookie[i++] = buffer.toString();
			HttpSetCookie setcookie = string2Cookie(cookie);
			if (setcookie.getMaxAge() > 0)
				{
					//					System.out.println("loadcookie: " + cookie2String(setcookie) + " insert called");
					insertCookie(setcookie);
				}
			else
				{
					//	System.out.println("loadcookie: " + cookie2String(setcookie) + " rejected");
				}
			

			buffer = new StringBuffer(30);
			cookie = new String[8];
			i = 0;
		    }
		    break;
		case '\t':
		case ' ':
		    cookie[i++] = buffer.toString();
		    buffer = new StringBuffer(30);
		    break;
		default:
		    buffer.append((char) ch);
		}
	    }
	} catch (IOException ex) {
	    System.out.println(ex.getMessage());
	    ex.printStackTrace();
	}
    }

    protected synchronized void sync(File file) {
	Enumeration e     = nodes.elements();
	DomainNode node   = null;
	FileWriter writer = null;
	try {
	    writer = new FileWriter(file);
	    writer.write("# Jigsaw client HTTP Cookie File\n");
	    writer.write("# This is a generated file!  Do not edit.\n\n");
	    while (e.hasMoreElements()) {
		node = (DomainNode)e.nextElement();
		node.sync(writer);
	    } 
	    writer.close();
	} catch (IOException ex) {
	    System.out.println(ex.getMessage());
	    ex.printStackTrace();
	} finally {
	    try {if (writer != null) writer.close();} catch (Exception ex2) {}
	}
    }

    protected boolean isIp(String domain) {
	int last = domain.length()-1;
	return ((domain.charAt(last)>='0') && (domain.charAt(last)<='9'));
    }

    protected String[] domainParts(String dom) {
	if (dom == null)
	    return null;
	String domain = new String(dom);
	Vector V = new Vector(5);
	int i = 0; int j = 0; int max = domain.length(); 
	// fix the . symbol bug
	while (i < max) {
	    j = domain.indexOf('.', i);
		if(j==0)
			{
				// .monster.com cookies should go into the monster node
				i++;
				continue;
			}
	    if (j == -1) 
		j = max;
	    V.addElement(domain.substring(i, j));
	    i = j + 1;
	};
	// end of fix
	if (V.size() == 0)
	    return null;
	String parts [] = new String[V.size()];
	V.copyInto(parts);
	return parts;
    }

    protected HttpCookie setCookie2Cookie(HttpSetCookie setcookie) {
	HttpCookie cookie = new HttpCookie();
	if (setcookie != null) {
	    cookie.setName(setcookie.getName());
	    cookie.setValue(setcookie.getValue());
	    cookie.setDomain(setcookie.getDomain());
	    cookie.setPath(setcookie.getPath());
	    cookie.setVersion(setcookie.getVersion());
	    return cookie;
	}
	return null;
    }

    protected void addMatchingPathCookiesInVector(HttpSetCookie cookieArray[],
						  String path,
						  Vector V) {

	int i = 0;
	while ( i < cookieArray.length ) {
		if(cookieArray[i] == null) break;
	    if (path.equals("/")) 
		V.addElement(setCookie2Cookie(cookieArray[i]));
	    else  if (cookieArray[i].getPath() != null) {
		if (path.startsWith(cookieArray[i].getPath())) {
		    // transform SetCookie in Cookie
		    V.addElement(setCookie2Cookie(cookieArray[i]));
		}
	    }
	    i++;
	}
    }

    public HttpCookieList getCookies(URL url) {
	String domain   = url.getHost();
	String path     = url.getFile();
	String parts [] = domainParts(domain);
	
	if (parts == null) //FIXME Exception
	    return null;
	Vector V = new Vector(5);
	int i = 0;
	DomainNode node = null;
	Hashtable childs = nodes;

	if (isIp(domain)) {
	    node = (DomainNode) childs.get(parts[i]);
	    while ( i < parts.length ) {
		if (node == null)
		    return null;
		if (node.nbcookies > 0) {
		    addMatchingPathCookiesInVector(node.cookies, path, V);
		}
		if ((i + 1) < parts.length)
			{
				node = (DomainNode)childs.get(parts[++i]);
			}
		else
		    node = null;

		if (node == null)
		    break;
		childs = node.nodes;
	    }
	} else {
	    i = parts.length - 1;      
	    node = (DomainNode) childs.get(parts[i]);

	    while ( i >= 0 ) {
		if (node == null)
		    return null;
		if (node.nbcookies > 0) {
		    addMatchingPathCookiesInVector(node.cookies, path, V);
		}
		if (i > 0)
			{
				node = (DomainNode)childs.get(parts[--i]);

			}
		else 
		    node = null;
		if (node == null)
		    break;
		childs = node.nodes;
	    }
	}
	if (V.size() == 0)
	    return  null;
	HttpCookie cookieArray[] = new HttpCookie[V.size()];
	V.copyInto(cookieArray);
	return HttpFactory.makeCookieList(cookieArray);
    }

    public void insertCookie(HttpSetCookie cookie) {
		//		System.out.println("adding cookie to tree: " + cookie.toString());
	String domain  = cookie.getDomain();
	String parts[] = domainParts(domain);


	if (parts == null) //FIXME Exception
	    return;
	int i = 0;
	DomainNode node = null;
	Hashtable childs = nodes;
	if (isIp(domain)) {
	    node = (DomainNode) childs.get(parts[i]);
	    while ( true ) {
		if (node == null) {
		    node = new DomainNode();
		    childs.put(parts[i], node);
		}
		if (i == parts.length - 1) {
		    node.addCookie(cookie);
		    return;
		}
		node   = (DomainNode)childs.get(parts[++i]);
		if (node == null) {
		    node = new DomainNode();
		    childs.put(parts[i], node);
		}
		childs = node.nodes;
	    }
	} else {
	    i = parts.length - 1;      
	    node = (DomainNode) childs.get(parts[i]);
	    while ( true ) {
		if (node == null) {
		    node = new DomainNode();
		    childs.put(parts[i], node);
		}
		if (i == 0) {
		    node.addCookie(cookie);
		    return;
		}
		node   = (DomainNode)childs.get(parts[--i]);
		if (node == null) {
		    node = new DomainNode();
		    childs.put(parts[i], node);
		}
		childs = node.nodes;
	    }
	}
    }

    DomainTree() {
	this.nodes = new Hashtable(10);
    }
}

/**
 * Client side CookieFilter :
 * @author Benoit Mahe <bmahe@sophia.inria.fr>
 */

public class CookieFilter implements PropRequestFilter {

    /**
     * The absolute Path of the file use to store cookies.
     */
    public static final 
    String COOKIES_FILE_P = "org.w3c.www.protocol.http.cookie.file";

    private static final String defaultFileName = "cookie";

    private static DomainTree root = null;

    static {
	root = new DomainTree();
    }

    protected HttpManager manager       = null;

    private static File cookiefile    = null;

    /**
     * The request pre-processing hook.
     * Before each request is launched, all filters will be called back 
     * through
     * this method. They will generally set up additional request header
     * fields to enhance the request.
     * @param request The request that is about to be launched.
     * @return An instance of Reply if the filter could handle the request,
     * or <strong>null</strong> if processing should continue normally.
     * @exception HttpException If the filter is supposed to fulfill the
     * request, but some error happened during that processing.
     */
    public Reply ingoingFilter(Request request) 
	throws HttpException
    {
	HttpCookieList cookielist = root.getCookies(request.getURL());

	if (cookielist != null)
	    request.setCookie(cookielist);
	    else
	    {
		System.out.println("cookies not found for: " + request.getURL());
	    }
	return null;
    }

	public void LookupURLAndPrintCookies(URL u)
	{
		HttpCookieList cl = root.getCookies(u);
		if(cl == null) 
			{
				System.out.println("no cookies for url");
				return;
			}

		HttpCookie cookies[] = cl.getCookies();
		System.out.println("cookies for url " + u);
		for(int i=0; i < cookies.length; i++)
			System.out.println(cookies[i].toString());
	}

    /**
     * The request post-processing hook.
     * After each request has been replied to by the target server (be it a 
     * proxy or the actual origin server), each filter's outgoingFilter
     * method is called.
     * <p>It gets the original request, and the actual reply as a parameter,
     * and should return whatever reply it wants the caller to get.
     * @param request The original (handled) request.
     * @param reply The reply, as emited by the target server, or constructed
     * by some other filter.
     * @exception HttpException If the reply emitted by the server is not
     * a valid HTTP reply.
     */
    public Reply outgoingFilter(Request request, Reply reply) 
	throws HttpException
    {
	HttpSetCookieList list = reply.getSetCookie();
	if (list != null) {
	    HttpSetCookie [] cooks = list.getSetCookies();
	    if (cooks != null) {
		int i = 0;
		while (i < cooks.length) {
		    if (cooks[i].getDomain() == null)
			cooks[i].setDomain(request.getURL().getHost());
		    root.insertCookie(cooks[i++]);
		}
	    }
	}
	return reply;
    }

    /**
     * An exception occured while talking to target server.
     * This method is triggered by the HttpManager, when the target server
     * (which can be a proxy for that request) was not reachable, or some
     * network error occured while emitting the request or reading the reply
     * headers.
     * @param request The request whose processing triggered the exception.
     * @param ex The exception that was triggered.
     * @return A boolean, <strong>true</strong> if that filter did influence
     * the target server used to fulfill the request, and it has fixed the 
     * problem in such a way that the request should be retried.
     */
    public boolean exceptionFilter(Request request, HttpException ex) {
	return false;
    }

    /** 
     * Synchronized any pending state into stable storage.
     * If the filter maintains some in-memory cached state, this method
     * should ensure that cached data are saved to stable storage.
     */
    public void sync() {
	root.sync(cookiefile);
    }

    /**
     * Initialize this filter, using the provided manager.
     * During initialization, it is up to the filter to install itself
     * in the manager, by invoking the appropriate <code>setFilter</code>
     * method.
     * @param manager The HttpManager initializing the filter.
     * @exception PropRequestFilterException If the filter couldn't be 
     * initialized properly.
     */
    public void initialize(HttpManager manager) 
	throws PropRequestFilterException
    {
	this.manager = manager;
	ObservableProperties props = manager.getProperties();
	String filepath = (String)props.getString(COOKIES_FILE_P, null);
	System.out.println("got cookie file name: " + filepath);
	if (filepath == null) {
	    cookiefile = new File(defaultFileName);
	} else {
	    cookiefile = new File(filepath);
	}
	if (cookiefile.exists()) {
	    // load all the Cookie filter and register them in the manager
	    try {
		root.loadCookies(cookiefile);
		System.out.println("cookies loaded");
	    } catch (FileNotFoundException ex) {
		System.out.println(ex.getMessage());
		throw new PropRequestFilterException(ex.getMessage());
	    }
	}
	manager.setFilter(this);
    }
}

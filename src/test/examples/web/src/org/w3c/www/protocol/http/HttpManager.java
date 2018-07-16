// HttpManager.java
// $Id: HttpManager.java,v 1.2 2003/07/17 05:19:13 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1996.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.www.protocol.http ;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Properties;

import java.net.URL;

import java.io.InputStream;
import java.io.PrintStream;

import org.w3c.www.mime.MimeHeaderHolder;
import org.w3c.www.mime.MimeParser;
import org.w3c.www.mime.MimeParserFactory;

import org.w3c.util.LRUList;
import org.w3c.util.ObservableProperties;
import org.w3c.util.PropertyMonitoring;
import org.w3c.util.SyncLRUList;

class ManagerDescription {
    HttpManager manager = null;
    Properties  properties = null;

    final HttpManager getManager() {
	return manager;
    }

    final boolean sameProperties(Properties props) {
	if (props.size() != properties.size())
	    return false;
	Enumeration enum = props.propertyNames();
	while (enum.hasMoreElements()) {
	    String name = (String) enum.nextElement();
	    String prop = properties.getProperty(name);
	    if ((prop == null) || (! prop.equals(props.getProperty(name))))
		return false;
	}
	return true;
    }

    ManagerDescription(HttpManager manager, Properties props) {
	this.manager    = manager;
	this.properties = (Properties)props.clone();
    }
}

class ReplyFactory implements MimeParserFactory {

    public MimeHeaderHolder createHeaderHolder(MimeParser parser) {
	return new Reply(parser);
    }

}

/**
 * The client side HTTP request manager.
 * This class is the user interface (along with the other public classes of
 * this package) for the W3C client side library implementing HTTP. 
 * A typicall request is launched though the following sequence:
 * <pre>
 * HttpManager     manager = HttpManager.getManager() ;
 * Request request = manager.createRequest() ;
 * request.setMethod(HTTP.GET) ;
 * request.setURL(new URL("http://www.w3.org/pub/WWW/"));
 * Reply    reply = manager.runRequest(request) ;
 * // Get the reply input stream that contains the actual data:
 * InputStream in = reply.getInputStream() ;
 * ...
 * </pre>
 */

public class HttpManager implements PropertyMonitoring {

    private static final boolean debug = false;

    private static final 
    String DEFAULT_SERVER_CLASS = "org.w3c.www.protocol.http.HttpBasicServer";

    /**
     * The name of the property indicating the class of HttpServer to use.
     */
    public static final
    String SERVER_CLASS_P = "org.w3c.www.protocol.http.server";

    /**
     * The name of the property containing the ProprequestFilter to launch.
     */
    public static final 
    String FILTERS_PROP_P = "org.w3c.www.protocol.http.filters";
    /**
     * The maximum number of simultaneous connectionlrus.
     */
    public static final
    String CONN_MAX_P = "org.w3c.www.protocol.http.connections.max";
    /**
     * The SO_TIMEOUT of the client socket.
     */
    public static final
    String TIMEOUT_P = "org.w3c.www.protocol.http.connections.timeout";
    /**
     * Header properties - The allowed drift for getting cached resources.
     */
    public static final 
    String MAX_STALE_P = "org.w3c.www.protocol.http.cacheControl.maxStale";
    /**
     * Header properties - The minium freshness required on cached resources.
     */
    public static final
    String MIN_FRESH_P = "org.w3c.www.protocol.http.cacheControl.minFresh";
    /**
     * Header properties - Set the only if cached flag on requests.
     */
    public static final 
    String ONLY_IF_CACHED_P=
                         "org.w3c.www.protocol.http.cacheControl.onlyIfCached";
    /**
     * Header properties - Set the user agent.
     */
    public static final 
    String USER_AGENT_P = "org.w3c.www.protocol.http.userAgent";
    /**
     * Header properties - Set the accept header.
     */
    public static final 
    String ACCEPT_P = "org.w3c.www.protocol.http.accept";
    /**
     * Header properties - Set the accept language.
     */
    public static final 
    String ACCEPT_LANGUAGE_P = "org.w3c.www.protocol.http.acceptLanguage";
    /**
     * Header properties - Set the accept encodings.
     */
    public static final 
    String ACCEPT_ENCODING_P = "org.w3c.www.protocol.http.acceptEncoding";
    /**
     * Header properties - are we parsing answers in a lenient way?
     */
    public static final 
    String LENIENT_P = "org.w3c.www.protocol.http.lenient";
    /**
     * Header properties - Should we use a proxy ?
     */
    public static final
    String PROXY_SET_P = "proxySet";
    /**
     * Header properties - What is the proxy host name.
     */
    public static final
    String PROXY_HOST_P = "proxyHost";
    /**
     * Header properties - What is the proxy port number.
     */
    public static final
    String PROXY_PORT_P = "proxyPort";

    /**
     * The default value for the <code>Accept</code> header.
     */
    public static final
    String DEFAULT_ACCEPT = "*/*";
    /**
     * The default value for the <code>User-Agent</code> header.
     */
    public static final
    String DEFAULT_USER_AGENT = "Jigsaw/2.2.1";

    /**
     * This array keeps track of all the created managers.
     * A new manager (kind of HTTP client side context) is created for each
     * diffferent set of properties.
     */
    private static ManagerDescription managers[] = new ManagerDescription[4];

    /**
     * The class to instantiate to create new HttpServer instances.
     */
    protected Class serverclass = null;
    /**
     * The properties we initialized from.
     */
    ObservableProperties props = null;
    /**
     * The server this manager knows about, indexed by FQDN of target servers.
     */
    protected Hashtable servers = null;
    /**
     * The template request (the request we will clone to create new requests)
     */
    protected Request template = null ;
    /**
     * The LRU list of connections.
     */
    protected LRUList connectionsLru = null;
    /**
     * The filter engine attached to this manager.
     */
    FilterEngine filteng = null;

    protected int timeout = 300000;
    protected int conn_count = 0;
    protected int conn_max = 5;
    protected boolean lenient = true;

    /**
     * Update the proxy configuration to match current properties setting.
     * @return A boolean, <strong>true</strong> if change was done,
     * <strong>false</strong> otherwise.
     */

    protected boolean updateProxy() {
	boolean set = props.getBoolean(PROXY_SET_P, false);
	if ( set ) {
	    // Wow using a proxy now !
	    String host  = props.getString(PROXY_HOST_P, null);
	    int    port  = props.getInteger(PROXY_PORT_P, -1);
	    URL    proxy = null;
	    try {
		proxy   = new URL("http", host, port, "/");
	    } catch (Exception ex) {
		return false;
	    }
	    // Now if a proxy...
	    if (( proxy != null ) && (proxy.getHost() != null))
		template.setProxy(proxy);
	} else {
	    template.setProxy(null);
	}
	return true;
    }

    /**
     * Get this manager properties.
     * @return An ObservableProperties instance.
     */

    public final ObservableProperties getProperties() {
	return props;
    }

    /**
     * PropertyMonitoring implementation - Update properties on the fly !
     * @param name The name of the property that has changed.
     * @return A boolean, <strong>true</strong> if change is accepted,
     * <strong>false</strong> otherwise.
     */

    public boolean propertyChanged(String name) {
	Request tpl = template;
	if ( name.equals(FILTERS_PROP_P) ) {
	    // FIXME
	    return true;
	    // return false;
	} else if ( name.equals(TIMEOUT_P) ) {
	    setTimeout(props.getInteger(TIMEOUT_P, timeout));
	    return true;
	} else if ( name.equals(CONN_MAX_P) ) {
	    setMaxConnections(props.getInteger(CONN_MAX_P, conn_max));
	    return true;
	} else if ( name.equals(MAX_STALE_P) ) {
	    int ival = props.getInteger(MAX_STALE_P, -1);
	    if ( ival >= 0 )
		tpl.setMaxStale(ival);
	    return true;
	} else if ( name.equals(MIN_FRESH_P) ) {
	    int ival = props.getInteger(MIN_FRESH_P, -1);
	    if ( ival >= 0 )
		tpl.setMinFresh(ival);
	    return true;
	} else if ( name.equals(LENIENT_P) ) {
	    lenient = props.getBoolean(ONLY_IF_CACHED_P, lenient);
	    return true;
	} if ( name.equals(ONLY_IF_CACHED_P) ) {
	    tpl.setOnlyIfCached(props.getBoolean(ONLY_IF_CACHED_P, false));
	    return true;
	} else if ( name.equals(USER_AGENT_P) ) {
	    tpl.setValue("user-agent"
			 , props.getString(USER_AGENT_P
					   , DEFAULT_USER_AGENT));
	    return true;
	} else if ( name.equals(ACCEPT_P) ) {
	    tpl.setValue("accept" 
			 , props.getString(ACCEPT_P, DEFAULT_ACCEPT));
	    return true;
	} else if ( name.equals(ACCEPT_LANGUAGE_P) ) {
	    String sval = props.getString(ACCEPT_LANGUAGE_P, null);
	    if ( sval != null )
		tpl.setValue("accept-language", sval);
	    return true;
	} else if ( name.equals(ACCEPT_ENCODING_P) ) {
	    String sval = props.getString(ACCEPT_ENCODING_P, null);
	    if ( sval != null )
		tpl.setValue("accept-encoding", sval);
	    return true;
	} else if ( name.equals(PROXY_SET_P)
		    || name.equals(PROXY_HOST_P)
		    || name.equals(PROXY_PORT_P) ) {
	    return updateProxy();
	} else {
	    return true;
	} 
    }

    /**
     * Allow the manager to interact with the user if needed.
     * This will, for example, allow prompting for paswords, etc.
     * @param onoff Turn interaction on or off.
     */

    public void setAllowUserInteraction(boolean onoff) {
	template.setAllowUserInteraction(onoff);
    }

    protected static synchronized 
	HttpManager getManager(Class managerclass, Properties p) 
    {
	// Does such a manager exists already ?
	for (int i = 0 ; i < managers.length ; i++) {
	    if ( managers[i] == null )
		continue;
	    if ( managers[i].sameProperties(p) )
		return managers[i].getManager();
	}
	// Get the props we will initialize from:
	ObservableProperties props = null;
	if ( p instanceof ObservableProperties )
	    props = (ObservableProperties) p;
	else
	    props = new ObservableProperties(p);
	// Create a new manager for this set of properties:
	HttpManager manager = null;;
	try {
	    Object o = managerclass.newInstance();
	    if (o instanceof HttpManager) {
		manager = (HttpManager) o;
	    } else { // default value
		manager = new HttpManager();
	    }
	} catch (Exception ex) {
	    ex.printStackTrace();
	    manager = new HttpManager();
	}
	manager.props = props;
	// Initialize this new manager filters:
	String filters[] = props.getStringArray(FILTERS_PROP_P, null);
	if ( filters != null ) {
	    for (int i = 0 ; i < filters.length ; i++) {
		try {
		    Class c = Class.forName(filters[i]);
		    PropRequestFilter f = null;
		    f = (PropRequestFilter) c.newInstance();
		    f.initialize(manager);
		} catch (PropRequestFilterException ex) {
		    System.out.println("Couldn't initialize filter \""
				       + filters[i]
				       + "\" init failed: "
                                       + ex.getMessage());
		} catch (Exception ex) {
		    System.err.println("Error initializing prop filters:");
		    System.err.println("Coulnd't initialize ["
                                       + filters[i]
				       + "]: " + ex.getMessage());
		    ex.printStackTrace();
		    System.exit(1);
		}
	    }
	}
	// The factory to create MIME reply holders:
	manager.factory = manager.getReplyFactory();
	// The class to create HttpServer instances from
	String c = props.getString(SERVER_CLASS_P, DEFAULT_SERVER_CLASS);
	try {
	  manager.serverclass = Class.forName(c);
	} catch (Exception ex) {
	    System.err.println("Unable to initialize HttpManager: ");
	    System.err.println("Class \""+c+"\" not found, from property "
                               + SERVER_CLASS_P);
	    ex.printStackTrace();
	    System.exit(1);
	}
	// Setup the template request:
	Request tpl = manager.template;
	// Set some default headers value (from props)
	// Check for a proxy ?
	manager.updateProxy();
	// CacheControl, only-if-cached
	tpl.setOnlyIfCached(props.getBoolean(ONLY_IF_CACHED_P, false));
	// CacheControl, maxstale
	int ival = props.getInteger(MAX_STALE_P, -1);
	if ( ival >= 0 )
	    tpl.setMaxStale(ival);
	// CacheControl, minfresh:
	ival = props.getInteger(MIN_FRESH_P, -1);
	if ( ival >= 0 )
	    tpl.setMinFresh(ival);
	// general, lenient
	manager.lenient = props.getBoolean(LENIENT_P, true);
	// General, User agent
	String sval;
	tpl.setValue("user-agent"
		     , props.getString(USER_AGENT_P
				       , DEFAULT_USER_AGENT));
	// General, Accept
	tpl.setValue("accept" 
		     , props.getString(ACCEPT_P, DEFAULT_ACCEPT));
	// General, Accept-Language
	sval = props.getString(ACCEPT_LANGUAGE_P, null);
	if ( sval != null ) {
	    if (sval.trim().length() > 0) {
		tpl.setValue("accept-language", sval);
	    }
	}
	// General, Accept-Encoding
	sval = props.getString(ACCEPT_ENCODING_P, null);
	if ( sval != null ) {
	    if (sval.trim().length() > 0) {
		tpl.setValue("accept-encoding", sval);
	    }
	}
	// Maximum number of allowed connections:
	manager.conn_max = props.getInteger(CONN_MAX_P, 5);
	// timeout value
	manager.timeout = props.getInteger(TIMEOUT_P, manager.timeout);
	// Register ourself as a property observer:
	props.registerObserver(manager);
	// Register that manager in our knwon managers:
	for (int i = 0 ; i < managers.length ; i++) {
	    if ( managers[i] == null ) {
		managers[i] = new ManagerDescription(manager, p);
		return manager;
	    }
	}
	ManagerDescription nm[] = new ManagerDescription[managers.length << 1];
	System.arraycopy(managers, 0, nm, 0, managers.length);
	nm[managers.length] = new ManagerDescription(manager, p);
	managers = nm;
	return manager;
    }
						       

    /**
     * Get an instance of the HTTP manager.
     * This method returns an actual instance of the HTTP manager. It may
     * return different managers, if it decides to distribute the load on
     * different managers (avoid the HttpManager being a bottleneck).
     * @return An application wide instance of the HTTP manager.
     */

    public static synchronized HttpManager getManager(Properties p) {
	return getManager(HttpManager.class, p);
    }

    public static HttpManager getManager() {
	return getManager(System.getProperties());
    }

    /**
     * Get the String key for the server instance handling that request.
     * This method takes care of any proxy setting (it will return the key
     * to the proxy when required.)
     * @return A uniq identifier for the handling server, as a String.
     */

    public final String getServerKey(Request request) {
	URL    proxy  = request.getProxy();
	URL    target = request.getURL();
	String key   = null;
	if ( proxy != null ) {
	    return ((proxy.getPort() == 80)
		    ? proxy.getHost().toLowerCase()
		    : (proxy.getHost().toLowerCase()+":"+proxy.getPort()));
	} else {
	    return ((target.getPort() == 80)
		    ? target.getHost().toLowerCase()
		    : (target.getHost().toLowerCase()+":"+target.getPort()));
	}
    }

    /**
     * Get the appropriate server object for handling request to given target.
     * @param key The server's key, as returned by <code>getServerKey</code>.
     * @return An object complying to the HttpServer interface.
     * @exception HttpException If the given host name couldn't be resolved.
     */

    protected synchronized HttpServer lookupServer(String protocol, String host, int port)
	throws HttpException
    {
		boolean isHttps = protocol.equalsIgnoreCase("https");
	int    p  = -1;
	if(isHttps == false)
		{
			p = (port == -1) ? 80 : port;
		}
	else
		{
			p = (port == -1) ? 443 : port;
		}
	String id = ((p == 80) 
		     ? host.toLowerCase() 
		     : (host.toLowerCase() +":"+p)) + isHttps;
	// Check for an existing server:
	HttpServer server = (HttpServer) servers.get(id);
	if ( server != null )
	    return server;
	// Create and register a new server:
	try {
	    server = (HttpServer) serverclass.newInstance();
	} catch (Exception ex) {
	    String msg = ("Unable to create an instance of \""
			  + serverclass.getName()
			  + "\", invalid config, check the "
			  + SERVER_CLASS_P + " property.");
	    throw new HttpException(ex, msg);
	}
	server.initialize(this, new HttpServerState(server), host, p, timeout, protocol.equalsIgnoreCase("https"));
	servers.put(id, server);
	return server;
    }

    /**
     * The given connection is about to be used.
     * Update our list of available servers.
     * @param conn The idle connection.
     */

    public void notifyUse(HttpConnection conn) {
	if (debug)
	    System.out.println("+++ connection used");
	connectionsLru.remove(conn);
    }

    /**
     * The given connection can be reused, but is now idle.
     * @param conn The connection that is now idle.
     */

    public synchronized void notifyIdle(HttpConnection conn) {
	if (debug)
	    System.out.println("+++ connection idle");
	connectionsLru.toHead(conn);
	HttpServerState ss = conn.getServer().getState();
	ss.registerConnection(conn);
	notifyAll();
    }

    /**
     * The given connection has just been created.
     * @param conn The newly created connection.
     */

    protected synchronized void notifyConnection(HttpConnection conn) {
	if (debug)
	    System.out.println("+++ notify conn_count " + (conn_count+1)
		               + " / " + conn_max);
	if ( ++conn_count > conn_max )
	    closeAnyConnection();
    }

    /**
     * The given connection has been deleted.
     * @param conn The deleted connection.
     */

    protected void deleteConnection(HttpConnection conn) {
	HttpServerState ss = conn.getServer().getState();
	ss.deleteConnection(conn);
	synchronized(this) {
	    --conn_count;
	    if (debug)
		System.out.println("+++ delete conn_count: " + conn_count);
	    notifyAll();
	}
    }

    protected synchronized boolean tooManyConnections() {
	return conn_count >= conn_max;
    }

    /**
     * Try reusing one of the idle connection of that server, if any.
     * @param server The target server.
     * @return An currently idle connection to the given server.
     */

    protected HttpConnection getConnection(HttpServer server) {
	HttpServerState ss = server.getState();
	return ss.getConnection();
    }

    /**
     * Wait for a connection to come up.
     * @param server, the target server.
     * @exception InterruptedException If interrupted..
     */

    protected synchronized void waitForConnection(HttpServer server)
	throws InterruptedException
    {
	wait();
    }

    /**
     * Close some connections, but pickling the least recently used ones.
     * One third of the max number of connection is cut. This is done to 
     * eliminate the old connections that should be broken already.
     * (no Socket.isAlive());
     * @return A boolean, <strong>true</strong> if a connection was closed
     * <strong>false</strong> otherwise.
     */

    protected boolean closeAnyConnection() {
	boolean saved = false;
	int max = Math.max(conn_max/3, 1);
	for (int i=0; i < max; i++) {
	    HttpConnection conn = (HttpConnection) connectionsLru.removeTail();
	    if ( conn != null ) {
		conn.close();
		deleteConnection(conn);
		if (debug)
		    System.out.println("+++ close request");
		    saved = true;
	    } else
		break;
	}
	return saved;
    }

    /**
     * One of our server handler wants to open a connection.
     * @param block A boolean indicating whether we should block the calling
     * thread until a token is available (otherwise, the method will just
     * peek at the connection count, and return the appropriate result).
     * @return A boolean, <strong>true</strong> if the connection can be
     * opened straight, <strong>false</strong> otherwise.
     */

    protected boolean negotiateConnection(HttpServer server) {
	HttpServerState ss = server.getState();
	if ( ! tooManyConnections() ) {
	    return true;
	} else if ( ss.notEnoughConnections() ) {
	    return closeAnyConnection();
	} else if ( servers.size() > conn_max ) {
	    return closeAnyConnection();
	}
	return false;
    }

    /**
     * A new client connection has been established.
     * This method will try to maintain a maximum number of established
     * connections, by closing idle connections when possible.
     * @param server The server that has established a new connection.
     */

    protected final synchronized void incrConnCount(HttpServer server) {
	if ( ++conn_count > conn_max )
	    closeAnyConnection();
	if (debug)
	    System.out.println("+++ incr conn_count: " + conn_count);
    }

    /**
     * Decrement the number of established connections.
     * @param server The server that has closed one connection to its target.
     */

    protected final synchronized void decrConnCount(HttpServer server) {
	--conn_count;
	if (debug)
	    System.out.println("+++ decr conn_count: " + conn_count);
    }

    /**
     * Run the given request, in synchronous mode.
     * This method will launch the given request, and block the calling thread
     * until the response headers are available.
     * @param request The request to run.
     * @return An instance of Reply, containing all the reply 
     * informations.
     * @exception HttpException If something failed during request processing.
     */

    public Reply runRequest(Request request)
	throws HttpException
    {
	Reply reply  = null;
	int   fcalls = 0;
	// Now run through the ingoing filters:
	RequestFilter filters[] = filteng.run(request);
	if ( filters != null ) {
	    for (int i = 0 ; i < filters.length ; i++) {
		if ((reply = filters[fcalls].ingoingFilter(request)) != null)
		    break;
		fcalls++;
	    }
	}
	// Locate the appropriate target server:
	URL target = request.getURL();
	if ( reply == null ) {
	    HttpServer srv = null;
	    boolean    rtry ;
	    do {
		rtry = false;
		try {
		    URL proxy  = request.getProxy();
		    if ( proxy != null ) 
			srv = lookupServer(proxy.getProtocol(),proxy.getHost(), proxy.getPort());
		    else
			srv = lookupServer(target.getProtocol(),target.getHost(), target.getPort());
		    request.setServer(srv);
		    reply = srv.runRequest(request);
		} catch (HttpException ex) {
		    for (int i = 0; i < fcalls; i++)
			rtry = rtry || filters[i].exceptionFilter(request, ex);
		    if ( ! rtry )
			throw ex;
		} finally {
		    request.unsetServer();
		}
	    } while (rtry);
	}
	// Apply the filters on the way back:
	if ( filters != null ) {
	    while (--fcalls >= 0) {
		Reply frep = filters[fcalls].outgoingFilter(request, reply);
		if ( frep != null ) {
		    reply = frep;
		    break;
		}
	    }
	}
	return reply;
    }

    /**
     * Get this manager's reply factory.
     * The Reply factory is used when prsing incomming reply from servers, it
     * decides what object will be created to hold the actual reply from the 
     * server.
     * @return An object compatible with the MimeParserFactory interface.
     */

    MimeParserFactory factory = null ;

    public MimeParserFactory getReplyFactory() {
	if (factory == null) {
	    factory = new ReplyFactory();
	}
	return factory;
    }

    /**
     * Add a new request filter.
     * Request filters are called <em>before</em> a request is launched, and
     * <em>after</em> the reply headers are available. They allow applications
     * to setup specific request headers (such as PICS, or PEP stuff) on the
     * way in, and check the reply on the way out.
     * <p>Request filters are application wide: if their scope matches
     * the current request, then they will always be aplied.
     * <p>Filter scopes are defined inclusively and exclusively
     * @param incs The URL domains for which the filter should be triggered.
     * @param exs The URL domains for which the filter should not be triggered.
     * @param filter The request filter to add.
     */

    public void setFilter(URL incs[], URL exs[], RequestFilter filter) {
	if ( incs != null ) {
	    for (int i = 0 ; i < incs.length ; i++)
		filteng.setFilter(incs[i], true, filter);
	}
	if ( exs != null ) {
	    for (int i = 0 ; i < exs.length ; i++)
		filteng.setFilter(exs[i], false, filter);
	}
	return;
    }

    /**
     * Add a global filter.
     * The given filter will <em>always</em> be invoked.
     * @param filter The filter to install.
     */

    public void setFilter(RequestFilter filter) {
	filteng.setFilter(filter);
    }

    /**
     * Find back an instance of a global filter.
     * This methods allow external classes to get a pointer to installed
     * filters of a given class.
     * @param cls The class of the filter to look for.
     * @return A RequestFilter instance, or <strong>null</strong> if not
     * found.
     */

    public RequestFilter getGlobalFilter(Class cls) {
	return filteng.getGlobalFilter(cls);
    }

    /**
     * Create a new default outgoing request.
     * This method should <em>always</em> be used to create outgoing requests.
     * It will initialize the request with appropriate default values for 
     * the various headers, and make sure that the request is enhanced by
     * the registered request filters.
     * @return An instance of Request, suitable to be launched.
     */

    public Request createRequest() {
	return (Request) template.getClone() ;
    }

    /**
     * Global settings - Set the max number of allowed connections.
     * Set the maximum number of simultaneous connections that can remain
     * opened. The manager will take care of queuing requests if this number
     * is reached.
     * <p>This value defaults to the value of the 
     * <code>org.w3c.www.http.maxConnections</code> property.
     * @param max_conn The allowed maximum simultaneous open connections.
     */

    public synchronized void setMaxConnections(int max_conn) {
	this.conn_max = max_conn;
    }

    /**
     * Global settings - Set the timeout on the socket
     *
     * <p>This value defaults to the value of the 
     * <code>org.w3c.www.http.Timeout</code> property.
     * @param timeout The allowed maximum microsecond before a timeout.
     */

    public synchronized void setTimeout(int timeout) {
	this.timeout = timeout;
	Enumeration e = servers.elements();
	while (e.hasMoreElements()) {
	    ((HttpServer) e.nextElement()).setTimeout(timeout);
	}
    }

    /**
     * Global settings - set the HTTP parsing lenient or not.
     * @param lenient, true by default, false to detect wrong servers
     */
    public void setLenient(boolean lenient) {
	this.lenient = lenient;
    }

    /**
     * Is this manager parsing headers in a lenient way?
     * @return A boolean.
     */
    public boolean isLenient() {
	return lenient;
    }

    /**
     * Global settings - Set an optional proxy to use.
     * Set the proxy to which all requests should be targeted. If the
     * <code>org.w3c.www.http.proxy</code> property is defined, it will be
     * used as the default value.
     * @param proxy The URL for the proxy to use.
     */

    public void setProxy(URL proxy) {
	template.setProxy(proxy);
    }

    /**
     * Does this manager uses a proxy to fulfill requests ?
     * @return A boolean.
     */

    public boolean usingProxy() {
	return template.hasProxy();
    }

    /**
     * Global settings - Set the request timeout.
     * Once a request has been emited, the HttpManager will sit for this 
     * given number of milliseconds before the request is declared to have
     * timed-out.
     * <p>This timeout value defaults to the value of the
     * <code>org.w3c.www.http.requestTimeout</code> property value.
     * @param ms The timeout value in milliseconds.
     */

    public void setRequestTimeout(int ms) {
    }

    /**
     * Global settings - Define a global request header.
     * Set a default value for some request header. Once defined, the
     * header will automatically be defined on <em>all</em> outgoing requests
     * created through the <code>createRequest</code> request.
     * @param name The name of the header, case insensitive.
     * @param value It's default value.
     */

    public void setGlobalHeader(String name, String value) {
	template.setValue(name, value);
    }

    /**
     * Global settings - Get a global request header default value.
     * @param name The name of the header to get.
     * @return The value for that header, as a String, or <strong>
     * null</strong> if undefined.
     */

    public String getGlobalHeader(String name) {
	return template.getValue(name);
    }

   
    /**
     * Dump all in-memory cached state to persistent storage.
     */

    public void sync() {
	filteng.sync();
    }

    /**
     * Create a new HttpManager.
     * FIXME Making this method protected breaks the static method
     * to create HttpManager instances (should use a factory here)
     * @param props The properties from which the manager should initialize 
     * itself, or <strong>null</strong> if none are available.
     */

    protected HttpManager() {
	this.template       = new Request(this);
	this.servers        = new Hashtable();
	this.filteng        = new FilterEngine();
	this.connectionsLru = new SyncLRUList();
    }

    /**
     * DEBUGGING !
     */

    public static void main(String args[]) {
	try {
	    // Get the manager, and define some global headers:
	    HttpManager manager = HttpManager.getManager();
	    manager.setGlobalHeader("User-Agent", "Jigsaw/1.0a");
	    manager.setGlobalHeader("Accept", "*/*;q=1.0");
	    manager.setGlobalHeader("Accept-Encoding", "gzip");
	    PropRequestFilter filter = 
	      new org.w3c.www.protocol.http.cookies.CookieFilter();
	    filter.initialize(manager);
	    PropRequestFilter pdebug = 
	      new org.w3c.www.protocol.http.DebugFilter();
	    pdebug.initialize(manager);
	    Request request = manager.createRequest();
	    request.setURL(new URL(args[0]));
	    request.setMethod("GET");
	    Reply       reply   = manager.runRequest(request);
	    //Display some infos:
	    System.out.println("last-modified: "+reply.getLastModified());
	    System.out.println("length       : "+reply.getContentLength());
	    // Display the returned body:
	    InputStream in = reply.getInputStream();
	    byte buf[] = new byte[4096];
	    int  cnt   = 0;
	    while ((cnt = in.read(buf)) > 0) 
	      System.out.print(new String(buf, 0, cnt));
	    System.out.println("-");
	    in.close();
	    manager.sync();
	} catch (Exception ex) {
	    ex.printStackTrace();
	}
	System.exit(1);
    }
}



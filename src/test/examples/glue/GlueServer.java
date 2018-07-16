import java.io.*;
import java.lang.*;
import java.net.*;
import java.security.*;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Properties;
import java.util.StringTokenizer;
import java.io.BufferedReader;

import org.telegraphcq.WCH;
import org.telegraphcq.jdbc.*;

public class GlueServer 
{
    static String 	propfile   = "properties.glue";
    static String         wrapper = "demoglue"; 
    static Properties 	properties = new Properties();
  
    public static void main(String[] args) {
	
	int port;    
    
	System.out.println("args.length " + args.length);
    
	if (args.length >=  1) {      
	    propfile = args[0];
	}
	if(args.length >= 2) {
	    wrapper = args[1];
	}
	try {
	    properties.load(new FileInputStream(propfile));
	    System.out.println("properties file is " + propfile);
	    System.out.println("wrapper is " + wrapper);
	}
	catch (IOException ioe) {
	    ioe.printStackTrace();
	}

	port = (new Integer(properties.getProperty("port"))).intValue();

	System.out.println("Starting GlueServer on port " + port);

	new GlueServer().ServerLoop(port,properties);    
    }
  
    void ServerLoop(int port, Properties properties) 
    {
	Socket 		clientConn = null;
	ServerSocket 	listenSock = null;
    
	try {
	    listenSock = new ServerSocket(port);
	}
	catch (Exception e) {      
	    e.printStackTrace();	  
	    System.out.println("Error setting up a socket on port " + port);
	    return;
	}

	while (true) {      
	    try {
		clientConn = listenSock.accept();
		serviceConnection(clientConn,properties);
	    }
	    catch(Exception ex) {
		System.out.println("Exception inside serverLoop.. continuing");
		ex.printStackTrace();
	    }  
	}
    }
  
    void serviceConnection(Socket clientConn, Properties properties) throws Exception
    {
	new ClientConnectionThread(clientConn, properties).start();
    }
}

class RemoteTCQReader extends Thread 
{
    private Properties 	mProp;

    boolean     running;
    String 	tgt_query;
    String 	tgt_host;
    int  	tgt_port;
    String 	tgt_db;
    String 	tgt_user;
    String      orig_view_name;
    String 	orig_wrapper;
    String 	orig_view;
    String 	orig_host;
    int 	orig_port;

    public RemoteTCQReader(Properties prop,
			   String arg_orig_view_name,
			   String arg_orig_wrapper,
			   String arg_orig_view,
			   String arg_orig_host,
			   int 	  arg_orig_port)   
	throws Exception
    {
	String          	tgt_info,tgt_sport;
	StringTokenizer 	t_tokens;
	mProp = prop;
	orig_wrapper = arg_orig_wrapper;  
	orig_view    = arg_orig_view;	 	 
	orig_view_name = arg_orig_view_name;	 	 
	orig_host    = arg_orig_host;	 	 
	orig_port    = arg_orig_port;      

	running = true;
      
	System.out.println("Lookup information regarding request: ");
	System.out.println(" Original VNme:  "  + orig_view_name +
			   "          View   "  + orig_view + 
			   "          Host:  "  + orig_host +
			   "          Port:  "  + orig_port +
			   "          Wrap:  "  + orig_wrapper);

	tgt_info = mProp.getProperty(orig_view);
	System.out.println("tgt_info is now: " + tgt_info);
	t_tokens = new StringTokenizer(tgt_info,";");

	tgt_query  = t_tokens.nextToken();
      
	tgt_host   = t_tokens.nextToken();
	tgt_sport  = t_tokens.nextToken();
	tgt_db     = t_tokens.nextToken();
	tgt_user   = t_tokens.nextToken();

	System.out.println("Target   :" + 
			   "\n  Query  : " + tgt_query +
 			   "\n  Host   : " + tgt_host +
			   "\n  Port   : " + tgt_sport +
			   "\n  User   : " + tgt_user);
	if ((tgt_query == null) || (tgt_host == null) || (tgt_sport == null)) {
	    throw new Exception("Couldn't lookup information for view: " + orig_view);
	}
	tgt_port = (new Integer(tgt_sport)).intValue();
    }

    public void die() 
    {
	running = false;
    }

    public void run() 
    {
	String 			driver, dburl, dbuser, dbpasswd;
	Enumeration     	enumRows = null, enumCols = null;
	PrintWriter     	pw 	 = null;
	CQConnectionManager 	connMan  = null;
	CQResultSet 		cqrset 	 = null;

	while (running) {

	    System.out.println("About to create print writer for writing back to socket");
	    try {
		if (pw != null) {
		    pw.flush();
		    pw.close();
		    pw = null;
		}
		System.out.println("About to create print writer for writing back to socket2");
		pw = WCH.getPrintWriter(orig_host,orig_port,orig_wrapper,orig_view_name);
		 System.out.println("created");
		driver = "org.postgresql.Driver";
		dburl  = "jdbc:postgresql://" + tgt_host + ":" + tgt_port + "/" + tgt_db;
		dbuser = tgt_user;
		dbpasswd = "";
		
		System.out.println("Trying to create CQConnectionManager");
		connMan = new CQConnectionManager(driver,dburl,dbuser,dbpasswd,tgt_query,"cq");
		System.out.println("Successfully Created CQConnectionManager");
		cqrset = connMan.getResultSet();
		
		while (running) {
		    enumRows = cqrset.getNextElements();

		    while (enumRows.hasMoreElements()) {
			String line = "";
			enumCols = (Enumeration) enumRows.nextElement();
			while (enumCols.hasMoreElements()) {
			    CQPair pair = (CQPair) enumCols.nextElement();
			    line = line + pair.value + ",";
			}
			System.out.println("Sending this row to [" + orig_host + ":" + orig_port + "]  " + line);
			pw.println(line);
			pw.flush();
		    }
		}
		break;
	    }
	    catch (Exception e) {
		System.out.println("Exception in RemoteTCQReader thread on view: " + orig_view_name);
		e.printStackTrace();
		try{
		    Thread.currentThread().sleep(5000);
		}catch(Exception sleepException){}
	    }
	}

	try {
	    System.out.print("Closing CQResultSet... ");
	    cqrset.close();
	    connMan.close();
	    
	    System.out.println("Done");
	} catch (Exception exc) {
	    System.out.println("Closing CQResultset Failed.");
	    exc.printStackTrace();
	}
    }
	
}

class ClientConnectionThread extends Thread
{
    private Socket 	mConn;
    private Properties 	mProp;
    private static String wrapper = "demoglue";
    
    Queue threadQueue;

    ClientConnectionThread(Socket sock, Properties properties) throws Exception
    {
	mConn = sock;
	mProp = properties;
	threadQueue = new Queue();
    }


    public void run()
    {
	String 		request;
	StringTokenizer 	r_tokens,t_tokens;
	String 		orig_view;
	String     		orig_host;
	int 		orig_port;
	String          	tgt_info, tgt_query;
	DataOutputStream dos =null;
	System.out.println("got a connection on socket " + mConn);
	int numstreams = 1;

	RemoteTCQReader tcqr;
	BufferedReader brd;
	String         donerequest;

	try {
	    System.out.println("about to construct data input stream");
	    brd = new BufferedReader(new InputStreamReader(mConn.getInputStream()));
	    request = brd.readLine();
	    if (request == null) {
		throw new Exception("GlueServer received NULL from consumer");
	    }
	
	    System.out.println("Request   : " + request);

	    r_tokens = new StringTokenizer(request,";");

	    orig_view  = r_tokens.nextToken();
	    orig_port  = (new Integer(r_tokens.nextToken())).intValue();
	    orig_host  = mConn.getInetAddress().getHostName();

	    tgt_info = mProp.getProperty(orig_view);
	    t_tokens = new StringTokenizer(tgt_info,";");
	    tgt_query  = t_tokens.nextToken();
	    if ("UNION".equals(tgt_query)) {
		System.out.println("In UNION context"); 
		String o_view = null;
		try {
		    while  (true) {
			o_view = t_tokens.nextToken();
			System.out.println("For view: " + o_view);
			System.out.println("Starting RemoteTCQReader");
			tcqr = new RemoteTCQReader(mProp,orig_view,wrapper,o_view,orig_host,orig_port);
			tcqr.start();
			threadQueue.push(tcqr);
			System.out.println("Done starting RemoteTCQReader");
		    }
		} catch (Exception tokenexception) {}
	    }
	    else {
		System.out.println("Starting RemoteTCQReader");
		tcqr = new RemoteTCQReader(mProp,orig_view,wrapper,orig_view,orig_host,orig_port);
		tcqr.start();
		threadQueue.push(tcqr);
		System.out.println("Done starting RemoteTCQReader");
	    }
	    System.out.println("Done processing request ... ");
	    while (true) {
		System.out.println("-------------------------------------------------------------\n");
		System.out.println("Checking if the pull wrapper has shut down yet ... \n");
 		if (mConn.isInputShutdown()){
 		    System.out.println("\n\nSOCKET INPUT HAS BEEN CLOSED.  SHUTTING DOWN QUERIES\n\n");
 		    killThreads();
 		}
 		if (mConn.isOutputShutdown()) {
 		    System.out.println("\n\nSOCKET OUTPUT HAS BEEN CLOSED.  SHUTTING DOWN QUERIES\n\n");
 		    killThreads();
 		}
		donerequest = brd.readLine();     // I hope this blocks ...
		if (donerequest != null && donerequest.indexOf("DONE") >= 0) {
		    System.out.println("\n\nSOCKET OUTPUT HAS BEEN CLOSED.  SHUTTING DOWN QUERIES\n\n");
		    killThreads();
		    break;
		}
		Thread.currentThread().sleep(2000);
	    }
	    
	}
	catch (Exception dosexception ) {
	    System.out.println("exception:");
	    dosexception.printStackTrace();
	    try {
		if(dos != null) dos.writeBytes("ERROR_MESSAGE: " + dosexception.getMessage() + "\n");
	    }
	    catch(Exception ex) {}
	}
	finally {
	    try {
		System.out.println("Closing socket !! ");
		mConn.close();
	    }
	    catch (Exception t) {}
	    System.out.println("GlueServer thread exiting");
	}
    }

    private void killThreads() {
	RemoteTCQReader rtcqr = null;
	while (threadQueue.hasMoreElements() ) {
	    rtcqr = (RemoteTCQReader)threadQueue.pop();
	    if(rtcqr == null) continue;

	    rtcqr.die();
	    
	}
    }

}

class Queue extends Vector{
        
    public Queue(){
	super();
    }

    public void push(Object o) {
	add(o);
    }

    public Object pop() {
	if (size() > 0)
	    return remove(0);
	return null;
    }

    public boolean hasMoreElements() {
	return !(size() <= 0);
    }

}
    

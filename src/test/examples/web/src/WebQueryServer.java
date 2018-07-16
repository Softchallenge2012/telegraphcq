import java.net.*;
import java.io.*;
import HtmlGet.HtmlGet;
import java.util.Vector;

class WebQueryServer 
{
    public static void main (String [] args) throws Exception
    {
	if(args.length < 1 ||
	   args.length > 2)
	    {
		System.out.println("usage: WebQueryServer port [jscdir]");
		System.exit(1);
	    }
	else
	    {
		String jscdir = null;
		int port = Integer.parseInt(args[0]);
		if(args.length == 2 )
		{
			jscdir = args[1];
		}
		new WebQueryServer().ServerLoop(port, jscdir);
	    }


    }


     void ServerLoop(int port, String jscdir) 
    {
	Socket clientConn = null;
	ServerSocket listenSock=null;
	try 
	    {
		
		listenSock = new ServerSocket(port);
	    }
	catch (Throwable t)
	    {
		System.out.println("Error setting up a socket on port " +port);
		return;
	    }

	while (true)
	    {
		try 
		    {
			clientConn = listenSock.accept();
			serviceConnection(clientConn, jscdir);
		    }
		catch(Throwable t)
		    {
			System.out.println("Exception inside serverLoop.. continuing");
			t.printStackTrace();
		    }

	    }
    }

    void serviceConnection(Socket clientConn, String jscdir) throws Exception
    {

    	new ClientConnectionThread(clientConn, jscdir).start();
    }


}

class ClientConnectionThread extends Thread
{
    ClientConnectionThread(Socket sock, String jscdir) throws Exception
    {
	mConnection = sock;
	mJSCDir = jscdir;
	File cookietemp =  File.createTempFile("WebQueryServer", ".cookies");
	File historytemp =  File.createTempFile("WebQueryServer", ".history");
	String cookiefile = cookietemp.getCanonicalPath();
	String historyfile = historytemp.getCanonicalPath();
	 
	mHtmlGet = new HtmlGet(cookiefile, historyfile,
				mJSCDir,
				false);
    }

    public void run()
    {
	String read;
	System.out.println("got a connection on socket " + mConnection);

	try
	    {
		System.out.println("about to construct data input stream");
		DataInputStream dis = new DataInputStream(mConnection.getInputStream());
		System.out.println("about to construct data output stream");
		DataOutputStream dos = new DataOutputStream(mConnection.getOutputStream());
		
		while((read = dis.readLine()) !=null)
		    {
			System.out.println("read: " +read);
			if(read.equalsIgnoreCase("quit"))
			    break;
			else
			    {
				System.out.println("about to process a command");
				try
				{
					mHtmlGet.parseAndProcessCommand(read,false);
				Vector v = mHtmlGet.getResults();
				for(int i=0; i < v.size(); i++)
				    {
					dos.writeBytes(mHtmlGet.objectArrayToCSV((Object [])v.get(i)));
				    }						     
					dos.writeBytes("DONE WITH PROBE\n");
					v.clear();
				}
				catch(Throwable t)
				{
					dos.writeBytes("ERROR:" + t.getMessage() + "\n");		
				}
				

			    }
		    }
	    }
	catch(Throwable t)
	    {
		System.out.println("exception:");
		t.printStackTrace();
	    }
	finally 
	    {
		try {
		    mConnection.close();
		}
		catch (Throwable t) {}
		System.out.println("WebQueryServer thread exiting");
	    }
    }
    private String mJSCDir;
    private Socket mConnection;
    private HtmlGet mHtmlGet;
}
    

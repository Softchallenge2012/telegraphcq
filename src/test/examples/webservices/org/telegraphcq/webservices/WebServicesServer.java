import java.net.*;
import java.io.*;
import java.util.Vector;
import java.util.Hashtable;
import java.util.Iterator;

class WebServicesServer 
{
    public static void main (String [] args) throws Exception
    {
	if(args.length < 1 ||
	   args.length > 4)
	    {
		System.out.println("usage: WebServicesServer port wrapchhost wrapchport");
		System.exit(1);
	    }
	else
	    {
		String jscdir = null;
		int port = Integer.parseInt(args[0]);
		String wchhost = args[1];
		int wchport = Integer.parseInt(args[2]);
		new WebServicesServer().ServerLoop(port, wchhost, wchport);
	    }


    }


     void ServerLoop(int port, String wchhost, int wchport) 
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
			serviceConnection(clientConn, wchhost, wchport);
		    }
		catch(Throwable t)
		    {
			System.out.println("Exception inside serverLoop.. continuing");
			t.printStackTrace();
		    }

	    }
    }

    void serviceConnection(Socket clientConn, String wchhost, int wchport) throws Exception
    {

    	new ClientConnectionThread(clientConn, wchhost, wchport).start();
    }


}

class ClientConnectionThread extends Thread
{
    ClientConnectionThread(Socket sock,String wchhost, int wchport) throws Exception
    {
	mConnection = sock;
	mWchhost = wchhost;
	mWchport = wchport;
	mWebServicesInvokers = new Hashtable();
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
				    int startidx =0;
				    int endidx=0;
				    endidx = read.indexOf(",", startidx);
				    String pkg = read.substring(startidx, endidx);
				    System.out.println("package=" +pkg +"\n");
				    startidx=endidx+1;
				    endidx = read.indexOf(",", startidx);
				    String service = read.substring(startidx, endidx);
				    System.out.println("service=" +service +"\n");
				    startidx=endidx+1;
				    endidx = read.indexOf(",", startidx);
				    String schema = read.substring(startidx, endidx);
				    System.out.println("schema=" +schema+"\n");
				    startidx=endidx+1;
				    endidx = read.indexOf(",", startidx);
				    String port = read.substring(startidx, endidx);
				    System.out.println("port=" +port+"\n");
				    startidx=endidx+1;
				    endidx = read.indexOf(",", startidx);
				    String operation = read.substring(startidx, endidx);
				    System.out.println("operation=" +operation +"\n");
				    read = read.substring(endidx+1);
				    System.out.println("args=" +read +"\n");
				    TCQWebServicesInvoker ti = (TCQWebServicesInvoker)mWebServicesInvokers.get(pkg+service);
				    if(ti == null)
					{
					    ti = new TCQWebServicesInvoker(pkg,
									   service,
									   schema,
									   port,
									   mWchhost,
									   ""+mWchport,
									   true);
					    mWebServicesInvokers.put(pkg+service, ti);
					}
				    Hashtable res = ti.TCQInvokeOperation(operation, read);
				    String t = operation.replaceAll("[0-9]+$","");
				    Vector v = (Vector)res.get(t);
				    res.remove(t);
				    ti.sendResults(operation, mPrintWriters, res);
				    if(v != null)
					{
					    for(Iterator i = v.iterator(); 
						i.hasNext();)
						{
						    String row = (String)i.next();
						    System.out.println("sending row " + row + " back on connection"); 
						    dos.writeBytes(row + "\n");
						    dos.flush();
						}


					}
				    
				    dos.writeBytes("DONE WITH PROBE\n");
				    dos.flush();
				}
				catch(Throwable t)
				{
					dos.writeBytes("ERROR MESSAGE:" + t.getMessage() + "\n");		
					dos.flush();
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
		System.out.println("WebServicesServer thread exiting");
	    }
    }
    private String mWchhost;
    private int mWchport;
    private Socket mConnection;
    private Hashtable mWebServicesInvokers= new Hashtable();
    private Hashtable mPrintWriters=new Hashtable();


}
    

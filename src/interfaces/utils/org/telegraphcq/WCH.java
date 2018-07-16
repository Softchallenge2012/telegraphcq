package org.telegraphcq;

import java.io.PrintWriter;
import java.io.DataOutputStream;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.io.IOException;


public class WCH
{
    public static PrintWriter getPrintWriter(String host, int port,
				      String wrapper, String stream)
	throws IOException
    {
	int i=0;

	InetAddress hostaddr = InetAddress.getByName(host);
	return getPrintWriter(hostaddr, port, wrapper,stream);

    }

    public static PrintWriter getPrintWriter(InetAddress hostaddr, int port,
				      String wrapper, String stream)
	throws IOException
    {
	Socket sock = new Socket(hostaddr,port);
	return getPrintWriter(sock,wrapper,stream);
    }

    public static PrintWriter getPrintWriter(Socket sock, String wrapper, String stream) 
	throws IOException
    {
	String wchconnect = computeWCHConnectionString(wrapper,stream);
	DataOutputStream dos = new DataOutputStream(sock.getOutputStream());
	System.out.println("WCH connection string  is:"+wchconnect);
	dos.writeBytes(wchconnect);
	dos.flush();
	PrintWriter pw = new PrintWriter(new OutputStreamWriter(dos));
	return pw;
    }
	
    static String  computeWCHConnectionString(String wrappername, String fqstream)
    {
	String sourcename = wrappername+","+fqstream;
	int length = sourcename.length();

	if(length < 10)
	    {
		sourcename ="   "+length+sourcename;
	    }
	else if(length < 100) 
	    {
		sourcename="  "+length+sourcename;
	    }
	else if(length < 1000)
		{
		    sourcename=" "+length+sourcename;
		}
	else 
	    return null;
	return sourcename;
    }
}

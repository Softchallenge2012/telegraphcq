package org.telegraphcq;
import java.io.*;
class source
{

    public static void main(String [] args)
    {
	if(args.length != 3)
	    {
		System.err.println("source: host port wrapper,stream");
		System.exit(1);
	    }
	String host = args[0];
	int port = Integer.parseInt(args[1]);
	String ws[] = args[2].split(",");
	String wrapper=ws[0];
	String stream=null;
	if(ws.length > 1) stream = ws[1];
	PrintWriter pw=null;
	DataInputStream in = null;
	
	try 
	    {
		pw = WCH.getPrintWriter(host,port,wrapper,stream);
		in = new DataInputStream(System.in);
		String read;
		while((read = in.readLine()) != null)
		{
		    pw.println(read);
		    pw.flush();
		}
		pw.close();
		pw=null;
	    }
	catch (EOFException e)
	    {
		if(pw != null) pw.close();
		pw=null;
	    }
	catch (IOException e)
	    {
		if(pw != null) pw.close();
		pw=null;
		System.err.println(e.toString());
	    }


    }
}

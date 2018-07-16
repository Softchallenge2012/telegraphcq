import org.apache.axis.wsdl.toJava.Utils;
import java.util.Hashtable;
import java.lang.reflect.*;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.Enumeration;
import java.util.Vector;


public class TCQWebServicesInvoker
{
    static 
    {
	try {
	TCQWebServicesInvoker.tcqinvokeparams=new Class[2];
	TCQWebServicesInvoker.tcqinvokeparams[0]=Class.forName("java.lang.String");
	TCQWebServicesInvoker.tcqinvokeparams[1]=Class.forName("java.lang.Integer");
	}
	catch (Exception e)
	    {
		System.out.println("cannot initialize static vars");
	    }
   }

    public static void main(String args[] ) throws Exception
    {
	Hashtable printwriters = new Hashtable();
	String pkg = args[0];
	String servicename =args[1];
	String schemaname=args[2];
	String portname=args[3];
	String operation = args[4];
	String csv=args[5];


	System.out.println("package is " + pkg);
	System.out.println("servicename is " + servicename);
	System.out.println("schema" + schemaname);
	System.out.println("port " + portname);
	System.out.println("operation is " + operation);
	System.out.println("csv is " + csv);
	TCQWebServicesInvoker i = new TCQWebServicesInvoker(pkg,
							    servicename,
							    schemaname,
							    portname,
							    "nohost",
							    "0",
							    false);
	
	Hashtable res = i.TCQInvokeOperation(operation, csv);
	i.sendResults(operation, printwriters, res);
    }
    public TCQWebServicesInvoker(String pkg,
				 String serviceName,
				 String schemaname,
				 String portname,
				 String wchHost,
				 String wchPort,
				 boolean useWch)
    {
	mPackagename = pkg;
	mServicename = serviceName;
	mSchemaname = schemaname;
	mPortname = portname;
	mWchport = Integer.parseInt(wchPort);
	mWchhost = wchHost;
	mUseWch = useWch;
    }
    public  Hashtable TCQInvokeOperation(String operationName,
						String csvparams) 
	throws Exception
    {
	String upper_servicename = Utils.capitalizeFirstChar(mServicename);
	String LocatorName = mPackagename +"." +  upper_servicename+"Locator";
	String portMethod = "get"+Utils.capitalizeFirstChar(mPortname);
	String opmethodname ="TCQInvoke" +Utils.capitalizeFirstChar(operationName);
	System.out.println(LocatorName);
	System.out.println(opmethodname);
	Object tcqinvokeargs [] = {csvparams, getUniqueId()};
	Class tcqinvoketype[]  = { Class.forName("java.lang.String"), Class.forName("java.lang.Integer")}; 
	Class bc = Class.forName(LocatorName);
	Object locatorobj = bc.newInstance();
	Method portmethod = locatorobj.getClass().getMethod(portMethod,null);
	Object binding = portmethod.invoke(locatorobj, new Object[0]);
	Class bindingClass = binding.getClass();
        Method tcqinvoke = binding.getClass().getMethod(opmethodname, 
							tcqinvoketype);
	System.out.println("calling " + operationName + " with " + csvparams);
	Hashtable results = (Hashtable)tcqinvoke.invoke(binding, tcqinvokeargs);
	System.out.println("call returned");
	return results;
    }

    public void sendResults(String operation, Hashtable printwriters, Hashtable csvtuples)
	throws Exception
    {
	PrintWriter pw=null;
	String typename;
	String streamname;
	String wrappername;
	Vector tuples=null;
	// printwriters has key=typename and value as a printwriter
	// csvtuples has key=typename and value= vector of strings
	for(Enumeration typekeys = csvtuples.keys(); typekeys.hasMoreElements();)
	    {
		typename = (String)typekeys.nextElement();
		streamname = typenameToStream(typename);
		wrappername = typenameToWrapper(typename);
		System.out.println("processing typename="+typename + " to stream " + streamname);
		pw = getPrintWriter(printwriters, wrappername, streamname);
		tuples = (Vector)csvtuples.get(typename);
		for(Iterator t = tuples.iterator(); t.hasNext();)
		    {
			String tup = (String)t.next();
			System.out.println(tup);
			pw.println(tup);
		    }
		pw.flush();
	    }
		
    }

    public String typenameToStream(String typename) 
    { 
	return mSchemaname.toLowerCase()+"."+typename.toLowerCase();
    }
    public String typenameToWrapper(String typename)
    {
	return "csvwrapper";
	//	return mSchemaname.toLowerCase()+"_"+typename.toLowerCase();
    }
    
    public PrintWriter getPrintWriter(Hashtable printwriters, String wrappername, String streamname)
	throws Exception
    {
	PrintWriter pw = (PrintWriter) printwriters.get(streamname);
	if(pw == null)
	    {
		if(mUseWch) 
		    {
			pw = org.telegraphcq.WCH.getPrintWriter(mWchhost, mWchport, 
						    wrappername, streamname);
		    }
		else
		    {
			pw = new PrintWriter(System.out);
		    }
		if(pw == null) 
		    {
			throw new Exception("unable to get print writer");
		    }
		printwriters.put(streamname, pw);
	    }
	return pw;
    }
    public static Integer getUniqueId() { return new Integer(mUniqueId++);} 
    protected String mServicename;
    protected String mSchemaname;
    protected String mPortname;
    protected String mPackagename;
    protected String mWchhost;
    protected int mWchport;
    protected boolean mUseWch;
    protected static int mUniqueId=9000;
    protected static Class tcqinvokeparams[] = null; 

}

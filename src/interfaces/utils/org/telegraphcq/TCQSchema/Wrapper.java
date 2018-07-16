
package org.telegraphcq.TCQSchema;
import org.telegraphcq.exceptions.*;
import java.util.Hashtable;
import java.util.Enumeration;

public class Wrapper
{
    public static final int ADD=990;
    public static final int DROP=995;

    protected String mName;
    protected String mInitFunc;
    protected String mNextFunc;
    protected String mDoneFunc;
    protected String mRescanFunc;
    protected Hashtable mAttributes=new Hashtable();
    protected static Hashtable sBuiltins = null;
    
    static {
	sBuiltins = new Hashtable();
	sBuiltins.put("csvwrapper", new Boolean(true));
    }

    public Wrapper(String name)
    {
	mName = name;
    }
    public void setName(String n) { mName=n;}
    public void setInit(String n) { mInitFunc=n;}
    public void setNext(String n) { mNextFunc=n;}
    public void setDone(String n) { mDoneFunc=n;}
    public void setRescan(String n) { mRescanFunc=n;}
    public void makePullWrapper(String host, String port)
    {
	addAttribute("WRAPPERTYPE", "PULL");
	addAttribute("HOST",host);
	addAttribute("PORT",port);
    }

    public void addAttribute(String key, String value)
    {
	mAttributes.put(key,value);
    }

    public String  getName() { return mName;}
    public String getInit() { return mInitFunc;}
    public String getNext() { return mNextFunc;}
    public String getDone() { return mDoneFunc;}
    public String getRescan(String n) { return mRescanFunc;}
    public Hashtable getAttributes() {return mAttributes; }


    public String generateCreateWrapperStatement()
    {
	if(IsBuiltin()) return " -- " + mName +" is builtin\n";
	boolean needwrapinfo=false;
	String key="";
	String value="";
	String ret =  "create wrapper " + mName+ " ( " ;
	ret += "init="+mInitFunc;
	ret += ",next="+mNextFunc;
	ret += ",done="+mDoneFunc;
	if(mRescanFunc != null) ret += ",rescan=" + mRescanFunc;
	ret += ")";
	String optinfo = generateKeyValueList(mAttributes);
	if(!optinfo.equals(""))
	    {
		ret += " with wrapperinfo '" + optinfo + "';\n";
	    }
	return ret;

    }

    public String generateDropWrapperStatement()
    {
	if(IsBuiltin()) return " -- " + mName +" is builtin\n";
	return "drop wrapper " + mName  +";";
    }
 

    public  boolean IsBuiltin()
    {
	return (sBuiltins.get(mName) != null);
    }

    static String generateKeyValueList(Hashtable h) 
    {
	String optinfo="";
	String key;
	String value;
	boolean needsep=false;
	for(Enumeration e = h.keys(); e.hasMoreElements(); )
	    {
		if(needsep == true) optinfo += ",";
		key = (String)e.nextElement();
		value = (String)h.get(key);
		optinfo += key+"="+value;
		needsep=true;

	    }
	return optinfo;
    }
}


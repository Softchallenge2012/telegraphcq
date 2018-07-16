package org.telegraphcq.webservices.utils;

import org.apache.axis.wsdl.toJava.Utils;
import java.util.Vector;
import java.util.Hashtable;

public class TUtils
{
    public static String xmlNameToSQL(String name)
    {
	return Utils.xmlNameToJava(name);
    }

    public static Vector getTupleVector(Hashtable h, String typename)
    {
	Vector v = (Vector)h.get(typename);
	if(v == null)
	    {
		v = new Vector();
		h.put(typename,v);
	    }
	return v;
    }

    public static String stripGeneratedFieldsFromCSV(String csv)
    {
	int fromidx=0;
	int numfields=5;
	int idx=0;
	for(; numfields > 0 ; numfields --)
	    {
		idx = csv.indexOf(",",idx);
		idx++;
	    }
       String substr = csv.substring(idx);
       System.out.println("idx="+idx+"substr="+substr);
	return substr;

    }

    public static String getTabs(int numtabs)
    {
	String tabs="";
	for(int i =0 ; i < numtabs; i++) tabs +="\t";
	return tabs;
    }
    public static String quote(String s) { return "\"" +s+ "\"";}
    public static String toCSVEscapedValue(String data)
    {
	return data.replaceAll(",", "");
    }

}

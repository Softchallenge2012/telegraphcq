package org.telegraphcq.webservices.utils;

import org.apache.axis.wsdl.symbolTable.SymbolTable;
import org.apache.axis.wsdl.symbolTable.TypeEntry;
import org.apache.axis.wsdl.toJava.Utils;
import java.util.Vector;

public class TypeConversion
{
    public static int StringToInt(String val) { return Integer.parseInt(val); } 
    public static float StringToFloat(String val) { return Float.parseFloat(val);}
    public static String StringToString(String val) { return val;}
    public static boolean StringToBoolean(String val) { return new Boolean(val).booleanValue();}

    public static String getNQTypeName(String typename)
    {
	String ret;
	int idx = typename.lastIndexOf('.');
	if(idx < 0) ret = typename;
	else ret= typename.substring(idx+1);
	System.out.println("getNQTypeName returns " + ret);
	return ret;
   }

    public static boolean IsSimpleType(String typename) 
    {
	if(typename.equalsIgnoreCase("java.lang.String") ||
	   typename.equalsIgnoreCase("Byte") ||
	   typename.equalsIgnoreCase("Int") ||
	   typename.equalsIgnoreCase("Float") ||
	   typename.equalsIgnoreCase("Double") ||
	   typename.equalsIgnoreCase("Boolean")) 
	    {
		return true;
	    }
	else
	    {
		return false;
	    }
    }
    public static boolean IsSimpleArrayType(String typename)
    {
	if(!typename.endsWith("[]")) return false;
	int idx = typename.indexOf("[]");
	String basetype = typename.substring(0,idx);
	System.out.println("isSimpleArrayType finds basetype of " + basetype);
	return IsSimpleType(basetype);
    }
    
    public static String ArrayTypeToString(String typename, Object array) throws Exception
    {
	if(typename.startsWith("byte"))
	    {
		byte val=0;
		byte[] b = (byte[])array;
		StringBuffer outstring=new StringBuffer(4096);
		for(int i=0; i < b.length; i++)
		    {
			val = b[i];
			if(val == '\\') outstring.append("\\");
			else
			    {
				outstring.append("\\");
				outstring.append((val & 07) + '0');
				val >>= 3;
				outstring.append((val & 07) + '0');
				val >>= 3;
				outstring.append((val & 03) + '0');
			    }
		    }
		return outstring.toString();
	    }
	throw new Exception("Cannot convert array of type " + typename + " to string");
    }
    public static boolean IsGeneratedType(String typename)
    {
	if(IsSimpleArrayType(typename)) return false;
	return (IsSimpleType(typename) == false &&
		typename.equals("java.lang.String") == false);
    }

    public static boolean  isEnumType(TypeEntry te, SymbolTable symtab)
    {
	Vector tmp =  Utils.getEnumerationBaseAndValues(te.getNode(), symtab);
	return tmp!=null;
    }

}

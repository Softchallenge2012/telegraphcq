package org.telegraphcq.webservices.utils;
import java.io.PrintWriter;
import java.util.Hashtable;
import java.util.Vector;
import java.util.List;

import org.apache.axis.wsdl.toJava.Utils;
import org.apache.axis.wsdl.symbolTable.BindingEntry;
public class CodeGen {

    public static void generateArrayTypeToCSV(PrintWriter pw, int numtabs, String typename, 
				   String htname, String arrayname, 
				   String loopvar, String thisname, int arraycolno)
    {
	boolean issimple = TypeConversion.IsSimpleArrayType(typename);
	String basename= typename.substring(0, typename.length()-2);
	String arraydecl = basename +" " + arrayname + "[]= null;";
	if(issimple)
	    {
		pw.println(TUtils.getTabs(numtabs) + "subtyperesults = (Vector)" + htname + ".get(\""+ TypeConversion.getNQTypeName(basename) + "\");");
		pw.println(TUtils.getTabs(numtabs) + "if (subtyperesults == null) subtyperesults = new Vector();");		   
	    }
	pw.println(TUtils.getTabs(numtabs) +"for(int "+loopvar+"=0; "+loopvar+" < " + arrayname+".length; "+loopvar+"++)");
	pw.println(TUtils.getTabs(numtabs) +"{");
	numtabs++;
	if(issimple)
	    {
		pw.println(TUtils.getTabs(numtabs) + 
			   "subcsv = operationid +"  + 
			   TUtils.quote(",") +"+"+ thisname + "." +" getMyId()+" +
			   TUtils.quote(",") +"+" +  TUtils.quote(""+arraycolno) + "+" + 
			   TUtils.quote(",") +"+"+loopvar+"+" + TUtils.quote(",") + "+TUtils.toCSVEscapedValue(" +  
			   arrayname + "["+loopvar+"]);");
		pw.println(TUtils.getTabs(numtabs) + "subtyperesults.add(TUtils.toCSVEscapedValue(subcsv));");
	    }
	else
	    {
		pw.println(TUtils.getTabs(numtabs) +arrayname+"["+loopvar+"].ObjectToCSV(operationid,"+thisname+"."+"getMyId(),"+arraycolno+","+loopvar+","+htname+");");
	    }
	numtabs--;
	pw.println(TUtils.getTabs(numtabs) +"}");
	if(issimple) pw.println(TUtils.getTabs(numtabs) + "tuples.put(\""+org.telegraphcq.webservices.utils.TypeConversion.getNQTypeName(basename) +"\", subtyperesults);");
	
	    }
    public static void generateFlattenTypeToCSV(PrintWriter pw,
						int numtabs, String tn, 
						String htvar, 
						String objvar, String csvvar)
    {
	String typename =CodeGen.getStorageTypeName(tn);
	String tmpvarv = getTempVar(pw, numtabs, "java.util.Vector");
	String tmpvarv2 = getTempVar(pw, numtabs, "java.util.Vector");
	String tmpvars = getTempVar(pw, numtabs, "java.lang.String");

	pw.println(TUtils.getTabs(numtabs) + 
		    tmpvarv + "= (Vector)" + htvar + ".get(" + TUtils.quote(typename) + ");");
	pw.println(TUtils.getTabs(numtabs) + htvar+".put("+TUtils.quote(typename)+ ", new Vector());");
	pw.println(TUtils.getTabs(numtabs) + objvar + "." + "ObjectToCSV(0,0,0,0,"+htvar+");");
	pw.println(TUtils.getTabs(numtabs) + tmpvarv2+" = (Vector)" + htvar + ".get(" + TUtils.quote(typename) + ");");
	pw.println(TUtils.getTabs(numtabs) +  tmpvars +"  = ((String)"+ tmpvarv2 +".get(0)).substring(8); ");
	pw.println(TUtils.getTabs(numtabs) + csvvar + " += " + tmpvars +";");
	pw.println(TUtils.getTabs(numtabs) + " if(" + tmpvarv +"  != null) " + htvar+".put("+TUtils.quote(typename)+ ", " + tmpvarv +");");
	doneTempVar("java.util.Vector", tmpvarv);
	doneTempVar("java.util.Vector", tmpvarv2);
	doneTempVar("java.lang.String", tmpvars);	
   }
    
    public static void generateSimpleTypeToCSV(PrintWriter pw, int numtabs, String expr)
    {
	pw.println(TUtils.getTabs(numtabs) +"csv += \",\"+TUtils.toCSVEscapedValue(\"\"+"+expr + ");");
    }

    public static void generateEnumToCSV(PrintWriter pw, int numtabs, String csvvar, String expr)
    {
	generateSimpleTypeToCSV(pw, numtabs, expr);
    }
    public static void generateEnumFromString(PrintWriter pw, int numtabs, String classname, String val)
    {
	pw.println(TUtils.getTabs(numtabs) + classname+".fromString("+val+");");
    }


    public static String getStorageTypeName(String typename)
    {
	return Utils.getJavaLocalName(typename);
    }

    public static String getTempVar(PrintWriter pw, int numtabs, 
				    String typename)
    {
	List varnames = (List)tmpvars.get(typename);
	String varname = null;
	if(varnames == null || varnames.size() == 0)
	    {
	        varname = "tmp"+tmpvaridx++;
	        String decl = typename + " " + varname +";";
		pw.println(TUtils.getTabs(numtabs) + decl);
	    }
	else
	    { 
		varname = (String)varnames.remove(0);
	    }
	return varname;
    }
    public static void doneTempVar(String typename, String varname)
    {
	List varnames = (List)tmpvars.get(typename);
	if(varnames == null)
	    {
		varnames = new Vector();
		tmpvars.put(typename, varnames);
	    }
	varnames.add(varname);
    }
    
    
    public static void clearTempVars()
    {
	tmpvars.clear();
    }

    public static String getSchemaName(BindingEntry be)
    {
		String schemaName =  TUtils.xmlNameToSQL(be.getQName().getLocalPart());
		int idx = schemaName.indexOf("Binding");
		if(idx > 0) schemaName = schemaName.substring(0,idx);
		schemaName = Utils.capitalizeFirstChar(schemaName);
		return schemaName;
    }



  public static int getGUID() { return guid++;}
    protected static Hashtable tmpvars = new Hashtable();
    protected static int tmpvaridx=0;
    protected static int guid=2004;
    
}


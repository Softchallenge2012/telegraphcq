import org.apache.axis.Constants;
import org.apache.axis.utils.JavaUtils;
import org.apache.axis.utils.Messages;
import org.apache.axis.wsdl.symbolTable.ElementDecl;
import org.apache.axis.wsdl.symbolTable.SchemaUtils;
import org.apache.axis.wsdl.symbolTable.TypeEntry;
import org.w3c.dom.Node;
import org.apache.axis.wsdl.toJava.Emitter;
import org.apache.axis.wsdl.toJava.JavaWriter;
import org.apache.axis.wsdl.toJava.Utils;
import org.apache.axis.wsdl.gen.Parser;
import org.telegraphcq.webservices.utils.TypeConversion;
import org.telegraphcq.webservices.utils.TUtils;
import org.telegraphcq.webservices.utils.CodeGen;
import javax.xml.namespace.QName;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Vector;

/* todo: needs to be extended to handle enumeration types via CodeGen methods
 */


public class TCQJavaBeanWriter extends org.apache.axis.wsdl.toJava.JavaBeanWriter
{
    protected TCQJavaBeanWriter(
            Emitter emitter,
            TypeEntry type,
            Vector elements,
            TypeEntry extendType,
            Vector attributes,
            JavaWriter helper) {    
	super(emitter, type, elements, extendType, attributes, helper);
    }
    protected void writePackage(PrintWriter pw) throws IOException {
	super.writePackage(pw);

	pw.println("import org.telegraphcq.webservices.utils.TypeConversion;");
	pw.println("import org.telegraphcq.webservices.utils.TUtils;");
	pw.println("import org.telegraphcq.webservices.utils.MutableInt;");
	pw.println("import java.util.Vector;");
	pw.println("import java.util.Hashtable;");

    }

    protected void writeFileBody(PrintWriter pw) throws IOException {

	super.writeFileBody(pw);
	writeMyIdMethods(pw);
	writeObjectFromCSV(pw);
	writeToCSV(pw);

    }

    protected void writeMyIdMethods(PrintWriter pw) throws IOException
    {
	CodeGen.clearTempVars();
	int numtabs=1;
	pw.println(TUtils.getTabs(numtabs) + " // methods to get and set id for TCQ ");
	pw.println(TUtils.getTabs(numtabs) + " public int getMyId() {  return " + CodeGen.getGUID() + ";}");

    }

    protected void writeMemberFields() {
	CodeGen.clearTempVars();
	super.writeMemberFields();
	pw.println("public int mMyId; // added for tcq ");
    };
    protected void writeObjectFromCSV(PrintWriter pw) 
    {
	CodeGen.clearTempVars();
	int numtabs=1;


	pw.println(TUtils.getTabs(numtabs) +"public static " + getClassName() + " ObjectFromCSV(String [] params, MutableInt lastConsumed) throws Exception");
	pw.println(TUtils.getTabs(numtabs) +"{");
	numtabs++;
	for(int i =0; i < names.size() ; i+=2)
	    {
		String typeName = (String) names.get(i);
		if(typeName.endsWith("[]"))
		{
		    pw.println(TUtils.getTabs(numtabs)+ " // object contains array types... cannot generate ObjectFromCSV ");
		    pw.println(TUtils.getTabs(numtabs) + "throw new Exception(\"ObjectFromCSV not allowed because type " + getClassName() + " contains an array\");");
		    pw.println(TUtils.getTabs(numtabs) + "}");
		    return;
		}
	    }
	pw.println(TUtils.getTabs(numtabs)+"if(params.length - lastConsumed.getValue() < " + names.size()/2 + ")");
	pw.println(TUtils.getTabs(numtabs)+"{");
	numtabs++;
	pw.println(TUtils.getTabs(numtabs) + " throw new Exception(\"" + getClassName() + ".objectToCSV got a csv with \" + (params.length -lastConsumed.getValue()) + \" parameters, but expected " + names.size()/2 +"\");");
	numtabs--;
	pw.println(TUtils.getTabs(numtabs)+"}");
	
	pw.println(TUtils.getTabs(numtabs) + getClassName() +" ni = new " + getClassName() +"();");


	String methodname  = "";
	String typeConversionCallStart = "TypeConversion.StringTo";
	String typeConversionCall;



        for (int i = 0, j=0; i < names.size(); i += 2, j++) {
            String typeName = (String) names.get(i);
            String variable = (String) names.get(i + 1);
	    methodname="set";

	    methodname += Utils.capitalizeFirstChar(variable);
	    if(TypeConversion.IsSimpleType(typeName) == false)
		{
		    pw.println(TUtils.getTabs(numtabs) +"// field " + variable + " (#" +  j + ") is a complex type");
		    pw.println(TUtils.getTabs(numtabs) +"lastConsumed.setValue(lastConsumed.getValue() + 1);");
		    pw.println(TUtils.getTabs(numtabs)+ typeName + " tmp" + j +" = null;");
		    pw.println(TUtils.getTabs(numtabs)+"tmp"+j+ " = " + typeName + ".ObjectFromCSV(params, lastConsumed);");
		    pw.println(TUtils.getTabs(numtabs) +"ni."+methodname + "(tmp" +j+ ");");
		}

	    else
		{
		    String typeConvTypeName = TypeConversion.getNQTypeName(typeName);
		    pw.println(	    TUtils.getTabs(numtabs) +"// field " + variable + " (#" +  j + ") is a simple type");
		    pw.println(	    TUtils.getTabs(numtabs) +"lastConsumed.setValue(lastConsumed.getValue() + 1);");


		    typeConversionCall = typeConversionCallStart + Utils.capitalizeFirstChar(typeConvTypeName) + "( params[lastConsumed.getValue()])";

		    pw.println(TUtils.getTabs(numtabs) +" ni."+methodname + "(" + typeConversionCall + ");");

		}
	}
	pw.println(" return ni; ");
	pw.println(TUtils.getTabs(numtabs) +"}");
    }


    protected void writeToCSV(PrintWriter pw)
    {
	CodeGen.clearTempVars();
	int tmpvaridx=1;
	int numtabs=1;
	pw.println(TUtils.getTabs(numtabs)+"public void ObjectToCSV(int operationid, int parenttableid, int parentcolid, int instanceid, Hashtable tuples)");
	pw.println(TUtils.getTabs(numtabs) +"{");
	numtabs++;
	pw.println(TUtils.getTabs(numtabs) +" String csv = \"\" +  operationid;");
	pw.println(TUtils.getTabs(numtabs) +" String subcsv = \"\";");
	pw.println(TUtils.getTabs(numtabs) +" String stypename=\"" + CodeGen.getStorageTypeName(getClassName())+"\";");
	pw.println(TUtils.getTabs(numtabs) +" Vector thistyperesults = TUtils.getTupleVector(tuples, \""+CodeGen.getStorageTypeName(getClassName())+"\");");
	pw.println(TUtils.getTabs(numtabs) + " System.out.println(\"type vector for type \" + stypename + \" has \" + thistyperesults.size() + \" elements\");");

	pw.println(TUtils.getTabs(numtabs) +" Vector subtyperesults = null;");

	pw.println(TUtils.getTabs(numtabs) +" csv += \",\" + getMyId() ;");
	pw.println(TUtils.getTabs(numtabs) +" if(parenttableid >=0) csv += \",\" + parenttableid;");
	pw.println(TUtils.getTabs(numtabs) +" if(parentcolid >=0) csv += \",\" + parentcolid;");
	pw.println(TUtils.getTabs(numtabs) +" if(instanceid >=0) csv += \",\" + instanceid;");

        for (int i = 0, j=0; i < names.size(); i += 2, j++) {
            String typename = (String) names.get(i);
            String variable = (String) names.get(i + 1);
	    String methodCall = "get";
	    if(typename.equalsIgnoreCase("boolean")) { methodCall = "is"; }
	    methodCall += Utils.capitalizeFirstChar(variable) + "()";
	    if(typename.endsWith("[]"))
		{

		    String arrayname="tmp"+j;
		    String obasename= typename.substring(0, typename.length()-2);
                    String arraydecl = obasename +" " + arrayname + "[]= null;";
		    pw.println(TUtils.getTabs(numtabs) + arraydecl);
		    pw.println(TUtils.getTabs(numtabs) +arrayname + "= get" + Utils.capitalizeFirstChar(variable) +"();");
		    CodeGen.generateArrayTypeToCSV(pw, numtabs, typename, "tuples", 
				       arrayname, "k", "this", j);
		   pw.println(TUtils.getTabs(numtabs) +"csv+=\","+j+"\";");
		}
	    else if(TypeConversion.IsGeneratedType(typename))
		{
		    CodeGen.generateFlattenTypeToCSV(pw, numtabs, typename, "tuples", methodCall, "csv");
		}
	    else
		{
		    CodeGen.generateSimpleTypeToCSV(pw, numtabs, methodCall);

		}
	}
	pw.println(TUtils.getTabs(numtabs) +"thistyperesults.add(csv);");
	numtabs--;
	pw.println(TUtils.getTabs(numtabs) +"}");
   }




}		      

	    

	
		   
	

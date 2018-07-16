import org.apache.axis.Constants;
import org.apache.axis.utils.JavaUtils;
import org.apache.axis.utils.Messages;
import org.apache.axis.wsdl.symbolTable.SymbolTable;
import org.apache.axis.wsdl.symbolTable.ElementDecl;
import org.apache.axis.wsdl.symbolTable.SchemaUtils;
import org.apache.axis.wsdl.symbolTable.BindingEntry;
import org.apache.axis.wsdl.symbolTable.TypeEntry;
import org.apache.axis.wsdl.symbolTable.Parameters;
import org.apache.axis.wsdl.symbolTable.Parameter;
import org.w3c.dom.Node;
import org.apache.axis.wsdl.toJava.Emitter;
import org.apache.axis.wsdl.toJava.JavaWriter;
import org.apache.axis.wsdl.toJava.JavaStubWriter;
import org.apache.axis.wsdl.toJava.Utils;
import org.apache.axis.wsdl.gen.Parser;
import org.telegraphcq.webservices.utils.TypeConversion;
import org.telegraphcq.webservices.utils.TUtils;
import org.telegraphcq.webservices.utils.CodeGen;
import javax.xml.namespace.QName;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Vector;
import javax.wsdl.BindingOperation;
import javax.wsdl.Operation;
import javax.wsdl.Binding;
import javax.wsdl.PortType;

import java.util.Iterator;
import java.util.List;


public class TCQJavaStubWriter extends org.apache.axis.wsdl.toJava.JavaStubWriter
{
    protected SymbolTable TCQsymtab; // alias to private object in superclass
    protected Binding TCQbinding; // alias to private object in superclass
    protected TCQJavaStubWriter(
				Emitter emitter,
				BindingEntry binding,
				SymbolTable symtab) {    
	super(emitter, binding, symtab);
	TCQsymtab = symtab;
	TCQbinding = binding.getBinding();
	    }
    protected void writePackage(PrintWriter pw) throws IOException {
	super.writePackage(pw);

	pw.println("import org.telegraphcq.webservices.utils.TypeConversion;");
	pw.println("import org.telegraphcq.webservices.utils.MutableInt;");
	pw.println("import org.telegraphcq.webservices.utils.TUtils;");
	pw.println("import java.util.Vector;");
	pw.println("import java.util.Hashtable;");

    }

    protected void writeFileBody(PrintWriter pw) throws IOException {
	CodeGen.clearTempVars();
	BindingEntry be = TCQsymtab.getBindingEntry(TCQbinding.getQName());
	PortType pt = TCQbinding.getPortType();
	Operation op = null;
	List operations = pt.getOperations();
	writeMemberFields(pw);
	super.writeFileBody(pw);
	writeMyIdMethods(pw);
        int opno=0;
	for(Iterator i = operations.iterator(); i.hasNext() ; )
	    {
		op = (Operation)i.next();
		pw.println("\n");
		writeTCQInterfaceForOperation(pw,be, op,opno++);
		pw.println("\n");
	    }

    }
    protected void writeMyIdMethods(PrintWriter pw) throws IOException
    {
	CodeGen.clearTempVars();
	int numtabs=1;
	pw.println(TUtils.getTabs(numtabs) + " // methods to get and set id for TCQ ");
	pw.println(TUtils.getTabs(numtabs) + " public int getMyId() {  return " + CodeGen.getGUID() + ";}");

    }

    protected void writeMemberFields(PrintWriter pw) {
	CodeGen.clearTempVars();
	pw.println("public  int mMyId; // added for tcq ");
    };

    protected void writeTCQInterfaceForOperation(PrintWriter pw, BindingEntry be, Operation o, int opno)
    {
	CodeGen.clearTempVars();
	int i;
	int inputno=0;
	int numtabs=1;
	Parameter tmpparam;
	TypeEntry tmptype;
	String tmptypename;
	String actualparams="";
	String methodcall;
	String methodname;
	String tcqoperationname = o.getName()+opno;
	Parameters params = be.getParameters(o);
	Vector callparams = params.list;
	Parameter retparam = params.returnParam; 
	if(params.inouts > 0 )
	    {
		pw.println(getTabs(numtabs) + " // Operation " + o.getName() + " cannot be invoked by TCQ because it has inout parameters");
		return;
	    }

	// note input CSV is assumed to have only input parameters in the same order as params
	// not specified params have "" or null in string array

	pw.println(getTabs(numtabs) + "// TCQ integration code for operation " + o.getName());
	pw.println(getTabs(numtabs) + " public Hashtable TCQInvoke" + Utils.capitalizeFirstChar(tcqoperationname) + "(String csv, Integer id) throws Exception");
	pw.println(getTabs(numtabs) + "{");
	numtabs++;
	pw.println(getTabs(numtabs) + "int operationid = id.intValue();");
	pw.println(getTabs(numtabs) + "Hashtable tuples = new Hashtable();");
	pw.println(getTabs(numtabs) + "MutableInt lastConsumed = new MutableInt(-1);");
	pw.println(getTabs(numtabs) + "Vector retvector=null;");

	pw.println(getTabs(numtabs) + "String inputs[] = csv.split(\",\");");
	pw.println(getTabs(numtabs) + "if(inputs.length != " + params.inputs + ")");
	pw.println(getTabs(numtabs) + "{");
	numtabs++;
	pw.println(getTabs(numtabs) + "throw new Exception(\"TCQInvoke" + Utils.capitalizeFirstChar(tcqoperationname) + "() called with \" + inputs.length + \" parameters, but expected \" + " +params.inputs +");");
	numtabs--;
	pw.println(getTabs(numtabs) + "}");

	for(i=0,inputno=0; i < callparams.size(); i++)
	    {
		tmpparam = (Parameter)callparams.get(i);
		tmptype  = tmpparam.getType();
		tmptypename= tmptype.getName();

		if(tmpparam.getMode() != Parameter.IN)
		    continue;
		if(inputno != 0)
		    {
			actualparams+=",";
		    }

		if(TypeConversion.IsSimpleType(tmptypename))
		    {
			pw.println(getTabs(numtabs) +"lastConsumed.setValue(lastConsumed.getValue() + 1);");
			actualparams += "TypeConversion.StringTo"+Utils.capitalizeFirstChar(TypeConversion.getNQTypeName(tmptypename));
			actualparams += "(inputs["+inputno+"])";
		    }
		else
		    {
		    pw.println(getTabs(numtabs) +"// argument " + i + "  is a complex type");
		    pw.println(getTabs(numtabs) +"lastConsumed.setValue(lastConsumed.getValue() + 1);");
		    pw.println(getTabs(numtabs)+ tmptypename + " tmp" + inputno +" = null;");
		    pw.println(getTabs(numtabs)+"tmp"+inputno+ " = " + tmptypename + ".ObjectFromCSV(inputs, lastConsumed);");
		    actualparams += "tmp" + inputno;
		    }
		inputno++;
	    }
	tmptypename = retparam.getType().getName();
	methodname = o.getName().substring(0,1).toLowerCase() + o.getName().substring(1);
	methodcall = tmptypename + " ret="  + methodname + " ( " + actualparams + ");";
	pw.println(getTabs(numtabs)+ methodcall);
	String resultExpr="";
	String vectortemp=null;
	String stringtmp=null;
	if(TypeConversion.IsGeneratedType(tmptypename)==false)
	    {
	
		if(TypeConversion.IsSimpleArrayType(tmptypename))
		    {
			resultExpr = " TypeConversion.ArrayTypeToString(\""+tmptypename+"\", ret)";
		    }
		else
		    {
			resultExpr = "ret";
		    }

	    }
	else if(tmptypename.endsWith("[]"))
	    {
		CodeGen.generateArrayTypeToCSV(pw,numtabs,tmptypename, "tuples", "ret", "k", "this",i);
		resultExpr = ""+i;
	    }
	else
	    {
		stringtmp =  CodeGen.getTempVar(pw, numtabs, "java.lang.String");
		if(TypeConversion.isEnumType(retparam.getType(), TCQsymtab))
		    {

			CodeGen.generateEnumToCSV(pw,numtabs,"csv","ret");
			resultExpr="\"\"";
		    }
		else
		    {
			CodeGen.generateFlattenTypeToCSV(pw,numtabs,CodeGen.getStorageTypeName(tmptypename),
							 "tuples", "ret", stringtmp);
			resultExpr=stringtmp;
		    }
	    }

	pw.println(getTabs(numtabs) + "retvector = new Vector();");
	pw.println(getTabs(numtabs) + "retvector.add(operationid + \",\" + csv + \",\"+" +  resultExpr +" );");    
	pw.println(getTabs(numtabs) + "tuples.put(\"" + o.getName() + "\", retvector);");
	pw.println(getTabs(numtabs) + "return tuples;");
	if(vectortemp != null) CodeGen.doneTempVar("java.util.Vector", vectortemp);
	if(stringtmp != null)  CodeGen.doneTempVar("java.lang.String", stringtmp);
	numtabs--;
	pw.println(getTabs(numtabs) + "}");

    }

    public String getTabs(int numtabs)
    {
	String tabs="";
	for(int i =0 ; i < numtabs; i++) tabs +="\t";
	return tabs;
    }
}		      

	    

	
		   
	

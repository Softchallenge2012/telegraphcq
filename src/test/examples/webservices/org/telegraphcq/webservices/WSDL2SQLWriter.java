/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001-2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Axis" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
//package samples.integrationGuide.example1;

import org.apache.axis.wsdl.symbolTable.ServiceEntry;
import org.apache.axis.wsdl.symbolTable.SymbolTable;
import org.apache.axis.wsdl.symbolTable.Parameters;
import org.apache.axis.wsdl.symbolTable.BindingEntry;
import org.apache.axis.wsdl.symbolTable.Parameter;
import org.apache.axis.wsdl.symbolTable.TypeEntry;
import org.apache.axis.wsdl.symbolTable.ElementDecl;
import org.apache.axis.wsdl.symbolTable.SchemaUtils;
import org.telegraphcq.TCQSchema.*;
import org.telegraphcq.exceptions.*;
import org.apache.axis.wsdl.toJava.Emitter;
import org.apache.axis.wsdl.toJava.JavaWriter;
import org.apache.axis.wsdl.toJava.Utils;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import org.telegraphcq.webservices.utils.TUtils;
import org.telegraphcq.webservices.utils.TypeConversion;
import org.telegraphcq.webservices.utils.CodeGen;

import javax.wsdl.Port;
import javax.wsdl.Service;
import javax.wsdl.Message;
import javax.wsdl.Operation;
import javax.wsdl.Binding;
import javax.wsdl.PortType;
import javax.wsdl.*;
import java.util.List;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;
import java.util.Hashtable;

/**
* This is my example of a class that writes a list of a service's
* ports to a file named <serviceName>Lst.lst.
*
* Note:  because of a name clash problem, I add the suffix "Lst".
* I hope to remove this in a future version of this example.
*
* Details of the JavaWriter bug:  JavaWriter looks to make sure a
* class doesn't already exist before creating a file, but not all
* files that we generate are .class files!  This works with
* deploy.wsdd and undeploy.wsdd because these files just happen
* to begin with lowercase letters, where Java classes begin with
* uppercase letters.  But this example shows the problem quite
* well.  I would LIKE to call the file <serviceName>.lst, but
* JavaWriter sees that we already have a class called
* <serviceName> and won't let me proceed.
*/
public class WSDL2SQLWriter extends JavaWriter {
    private Service service;
    private SymbolTable symtab;
    private String fileName;
    private Hashtable generatedStreams;
    private PrintWriter mcw;
    private PrintWriter mdw;
    /**
     * Constructor.
     */
    public WSDL2SQLWriter(
            Emitter emitter,
            ServiceEntry sEntry,
            SymbolTable symbolTable) {
        super(emitter, "service list");
        this.service = sEntry.getService();
	this.symtab = symbolTable;

        // Create the fully-qualified file name
        String javaName = sEntry.getName();
        fileName = emitter.getNamespaces().toDir(
                Utils.getJavaPackageName(javaName))
                + "operations";
	try {
	    mcw = new PrintWriter(new FileWriter(fileName+"Create.sql"));
	    mdw = new PrintWriter(new FileWriter(fileName+"Drop.sql"));
	}
	catch (IOException e)
	    {}
	generatedStreams = new Hashtable();
    } // ctor

    protected String getFileName() {
        return fileName;
    } // getFileName

    /**
     * Override the common JavaWriter header to a no-op.
     */
    protected void writeFileHeader(PrintWriter pw) throws IOException {
    } // writeFileHeader

    /**
     * Write the service list file.
     */
    protected void writeFileBody(PrintWriter pw) throws IOException {
        Map portMap = service.getPorts();
        Iterator portIterator = portMap.values().iterator();

        while (portIterator.hasNext()) {
            Port p = (Port) portIterator.next();
            mcw.println("port name:" + p.getName());
	    printBinding(p,pw);
        }
        pw.close(); // Note:  this really should be done in JavaWriter.
    } // writeFileBody

    void printBinding(Port p, PrintWriter pw)
    {
	Iterator gi=null;
	Binding b = p.getBinding();
	BindingEntry bentry = symtab.getBindingEntry(b.getQName());
	if(bentry.getBindingType() != BindingEntry.TYPE_SOAP)
	    {
		mcw.println("-- skiping non-soap binidng " + bentry.getName());
		return;
	    }
	Vector SQLGenerators = new Vector();
	SQLGenerator g=null;
	Operation o = null;
	String schemaName = CodeGen.getSchemaName(bentry);

	PortType pt = b.getPortType();
	List ops = pt.getOperations();
	mcw.println("create schema " + schemaName +";");
	mcw.println("create function webservices_rescan(integer) " +
		    " returns boolean as 'libwebservices.so', " +
		    " 'webservices_rescan' language 'C';");
	int opno=0;
	for(Iterator i = ops.iterator(); i.hasNext();)
	    {
		o = (Operation) i.next();
		
		try {

		    g = OperationToTCQ(p.getName(), o, bentry,opno++);
		    if(g != null)
			{
			    SQLGenerators.add(g);
			    mcw.println("-- generating SQL for operation " + o.getName() + ":");


			}
		    else
			{
			    throw new SQLGeneratorException("");
			}
		}
		catch (SQLGeneratorException e)
		    {
			mcw.println("-- error constructing SQLGenerator for operation " + o.getName() + ":" + e.toString());
		    }
		
	    }


	try
	    {

		mcw.println("-- create stream:");
		for(gi = SQLGenerators.iterator();
		    gi.hasNext();
		    )
		    {
			SQLGenerator gen = (SQLGenerator)gi.next();
			mcw.println(gen.generateCreateStreamStatements());
		    }
		mcw.println("-- create wrapper:");
		
		for(gi = SQLGenerators.iterator();
		    gi.hasNext();
		    )
		    {
			SQLGenerator gen = (SQLGenerator)gi.next();
			mcw.println(gen.generateCreateWrapperStatements(o.getName().toString()));
		    }
		mcw.println("-- alter stream: ");
		
		for(gi = SQLGenerators.iterator();
		    gi.hasNext();
		    )
		    {
			SQLGenerator gen = (SQLGenerator)gi.next();
			mcw.println(gen.generateAlterStreamStatements());
		    }
		

		mdw.println("-- drop wrapper:");
		
		for(gi = SQLGenerators.iterator();
		    gi.hasNext();
		    )
		    {
			SQLGenerator gen = (SQLGenerator)gi.next();
			mdw.println(gen.generateDropWrapperStatements());
			
		    }
	        mdw.println("-- drop stream:");
		for(gi = SQLGenerators.iterator();
		    gi.hasNext();
		    )
		    {
			SQLGenerator gen = (SQLGenerator)gi.next();
			
			mdw.println(gen.generateDropStreamStatements());
		    }
	    }
	catch (SQLGeneratorException e)
	    {
		pw.println("-- error constructing SQLGenerator for operation " + o.getName() + ":" + e.toString());
	    }
	mdw.println("drop schema " + schemaName +";");
	mcw.close();
	mdw.close();
    }
	

    SQLGenerator OperationToTCQ(String portname, Operation o, BindingEntry bentry, int opno) throws SQLGeneratorException
    {

	Vector streamList = new Vector();
	String schemaName =  CodeGen.getSchemaName(bentry);
	String javaPackage = Utils.getJavaPackageName(bentry.getName());
	Stream s = new Stream(schemaName, o.getName()+opno, Stream.ARCHIVED);
	Wrapper w = new Wrapper("webservices_rescan");
	s.addAttribute("package", javaPackage);
	s.addAttribute("service", this.service.getQName().getLocalPart());
	s.addAttribute("wsdlport", portname);
	s.addAttribute("schema", schemaName);
	s.addAttribute("operation", o.getName()+opno);
	w.addAttribute("QUERY","");
	w.makePullWrapper("localhost", "3000");
	w.setInit("csv_init");
	w.setNext("csv_next");
	w.setDone("csv_done");
	w.setRescan("public.webservices_rescan");
	
	s.addWrapper(w);
	s.addColumn("opinstanceid", java.sql.Types.INTEGER, 0,0, false, null);
	Parameters params = bentry.getParameters(o);
	if(params==null) 
	    {
	        mcw.println("-- operation " + o.getName() + " has no parameters ");
		HashMap p = bentry.getParameters();
		mcw.println("-- dumping parameters for binding entry " + bentry.getName());
		printMap(p, mcw);
	    }
	else
	    {
		for(Iterator ii = params.list.iterator();
		    ii.hasNext();
		    )
		    {
			Parameter param = (Parameter) ii.next();
			addColumnToStream(TUtils.xmlNameToSQL(param.getName()), 
					  param.getType(),
					  param.getMode(),
					  s, streamList,
					  s,true, 
					  schemaName);
		    }
		if(params.returnParam != null)
		    {
			addColumnToStream(TUtils.xmlNameToSQL(params.returnParam.getName()),
					  params.returnParam.getType(),
					  params.returnParam.getMode(),
					  s,streamList,
					  s,true, 
					  schemaName);
		    }
		
		streamList.add(s);
		return new SQLGenerator(streamList);
	    }

	    return null;
    }


    
    

    void addColumnToStream(String colName, TypeEntry t, int mode, 
			   Stream s, List streamList, Stream operation,
			   boolean allowSubtypes, String bindingName) throws SQLGeneratorException
    {


	boolean isInput=false;
	int sqltype=-1;
	int scale=0;
	int precision=0;
	int typeinfo[] = null;
	boolean isBaseType = t.isBaseType();
	int typeDimensions = (t.getDimensions().length())/2; /* string is "","[]","[][]",...*/
	if(mode == Parameter.INOUT)
	    {
		throw new SQLGeneratorException("INOUT parameters not supported");
	    }
	else if(mode == Parameter.IN)
	    {
		isInput = true;
		allowSubtypes=false;
	    }
	else if(mode == Parameter.OUT)
	    {
		isInput=false;
	    }
	else
	    {
		throw new SQLGeneratorException("unknown parameter type " + mode);
	    }



	if((typeDimensions > 0  && allowSubtypes == false))
	    {
		throw new SQLGeneratorException("FATAL: array type found within a type that must be flattened");
	    }
	if((typeDimensions > 1 ))
	    {
		throw new SQLGeneratorException("FATAL: array types with dimension > 1 not handled");
	    }

	if(!t.isBaseType())
	    {
		TypeEntry ref = t.getRefType();
		if(ref == null && typeDimensions > 0)
		    {
			throw new SQLGeneratorException(" reftype for type " + t + " is null");
		    }
		
		if(typeDimensions > 0)
		    {
		
			StreamReference instanceColumn = generateNewStreamForType(colName, s, bindingName,  ref,streamList);
		
			s.addColumn(TUtils.xmlNameToSQL(colName), java.sql.Types.INTEGER, 0, 0, isInput, instanceColumn);
		    }
		else
		    {

			Vector typedecls = 
			    SchemaUtils.getContainedElementDeclarations(t.getNode(), 
									symtab);
			if(typedecls == null && t.getRefType() != null)
			    {
				typedecls = SchemaUtils.getContainedElementDeclarations(t.getRefType().getNode(), symtab);
			    }
			if(typedecls != null)
			    {
				for(Iterator i = typedecls.iterator();
				    i.hasNext();
				    )
				    {
					ElementDecl e = (ElementDecl)i.next();
					TypeEntry te = e.getType();
					addColumnToStream(TUtils.xmlNameToSQL(e.getName().toString()), te,mode, 
							  s, streamList, operation, 
							  allowSubtypes, bindingName);
				    }
			    }
			else
			    {
				mcw.println(" -- typedecls for reference type  is null");
			    }
		    }
	    }
	else
	    {
		typeinfo = org.telegraphcq.webservices.utils.SQLUtils.XMLTypeToSQLType(t.getBaseType());
		if(typeinfo == null) 
		    throw new SQLGeneratorException("XMLTypeToSQLType returns null for type: " + t.toString());
		s.addColumn(TUtils.xmlNameToSQL(colName), typeinfo[0], typeinfo[1], typeinfo[2], isInput, null);
	    }
	
    }


StreamReference generateNewStreamForType(String fcolname, 
					 Stream opstream,
					 String schemaname,
					 TypeEntry t, 
					 List streamList) throws SQLGeneratorException
{
    
    Stream s = new Stream(schemaname, TypeConversion.getNQTypeName(t.getName()), Stream.ARCHIVED);
    Wrapper w = new Wrapper("csvwrapper");
    s.addAttribute("dependant_stream", "true");
    s.addWrapper(w);
    Stream alreadyGen = (Stream) generatedStreams.get(s.getStreamFQN());

    StreamReference parentTable = new StreamReference();
    StreamReference operationInstance = new StreamReference();

    if(alreadyGen != null)
	{
	    parentTable.setFromStream(null); // filled in by caller
	    parentTable.setFromColumn(-1);  // out of range
	    parentTable.setToStream(alreadyGen);
	    parentTable.setToColumn(2);
	}
    else
	{
	    operationInstance.setFromStream(opstream);
	    operationInstance.setFromColumn(1);
	    operationInstance.setToStream(s);
	    operationInstance.setToColumn(1);
	    
	    parentTable.setFromStream(null); // filled in by caller
	    parentTable.setFromColumn(-1);  // out of range
	    parentTable.setToStream(s);
	    parentTable.setToColumn(3);
	    s.addColumn("opinstanceid", java.sql.Types.INTEGER,0,0,false, operationInstance);
	    s.addColumn("tableid", java.sql.Types.INTEGER,0,0,false,null);
	    s.addColumn("parenttableid", java.sql.Types.INTEGER,0,0,false,null);
	    s.addColumn("parentcolid", java.sql.Types.INTEGER,0,0,false,null);
	    s.addColumn("instanceid", java.sql.Types.INTEGER,0,0,false,null);
	    addColumnToStream("val", t,Parameter.OUT, 
			      s, streamList, opstream, 
			      true,schemaname);
	    streamList.add(s);
	    generatedStreams.put(s.getStreamFQN(), s);
	}
    return parentTable;
}

    void printMap(Map map, PrintWriter pw)
    {
	Set entryset = map.entrySet();
	for(Iterator i = entryset.iterator(); i.hasNext();)
	    {
		Map.Entry e = (Map.Entry)i.next();
		pw.println(e.getKey()+"="+e.getValue());
	    }
    }
				

				



} // class MyListPortsWriter


import org.apache.axis.wsdl.gen.Generator;
import org.apache.axis.wsdl.toJava.JavaTypeWriter;
import org.apache.axis.wsdl.toJava.JavaWriter;
import org.apache.axis.wsdl.toJava.JavaGeneratorFactory;
import org.apache.axis.wsdl.toJava.JavaBeanFaultWriter;
import org.apache.axis.wsdl.toJava.Emitter;

import org.apache.axis.wsdl.symbolTable.SchemaUtils;
import org.apache.axis.wsdl.symbolTable.SymTabEntry;
import org.apache.axis.wsdl.symbolTable.SymbolTable;
import org.apache.axis.wsdl.symbolTable.TypeEntry;
import org.w3c.dom.Node;

import javax.xml.namespace.QName;
import java.io.IOException;
import java.util.Vector;

class TCQJavaTypeWriter extends JavaTypeWriter
{

    public TCQJavaTypeWriter(
		      Emitter emitter,
		      TypeEntry type,
		      SymbolTable symbolTable) {
	super(emitter, type, symbolTable);
    }

    protected JavaWriter getBeanWriter(Emitter emitter, TypeEntry type,
				       Vector elements, TypeEntry base,
				       Vector attributes) {
	JavaWriter w = super.getBeanWriter(emitter, type, elements, base, attributes);
	if ( w instanceof JavaBeanFaultWriter) return w;

        JavaWriter helperWriter = getBeanHelperWriter(emitter, type, elements, base,
						      attributes);
	

        return new TCQJavaBeanWriter(emitter, type,
                                  elements, base, attributes,
                                  helperWriter);
    }
}

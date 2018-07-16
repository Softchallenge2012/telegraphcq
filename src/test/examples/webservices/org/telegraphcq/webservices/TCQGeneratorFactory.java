import org.apache.axis.wsdl.symbolTable.SymbolTable;
import org.apache.axis.wsdl.symbolTable.TypeEntry;
import org.apache.axis.wsdl.gen.Generator;
import org.apache.axis.wsdl.WSDL2Java;
import org.apache.axis.wsdl.toJava.JavaGeneratorFactory;
import org.apache.axis.wsdl.toJava.Emitter;
import org.apache.axis.wsdl.gen.Parser;
import javax.wsdl.Service;
import javax.wsdl.Binding;


public class TCQGeneratorFactory extends JavaGeneratorFactory
{
    public TCQGeneratorFactory()
    {
	super();
    }
    public TCQGeneratorFactory(Emitter e)
    {
	super(e);
    }


    public Generator getGenerator(TypeEntry type, SymbolTable symbolTable) {
        Generator writer = new TCQJavaTypeWriter(emitter, type, symbolTable);
	return writer;
    } // getGenerator
    public Generator getGenerator(Binding binding, SymbolTable symbolTable) {
        Generator writer = new TCQJavaBindingWriter(emitter, binding, symbolTable);
	return writer;
    } // getGenerator


	

}

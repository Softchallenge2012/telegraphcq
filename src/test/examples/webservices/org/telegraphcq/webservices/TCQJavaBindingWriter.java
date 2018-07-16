import org.apache.axis.wsdl.gen.Generator;
import org.apache.axis.wsdl.toJava.JavaBindingWriter;
import org.apache.axis.wsdl.toJava.Emitter;
import org.apache.axis.wsdl.symbolTable.SymbolTable;
import org.apache.axis.wsdl.symbolTable.BindingEntry;
import javax.wsdl.Binding;



class TCQJavaBindingWriter extends JavaBindingWriter
{

    public TCQJavaBindingWriter(
		      Emitter emitter,
		      Binding binding,
		      SymbolTable symbolTable) {
	super(emitter, binding, symbolTable);
    }


    protected Generator getJavaStubWriter(Emitter emitter,
                                          BindingEntry bEntry,
                                          SymbolTable st) {
        return new TCQJavaStubWriter(emitter, bEntry, st);
    }


}

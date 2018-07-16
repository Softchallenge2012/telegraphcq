
package org.telegraphcq.TCQSchema;
import java.util.Vector;
import org.telegraphcq.exceptions.*;
import java.util.List;
import java.util.Iterator;
import java.util.Hashtable;
public class SQLGenerator
{
    private List mStreams;
    private static Hashtable mGenerated = new Hashtable();
    public SQLGenerator(List streams) { mStreams = streams; }
 
    public String generateCreateStreamStatements() throws SQLGeneratorException
    {
	String sql="";
	Stream stream;
	for(Iterator i = mStreams.iterator();
	    i.hasNext();
	    )
	    {
		stream = (Stream)i.next();
		sql += stream.generateCreateStreamStatement() + "\n";
	    }
	return sql;

    }
    public String generateDropStreamStatements() throws SQLGeneratorException
    {
	String sql="";
	Stream stream;
	for(Iterator i = mStreams.iterator();
	    i.hasNext();
	    )
	    {
		stream = (Stream)i.next();
		sql += stream.generateDropStreamStatement() + "\n";
	    }
	return sql;

    }
    public String generateCreateWrapperStatements(String opname) throws SQLGeneratorException
    {
	Wrapper wrapper=null;
	List wrappers=null;
       	String sql="";
	Stream stream;
	for(Iterator i = mStreams.iterator();
	    i.hasNext();
	    )
	    {
		stream = (Stream)i.next();
		wrappers = stream.getWrappers();
		for(Iterator w = wrappers.iterator(); 
		    w.hasNext(); )
		    {
			wrapper = (Wrapper)w.next();
			if(mGenerated.get("create_"+wrapper.getName()) != null)
			    continue;
			sql += wrapper.generateCreateWrapperStatement();
			mGenerated.put("create_"+wrapper.getName(), new Boolean(true));
		    }

	    }
    
	return sql;
    }


    public String generateDropWrapperStatements() throws SQLGeneratorException
    {

	List wrappers=null;
	Wrapper wrapper=null;
       	String sql="";
	Stream stream;
	for(Iterator i = mStreams.iterator();
	    i.hasNext();
	    )
	    {
		stream = (Stream)i.next();
		wrappers = stream.getWrappers();
		for(Iterator w = wrappers.iterator(); 
		    w.hasNext(); )
		    {
			wrapper = (Wrapper)w.next();
			if(mGenerated.get("drop_"+wrapper.getName()) != null)
			    continue;
			sql += wrapper.generateDropWrapperStatement();
			mGenerated.put("drop_"+wrapper.getName(), new Boolean(true));
		    }

	    }
    
	return sql;
    }


    public String generateAlterStreamStatements() throws SQLGeneratorException
    {
	String sql="";
	Stream stream;
	List wrappers=null;
	Wrapper wrapper=null;
	for(Iterator i = mStreams.iterator();
	    i.hasNext();
	    )
	    {
		stream = (Stream)i.next();
		wrappers = stream.getWrappers();
		for(Iterator w = wrappers.iterator();
		    w.hasNext();)
		    {
			wrapper = (Wrapper)w.next(); 
			sql += stream.generateAlterStreamStatement(Wrapper.ADD,wrapper ) + "\n";
		    }
	    }
	return sql;


    }
    
};

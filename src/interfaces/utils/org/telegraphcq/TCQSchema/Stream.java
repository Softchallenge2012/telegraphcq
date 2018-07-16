
package org.telegraphcq.TCQSchema;
import org.telegraphcq.exceptions.*;
import java.util.List;
import java.util.Vector;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Hashtable;
import java.util.Properties;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class Stream
{
    public static final int UNARCHIVED = 1000;
    public static final int ARCHIVED = 1001;

    protected String mSchemaName;
    protected String mStreamName;
    protected int mStreamType;
    protected int mNumCols;
    protected List  mCols;
    protected Properties mStreamWrapperProps = new Properties();
    protected Vector mWrappers = new Vector();
    
    public Stream(String schemaName, String streamName, int streamType)
    {
	mSchemaName = schemaName;
	mStreamName = streamName;
	mStreamType = streamType;
	mCols = new ArrayList();
	mNumCols = 0;
    }
    public Iterator getColumnIterator() { return mCols.iterator(); }
    public int getNumCols() { return mNumCols; }
    public Column getColumn(int i) {return (Column)mCols.get(i);}
    public String getStreamName() { return mStreamName;}
    public String getSchemaName() { return mSchemaName;}
    public int getStreamType() { return mStreamType; }
    public String getStreamTypeAsString() throws SQLGeneratorException 
    {
	switch (mStreamType)
	{
	case Stream.UNARCHIVED:
	    return "unarchived";
	case Stream.ARCHIVED:
	    return "archived";
	
	}
	    throw new SQLGeneratorException("unknown stream type");
    }

    public String getStreamFQN()
    {
	return getSchemaName() + "." + getStreamName();
    }
    
    public void addColumn(String name, int type, int precision, int scale,
		   boolean isInput, StreamReference r)
    {
	Column c = new Column();
	c.setColNo(++mNumCols);
	c.setColName(name);
	c.setColSQLType(type);
	c.setColScale(scale);
	c.setColPrecision(precision);
	c.setColIsInput(isInput);
	c.setStreamReference(r);
	if(r != null)
	    {
		if(r.getFromStream() == null) 
		    {
			r.setFromStream(this);
			r.setFromColumn(c.getColNo());
		    }
		else if(r.getToStream() == null)
		    {
			r.setToStream(this);
			r.setToColumn(c.getColNo());
		    }
	    }
		
	mCols.add(c);

    }
    /* remove returns true if removed, false otherwise */
    public boolean removeColumn(int colno) { return false; }
    public boolean removeColumn(Column c) {
	Column tmp = null;
	for(Iterator i = this.getColumnIterator();
	    i.hasNext();)
	    {
		tmp = (Column) i.next();
		if(tmp == c) 
		    {
			i.remove();
			return true;
		    }
	    }
	return false;
    }
    
    public String generateCreateStreamStatement() throws SQLGeneratorException
    {
	Column coltmp=null;
	StreamReference reftemp=null;
	String colspec="";
	boolean isFirstCol=true;
	String comments="";
	String comment;
	
	for(Iterator i = getColumnIterator();
	    i.hasNext(); )
	    {
		coltmp = (Column) i.next();
		if(!isFirstCol)
		    {
			colspec +=",";
		    }
		colspec += coltmp.getColName();
		colspec += " ";
		colspec += SQLUtils.SQLTypeToTypename(coltmp.getColSQLType(), coltmp.getColPrecision(), coltmp.getColScale());
		isFirstCol=false;
		reftemp = coltmp.getStreamReference();
		if(reftemp != null)
		    {
			String streamFromFQN;
			String streamToFQN;
			String colFromName;
			String colToName;
			
			streamFromFQN = reftemp.getFromStream().getStreamFQN();
			colFromName = reftemp.getFromStream().getColumn(reftemp.getFromColumn()-1).getColName();
			streamToFQN = reftemp.getToStream().getStreamFQN();
			colToName = reftemp.getToStream().getColumn(reftemp.getToColumn()-1).getColName();
			comment = " -- column " + colFromName + " in stream " + streamFromFQN +
			    " references column " + colToName + " in stream " + streamToFQN;
			comments += comment + "\n";


		    }
	

	    }

	String sql =  "create stream " + getStreamFQN() + "(" + colspec;
	sql += ",tcqtime timestamp TIMESTAMPCOLUMN) type " + getStreamTypeAsString() + ";";
	if(comments.length() > 0)
	    {
		sql += "\n" + comments;
	    }

	return sql;	
	
    }

    public String generateAlterStreamStatement(int alterop, Wrapper w)
    {
	Properties p = new Properties();
	try 
	    {
		String propfile = System.getProperty("WSDL2SQL_defaults", "defaults.props");
		File f = new File(propfile);
		FileInputStream fis = new FileInputStream(f);
		p.load(fis);
	    }
	catch (IOException e)
	    {
		System.out.println("defaults file not found for stream ");
	    }

	Column tmpcol=null;
	String optwithbindings = " with bindings (";
	int numbindings =0;
	String op="";
	switch(alterop)
	    {
	    case Wrapper.ADD:
		op ="add";
		break;
	    case Wrapper.DROP:
	op ="drop";
	break;
	    default:
		return "-- invalid alter op";
	    }	
	String sql = "alter stream " + getStreamFQN() + " " + op + " wrapper " +
	w.getName() ;

	boolean itrisfirst=true;
	for(Iterator i = getColumnIterator();
	    i.hasNext();)
	    {
		tmpcol = (Column) i.next();
		if(tmpcol.getColIsInput()) 
		    {

			if(itrisfirst == false) optwithbindings +=",";
			String key=(getStreamName()+"."+tmpcol.getColName()).toLowerCase();
			System.out.println("key=" + key);
			String def = p.getProperty(key+"_default");
			if(def!=null && def.equals("")) def=" ";
			String optional= p.getProperty(key+"_optional");
			optwithbindings += tmpcol.getColName();
			if(optional != null || def != null) optwithbindings += " OPTIONAL ";
			if(def != null) optwithbindings += " DEFAULT '" + def + "' ";
			numbindings ++;
			itrisfirst = false;
		    }

	    }
	if( numbindings > 0)
	    {
		optwithbindings += " ) " ;
		sql += optwithbindings;
	    }
	String keyvalue = Wrapper.generateKeyValueList(mStreamWrapperProps);
	if(!keyvalue.equals(""))
	    sql += " wrapperinfo '" + keyvalue + "'";
	sql += ";";
	return sql;
    }
    
    public String generateDropStreamStatement()
    {
	return "drop stream " + getStreamFQN() + ";";
    }

    public Vector getWrappers() { return mWrappers; }
    public void addWrapper(Wrapper w) { mWrappers.add(w); }
    public void addAttribute(String key, String value) { mStreamWrapperProps.put(key, value); }
}

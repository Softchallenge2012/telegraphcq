import java.sql.*;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Map;
import java.util.Calendar;

import java.math.BigDecimal;
import java.io.*;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import org.postgresql.Driver;
import org.postgresql.Field;
import org.postgresql.core.Encoding;
import org.postgresql.largeobject.*;
import org.postgresql.util.PGbytea;
import org.postgresql.util.PSQLException;

/*
 *
 */
class CQResultSet implements java.sql.ResultSet
{
    Integer lock = new Integer(0);
    int windowSize = -1;
    int remainingWindow = 0;

    boolean isFetched = false;

    ResultSet rset;
    Connection conn;
    Statement stmt;
    String fetchQuery;
    ResultSetMetaData metaData;

    public CQResultSet(ResultSet rs){
	rset = rs;
	isFetched = true;
    }

    public CQResultSet() {
	
    }
   
    private Enumeration getEnumeration()throws SQLException{
	Vector rows = new Vector();
	Vector cols = null;
	int numCols, numRows, colType;
	String colName, colVal;
	Enumeration colsEnum;
	CQPair pair;

	if (!next()) {
	    return null;
	}

	//synchronized(lock){
	    metaData = rset.getMetaData();
	    numCols = metaData.getColumnCount();
	    cols = new Vector();

	    for(int i = 1; i<= numCols; i++){
		colType = metaData.getColumnType(i);
		colName = metaData.getColumnName(i);
		//colVal = getColValue(colType, colName);
		colVal = rset.getString(colName);
		cols.addElement(new CQPair(colName, colVal));
	    }
	    colsEnum = cols.elements();
	    rows.addElement(colsEnum);
	    //remainingWindow--;
	    //}

	return rows.elements();	
    }


    void sleep(long sleepFor) {
	long startTime = System.currentTimeMillis();
	long endTime = startTime + sleepFor;

	while (endTime >= System.currentTimeMillis()) {
	    // spin;
	}
    }

    /*****************************************************
                 PUBLIC EXTENDED FUNCTIONS
     *****************************************************/

    public boolean next()throws SQLException {
	//return rset.next();
	synchronized (lock) {
	    if (! isFetched) {
		rset = stmt.executeQuery(fetchQuery);
		isFetched = true;
	    }
	    
	    if(windowSize < 0) throw new SQLException("window Size is less than zero");
	    
	    if(rset == null || remainingWindow <= 0){
		rset = stmt.executeQuery(fetchQuery);
		metaData = rset.getMetaData();
		remainingWindow = windowSize;

	    }
	}
	
	if (rset == null) throw new SQLException("query could not be executed");

	remainingWindow --;
	return (rset.next());

	
    }  

    
    /*****************************************************
                  PUBLIC INTERFACE FUNCTIONS
    ******************************************************/
    public Enumeration getNextElements()throws SQLException{
	return getEnumeration();
    }

    public void setStatement(Statement s){
	stmt = s;
    }
    public void setQuery(String q){
	fetchQuery = q;
    }

    public String getQuery() {
	return fetchQuery;
    }

    public void setWindowSize(int w){
	windowSize = w;
	remainingWindow = w;
    }

    public void setConn(Connection c) {
	conn = c;
    }

    public void close(){
	try{
	    /*
	      if (rset != null){
		rset.close();
	    }
	    */
	    if (stmt != null){
		stmt.cancel();
		stmt.close();
	    }
	    if (conn != null)
		conn.close();
	}
	catch(Exception e){}

    }


    /****************************************************
              RESULT SET "IMPLEMENTATION"
     ****************************************************/
    public boolean absolute(int row)throws SQLException{return rset.absolute(row);}
    public void afterLast()throws SQLException{rset.afterLast();}
    public void beforeFirst()throws SQLException{rset.beforeFirst();}
    public void cancelRowUpdates()throws SQLException{rset.cancelRowUpdates();}
    public void clearWarnings()throws SQLException{rset.clearWarnings();}
    public void deleteRow()throws SQLException{rset.deleteRow();}
    public int findColumn(String str)throws SQLException{return rset.findColumn(str);}
    public boolean first()throws SQLException{return rset.first();}
    public Array getArray(int i)throws SQLException{remainingWindow--; return rset.getArray(i);}
    public Array getArray(String str)throws SQLException{remainingWindow--;return rset.getArray(str);}
    public InputStream getAsciiStream(int columnIndex)throws SQLException{remainingWindow--;return rset.getAsciiStream(columnIndex);}
    public InputStream getAsciiStream(String str)throws SQLException{remainingWindow--;return rset.getAsciiStream(str);}
    public BigDecimal getBigDecimal(int i)throws SQLException{remainingWindow--;return rset.getBigDecimal(i);}
    public BigDecimal getBigDecimal(String str)throws SQLException{remainingWindow--;return rset.getBigDecimal(str);}
    public BigDecimal getBigDecimal(String str, int i)throws SQLException{remainingWindow--;return rset.getBigDecimal(str, i);}
    public BigDecimal getBigDecimal(int i1, int i2)throws SQLException{remainingWindow--;return rset.getBigDecimal(i1, i2);}
    public InputStream getBinaryStream(int i)throws SQLException{remainingWindow--;return rset.getBinaryStream(i);}
    public InputStream getBinaryStream(String str)throws SQLException{remainingWindow--;return rset.getBinaryStream(str);}
    public Blob getBlob(int i)throws SQLException{remainingWindow--;return rset.getBlob(i);}
    public Blob getBlob(String str)throws SQLException{remainingWindow--;return rset.getBlob(str);}
    public boolean getBoolean(int i)throws SQLException{remainingWindow--;return rset.getBoolean(i);}
    public boolean getBoolean(String str)throws SQLException{remainingWindow--;return rset.getBoolean(str);}
    public byte getByte(int i)throws SQLException{remainingWindow--;return rset.getByte(i);}
    public byte getByte(String i)throws SQLException{remainingWindow--;return rset.getByte(i);}
    public byte[] getBytes(int i)throws SQLException{remainingWindow--;return rset.getBytes(i);}
    public byte[] getBytes(String i)throws SQLException{remainingWindow--;return rset.getBytes(i);}
    public Reader getCharacterStream(int i)throws SQLException{remainingWindow--;return rset.getCharacterStream(i);}
    public Reader getCharacterStream(String i)throws SQLException{remainingWindow--;return rset.getCharacterStream(i);}
    public Clob getClob(int i)throws SQLException{remainingWindow--;return rset.getClob(i);}
    public Clob getClob(String i)throws SQLException{remainingWindow--;return rset.getClob(i);}
    public int 	getConcurrency()throws SQLException{remainingWindow--;return rset.getConcurrency();}
    public String 	getCursorName()throws SQLException{return rset.getCursorName();}
    public Date 	getDate(int columnIndex)throws SQLException{remainingWindow--;return rset.getDate(columnIndex);}
    public Date 	getDate(int columnIndex, Calendar cal)throws SQLException{remainingWindow--;return rset.getDate(columnIndex, cal);}
    public Date 	getDate(String columnName)throws SQLException{remainingWindow--;return rset.getDate(columnName);}
    public Date 	getDate(String columnName, Calendar cal)throws SQLException{remainingWindow--;return rset.getDate(columnName, cal);}
    public double 	getDouble(int columnIndex)throws SQLException{remainingWindow--;return rset.getDouble(columnIndex);}
    public double 	getDouble(String columnName)throws SQLException{remainingWindow--;return rset.getDouble(columnName);}
    public int  	getFetchDirection()throws SQLException{return rset.getFetchDirection();}
    public int  	getFetchSize()throws SQLException{return rset.getFetchSize();}
    public float 	getFloat(int columnIndex)throws SQLException{remainingWindow--;return rset.getFloat(columnIndex);}
    public float 	getFloat(String columnName)throws SQLException{remainingWindow--;return rset.getFloat(columnName);}
    public int  	getInt(int columnIndex)throws SQLException{remainingWindow--;return rset.getInt(columnIndex);}
    public int  	getInt(String columnName)throws SQLException{remainingWindow--;return rset.getInt(columnName);}
    public long 	getLong(int columnIndex)throws SQLException{remainingWindow--;return rset.getLong(columnIndex);}  
    public long 	getLong(String columnName)throws SQLException{remainingWindow--;return rset.getLong(columnName);}  
    public ResultSetMetaData 	getMetaData()throws SQLException{return rset.getMetaData();}   
    public Object 	getObject(int columnIndex)throws SQLException{remainingWindow--;return rset.getObject(columnIndex);}  
    public Object 	getObject(int i, Map map)throws SQLException{remainingWindow--;return rset.getObject(i, map);} 
    public Object 	getObject(String columnName)throws SQLException{remainingWindow--;return rset.getObject(columnName);} 
    public Object 	getObject(String colName, Map map)throws SQLException{remainingWindow--;return rset.getObject(colName, map);} 
    public Ref 	        getRef(int i)throws SQLException{remainingWindow--;return rset.getRef(i);} 
    public Ref 	        getRef(String colName)throws SQLException{remainingWindow--;return rset.getRef(colName);} 
    public int   	getRow()throws SQLException{remainingWindow--;return rset.getRow();} 
    public short 	getShort(int columnIndex)throws SQLException{remainingWindow--;return rset.getShort(columnIndex);} 
    public short 	getShort(String columnName)throws SQLException{remainingWindow--;return rset.getShort(columnName);} 
    public Statement 	getStatement()throws SQLException{return rset.getStatement();} 
    public String 	getString(int columnIndex)throws SQLException{remainingWindow--;return rset.getString(columnIndex);} 
    public String 	getString(String columnName)throws SQLException{remainingWindow--;return rset.getString(columnName);} 
    public Time 	getTime(int columnIndex)throws SQLException{remainingWindow--;return rset.getTime(columnIndex);} 
    public Time 	getTime(int columnIndex, Calendar cal)throws SQLException{remainingWindow--;return rset.getTime(columnIndex, cal);} 
    public Time 	getTime(String columnName)throws SQLException{remainingWindow--;return rset.getTime(columnName);} 
    public Time 	getTime(String columnName, Calendar cal)throws SQLException{remainingWindow--;return rset.getTime(columnName, cal);} 
    public Timestamp 	getTimestamp(int columnIndex)throws SQLException{remainingWindow--;return rset.getTimestamp(columnIndex);}
    public Timestamp 	getTimestamp(int columnIndex, Calendar cal)throws SQLException{remainingWindow--;return rset.getTimestamp(columnIndex, cal);} 
    public Timestamp 	getTimestamp(String columnName)throws SQLException{remainingWindow--;return rset.getTimestamp(columnName);} 
    public Timestamp 	getTimestamp(String columnName, Calendar cal)throws SQLException{remainingWindow--;return rset.getTimestamp(columnName, cal);}
    public int 	        getType()throws SQLException{remainingWindow--;return rset.getType();} 
    public SQLWarning 	getWarnings()throws SQLException{return rset.getWarnings();} 
    public URL          getURL(int i)throws SQLException{remainingWindow--;return rset.getURL(i);}
    public URL          getURL(String str)throws SQLException{remainingWindow--;return rset.getURL(str);}
    public InputStream  getUnicodeStream(int columnIndex)throws SQLException{remainingWindow--;return rset.getUnicodeStream(columnIndex);}
    public InputStream  getUnicodeStream(String str)throws SQLException{remainingWindow--;return rset.getUnicodeStream(str);}
    public void 	insertRow()throws SQLException{rset.insertRow();} 
    public boolean 	isAfterLast()throws SQLException{return rset.isAfterLast();} 
    public boolean 	isBeforeFirst()throws SQLException{return rset.isBeforeFirst();} 
    public boolean 	isFirst()throws SQLException{return rset.isFirst();} 
    public boolean 	isLast()throws SQLException{return rset.isLast();} 
    public boolean 	last()throws SQLException{return rset.last();} 
    public void 	moveToCurrentRow()throws SQLException{rset.moveToCurrentRow();} 
    public void 	moveToInsertRow()throws SQLException{rset.moveToInsertRow();} 
    //public boolean 	next()throws SQLException{return rset.next();}  
    public boolean 	previous()throws SQLException{return rset.previous();} 
    public void 	refreshRow()throws SQLException{rset.refreshRow();} 
    public boolean 	relative(int rows)throws SQLException{return rset.relative(rows);} 
    public boolean 	rowDeleted()throws SQLException{return rset.rowDeleted();} 
    public boolean 	rowInserted()throws SQLException{return rset.rowInserted();} 
    public boolean 	rowUpdated()throws SQLException{return rset.rowUpdated();} 
    public void 	setFetchDirection(int direction)throws SQLException{rset.setFetchDirection(direction);} 
    public void 	setFetchSize(int rows)throws SQLException{rset.setFetchSize(rows);} 
    public void         updateArray(int i, Array a)throws SQLException{rset.updateArray(i, a);}
    public void         updateArray(String str, Array a)throws SQLException{rset.updateArray(str, a);}
    public void 	updateAsciiStream(int columnIndex, InputStream x, int length)throws SQLException{rset.updateAsciiStream(columnIndex,x,length);} 
    public void 	updateAsciiStream(String columnName, InputStream x, int length)throws SQLException{rset.updateAsciiStream(columnName,x,length);} 
    public void 	updateBigDecimal(int columnIndex, BigDecimal x)throws SQLException{rset.updateBigDecimal(columnIndex, x);} 
    public void 	updateBigDecimal(String columnName, BigDecimal x)throws SQLException{rset.updateBigDecimal(columnName, x);} 
    public void 	updateBinaryStream(int columnIndex, InputStream x, int length)throws SQLException{rset.updateBinaryStream(columnIndex,x,length);}
    public void 	updateBinaryStream(String columnName, InputStream x, int length)throws SQLException{rset.updateBinaryStream(columnName,x,length);}
    public void         updateBlob(int i, Blob b)throws SQLException{rset.updateBlob(i, b);}
    public void         updateBlob(String str, Blob b)throws SQLException{rset.updateBlob(str, b);}
    public void 	updateBoolean(int columnIndex, boolean x)throws SQLException{rset.updateBoolean(columnIndex,x);}
    public void 	updateBoolean(String columnName, boolean x)throws SQLException{rset.updateBoolean(columnName, x);}
    public void 	updateByte(int columnIndex, byte x)throws SQLException{rset.updateByte(columnIndex, x);}
    public void 	updateByte(String columnName, byte x)throws SQLException{rset.updateByte(columnName, x);}
    public void 	updateBytes(int columnIndex, byte[] x)throws SQLException{rset.updateBytes(columnIndex,x);}
    public void 	updateBytes(String columnName, byte[] x)throws SQLException{rset.updateBytes(columnName, x);}
    public void 	updateCharacterStream(int columnIndex, Reader x, int length)throws SQLException{rset.updateCharacterStream(columnIndex,x,length);}
    public void 	updateCharacterStream(String columnName, Reader reader, int length)throws SQLException{rset.updateCharacterStream(columnName, reader,length);}
    public void         updateClob(int i, Clob c)throws SQLException{rset.updateClob(i, c);}
    public void         updateClob(String str, Clob c)throws SQLException{rset.updateClob(str, c);}
    public void 	updateDate(int columnIndex, Date x)throws SQLException{rset.updateDate(columnIndex, x);}
    public void 	updateDate(String columnName, Date x)throws SQLException{rset.updateDate(columnName, x);}
    public void 	updateDouble(int columnIndex, double x)throws SQLException{rset.updateDouble(columnIndex, x);}
    public void 	updateDouble(String columnName, double x)throws SQLException{rset.updateDouble(columnName, x);}
    public void 	updateFloat(int columnIndex, float x)throws SQLException{rset.updateFloat(columnIndex, x);}
    public void 	updateFloat(String columnName, float x)throws SQLException{rset.updateFloat(columnName, x);}
    public void 	updateInt(int columnIndex, int x)throws SQLException{rset.updateInt(columnIndex, x);}
    public void 	updateInt(String columnName, int x)throws SQLException{rset.updateInt(columnName,x);}
    public void 	updateLong(int columnIndex, long x)throws SQLException{rset.updateLong(columnIndex, x);}
    public void 	updateLong(String columnName, long x)throws SQLException{rset.updateLong(columnName, x);}
    public void 	updateNull(int columnIndex)throws SQLException{rset.updateNull(columnIndex);}
    public void 	updateNull(String columnName)throws SQLException{rset.updateNull(columnName);}
    public void 	updateObject(int columnIndex, Object x)throws SQLException{rset.updateObject(columnIndex, x);}
    public void 	updateObject(int columnIndex, Object x, int scale)throws SQLException{rset.updateObject(columnIndex, x, scale);}
    public void 	updateObject(String columnName, Object x)throws SQLException{rset.updateObject(columnName, x);}
    public void 	updateObject(String columnName, Object x, int scale)throws SQLException{rset.updateObject(columnName, x, scale);}
    public void         updateRef(int i, Ref r)throws SQLException{rset.updateRef(i,r);}
    public void         updateRef(String str, Ref r)throws SQLException{rset.updateRef(str, r);}
    public void 	updateRow()throws SQLException{rset.updateRow();}
    public void 	updateShort(int columnIndex, short x)throws SQLException{rset.updateShort(columnIndex, x);}
    public void 	updateShort(String columnName, short x)throws SQLException{rset.updateShort(columnName, x);}
    public void 	updateString(int columnIndex, String x)throws SQLException{rset.updateString(columnIndex, x);}
    public void 	updateString(String columnName, String x)throws SQLException{rset.updateString(columnName, x);}
    public void 	updateTime(int columnIndex, Time x)throws SQLException{rset.updateTime(columnIndex, x);}
    public void 	updateTime(String columnName, Time x)throws SQLException{rset.updateTime(columnName, x);}
    public void 	updateTimestamp(int columnIndex, Timestamp x)throws SQLException{rset.updateTimestamp(columnIndex, x);}
    public void 	updateTimestamp(String columnName, Timestamp x)throws SQLException{rset.updateTimestamp(columnName, x);}
    public boolean 	wasNull()throws SQLException{return rset.wasNull();}
    
}

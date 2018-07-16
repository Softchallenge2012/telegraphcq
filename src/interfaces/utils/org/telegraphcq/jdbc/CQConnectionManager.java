import java.sql.*;
import java.util.*;

public class CQConnectionManager
{
    int windowSize = 1;
    boolean isContQuery = false;
    Integer lock = new Integer(0);

    Connection conn;
    Statement stmt;
    ResultSetMetaData metaData;

    String fetchQuery = "fetch " + windowSize + " from tcqcursor";
    String declareCursorQuery = "declare tcqcursor cursor for ";


    /*
     *
     *
     * 
     */
    public CQConnectionManager(String driver, String dbUrl,
			       String dbUser, String dbPassword,
			       String query, String contQuery)throws SQLException{
	this(driver, dbUrl, dbUser, dbPassword, query, contQuery, 1);
    }


    public CQConnectionManager(String driver, String dbUrl,
			       String dbUser, String dbPassword,
			       String query, String contQuery,
			       int winSize)throws SQLException{
	if (winSize > 0)
	    windowSize = winSize;
	isContQuery = (contQuery != "" && contQuery != "null" && contQuery != null );
	declareCursorQuery += query;
	if(!isContQuery)
	    fetchQuery = query;
	
	try{
	    Class.forName(driver);
	    conn = DriverManager.getConnection(dbUrl, dbUser, dbPassword);
	    stmt = conn.createStatement();
	}catch(Exception e){
	    close();
	    throw new SQLException("\n" + e + "\n\nerror setting up connection\n");
	}
	try{
	    start();
	}
	catch(Exception e){
	    //stmt.executeUpdate("ABORT");
	    close();
	    throw new SQLException("\nerror trying to run start() on:  " + declareCursorQuery + "\n" + e + "\n");
	}

    }


    private Enumeration executeSelectQuery() throws SQLException{
	Vector rows = new Vector();
	Vector cols = null;
	int numCols, numRows, colType;
	String colName, colVal;
	Enumeration colsEnum;

	ResultSet rset;
	CQPair pair;

	synchronized(lock){
	    rset = stmt.executeQuery(fetchQuery);
	    metaData = rset.getMetaData();
	    numCols = metaData.getColumnCount();
	    while(rset.next()){
		cols = new Vector();
		for(int i = 1; i <= numCols; i++){
		    colType = metaData.getColumnType(i);
		    colName = metaData.getColumnName(i);
		    //colVal = getColValue(colType, colName);
		    colVal = rset.getString(colName);
		    cols.addElement(new CQPair(colName, colVal)); 
		}
		colsEnum = cols.elements();
		rows.addElement(colsEnum);
	    }
	}
	return rows.elements();
	
    }


    private Enumeration executeInsertQuery() throws SQLException{
	stmt.executeUpdate(fetchQuery);
	stmt.executeUpdate("COMMIT");
	return new Vector().elements();
    }
	

    private void start()throws SQLException{
	if(isContQuery){
	    stmt.executeUpdate("BEGIN");
	    stmt.executeUpdate(declareCursorQuery);
	}
	
    }


    /*****************************************************
                    PUBLIC FUNCTIONS
    ******************************************************/
    
    /*
     *  Description: one time function that gets an enumeration of the
     *               results of the query
     *
     *  NOTE: ONLY CALLED IF IT IS A NON-CONTINUOUS QUERY
     */
    public Enumeration executeQuery() throws SQLException{
	String lowerCaseQuery = new String(fetchQuery).toLowerCase();
	int insertIndex = lowerCaseQuery.indexOf("insert");
	Enumeration enum = null;
	if(insertIndex != -1)
	    enum = executeInsertQuery();
	else
	    enum = executeSelectQuery();
	close();
	return enum;
    }


    public CQResultSet getResultSet()throws SQLException{
	if(!isContQuery)return null;
	CQResultSet rset = new CQResultSet();
	rset.setStatement(stmt);
	rset.setQuery(fetchQuery);
	rset.setWindowSize(windowSize);
	rset.setConn(conn);
	return rset;
    }

    public boolean close(){
	try{
	    stmt.close();
	    conn.close();
	}catch(Exception e){
	    return false;
	}
	return true;
    }

    public boolean isContQuery(){
	return isContQuery;
    }
}

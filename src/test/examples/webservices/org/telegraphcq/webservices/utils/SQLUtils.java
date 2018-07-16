
package org.telegraphcq.webservices.utils;
import org.telegraphcq.exceptions.*;
public class SQLUtils
{
    /*returns java.sql.type, precision, scale  or null if complex */
    public static int[] XMLTypeToSQLType(String tn) 
	throws UnknownTypeException
    {
	if(tn.equals("null")) return null;
	int [] returntype = new int[3];
	
	if(tn.equalsIgnoreCase("java.util.Calendar"))
	    {
		returntype[0] = java.sql.Types.TIMESTAMP;
	    }
	else if(tn.equalsIgnoreCase("java.lang.String"))
	    {
		returntype[0] = java.sql.Types.VARCHAR;
		returntype[1] = 250;
		returntype[2] = 0;
	    }
	else if(tn.equalsIgnoreCase("int") || tn.equalsIgnoreCase("long"))
	    {
		returntype[0] = java.sql.Types.INTEGER;
		returntype[1]=0;
		returntype[2]=0;
	    }
	else if(tn.equalsIgnoreCase("byte[]"))
	    {
		returntype[0] = java.sql.Types.LONGVARBINARY;
		returntype[1] = 4096;
		returntype[2] = 0;
	    }
	else if(tn.equalsIgnoreCase("boolean") || tn.equalsIgnoreCase("bit"))
	    {
		returntype[0] = java.sql.Types.BOOLEAN;
	    }
	else if(tn.equalsIgnoreCase("float"))
	    {
		returntype[0] = java.sql.Types.REAL;
	    }
	else if(tn.equalsIgnoreCase("double"))
	    {
		returntype[0] = java.sql.Types.FLOAT;
	    }
	else
	    {
		throw new UnknownTypeException(tn);
	    }
	return returntype;
    }

    public static String SQLTypeToTypename(int sqltype, int precision, int scale) 
	throws UnknownTypeException
    {
	String type;
	boolean needss=false;
	boolean needsp=false;

	switch(sqltype)
	    {
	    case java.sql.Types.DATE:
		type="date";
		break;
	    case java.sql.Types.TIME:
		type="time";
		break;
	    case java.sql.Types.TIMESTAMP:
		type="datetime";
		break;
	    case java.sql.Types.CHAR:
		type="char";
		needsp=true;
		needss=false;
		break;
	    case java.sql.Types.VARCHAR:
	    case java.sql.Types.LONGVARCHAR:
		type="varchar";
		needsp=true;
		needss=false;
		break;
	    case java.sql.Types.REAL:
		type =  "real";
		break;
	    case java.sql.Types.FLOAT:
		type = "float";
		break;
	    case java.sql.Types.NUMERIC:
		type = "numeric";
		needss=true;
		needsp=true;
		break;
	    case java.sql.Types.INTEGER:
		type = "int";
		break;
	    case java.sql.Types.BIT:
	    case java.sql.Types.BOOLEAN:
		type= "boolean";
		break;
	    case java.sql.Types.LONGVARBINARY:
	    case java.sql.Types.VARBINARY:
		type="longvarbinary";
		break;
	    default:
		throw new UnknownTypeException(""+sqltype);
	    };
	if(needss || needsp) type+="(";
	if(needsp) type += precision;
	if(needss) type += ","+scale;
	if(needss || needsp) type+=")";
	return type;
    }
}

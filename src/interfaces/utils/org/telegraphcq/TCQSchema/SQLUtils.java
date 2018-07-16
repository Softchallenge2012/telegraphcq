
package org.telegraphcq.TCQSchema;
import org.telegraphcq.exceptions.*;
public class SQLUtils
{


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

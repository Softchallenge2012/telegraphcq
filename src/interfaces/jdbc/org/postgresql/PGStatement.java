package org.postgresql;


import java.sql.*;

/* $Header: /project/eecs/db/cvsroot/postgres/src/interfaces/jdbc/org/postgresql/PGStatement.java,v 1.2 2004/03/24 08:11:31 chungwu Exp $
 * This interface defines PostgreSQL extentions to the java.sql.Statement interface.
 * Any java.sql.Statement object returned by the driver will also implement this
 * interface
 */
public interface PGStatement
{

	/*
	 * Returns the Last inserted/updated oid.
	 * @return OID of last insert
			* @since 7.3
	 */
	public long getLastOID() throws SQLException;

	public void setUseServerPrepare(boolean flag) throws SQLException;

	public boolean isUseServerPrepare();

}

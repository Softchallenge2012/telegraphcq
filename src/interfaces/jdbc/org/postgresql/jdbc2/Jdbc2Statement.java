package org.postgresql.jdbc2;


import java.sql.*;

/* $Header: /project/eecs/db/cvsroot/postgres/src/interfaces/jdbc/org/postgresql/jdbc2/Jdbc2Statement.java,v 1.3 2004/03/24 08:11:32 chungwu Exp $
 * This class implements the java.sql.Statement interface for JDBC2.
 * However most of the implementation is really done in
 * org.postgresql.jdbc2.AbstractJdbc2Statement or one of it's parents
 */
public class Jdbc2Statement extends org.postgresql.jdbc2.AbstractJdbc2Statement implements java.sql.Statement
{

	public Jdbc2Statement (Jdbc2Connection c)
	{
		super(c);
	}

   public void initDefaults()
   {
      super.initDefaults();
      m_useServerPrepare=((AbstractJdbc2Connection) connection).useServerPrepareAlways;
   }

}

package org.postgresql.jdbc1;


import java.sql.*;

/* $Header: /project/eecs/db/cvsroot/postgres/src/interfaces/jdbc/org/postgresql/jdbc1/Jdbc1Statement.java,v 1.3 2004/03/24 08:11:32 chungwu Exp $
 * This class implements the java.sql.Statement interface for JDBC1.
 * However most of the implementation is really done in
 * org.postgresql.jdbc1.AbstractJdbc1Statement
 */
public class Jdbc1Statement extends org.postgresql.jdbc1.AbstractJdbc1Statement implements java.sql.Statement
{

	public Jdbc1Statement (Jdbc1Connection c)
	{
		super(c);
	}

   public void initDefaults()
   {
      super.initDefaults();
      m_useServerPrepare=((AbstractJdbc1Connection) connection).useServerPrepareAlways;
   }

}

package org.postgresql.jdbc1;


import java.sql.*;

public class Jdbc1PreparedStatement extends AbstractJdbc1Statement implements PreparedStatement
{

	public Jdbc1PreparedStatement(Jdbc1Connection connection, String sql) throws SQLException
	{
		super(connection, sql);
	}

   public void initDefaults()
   {
      super.initDefaults();
      m_useServerPrepare=!((AbstractJdbc1Connection) connection).useServerPrepareNever;
   }

}

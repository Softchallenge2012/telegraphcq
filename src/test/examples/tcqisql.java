import java.sql.*;
import java.io.*;
class tcqisql 
{
    static
    {
	try 
	    {
		Class.forName("org.postgresql.Driver");
	    }
	catch (Throwable t)
	    {
		t.printStackTrace();
		System.exit(1);
	    }
    }

    public static void main(String [] args) throws Exception
    {
	if(args.length != 3)
	    { 
			System.out.println("usage: db user passwd ");
		System.exit(1);
	    }

	String URL = "jdbc:postgresql:" + args[0];
	Connection c = DriverManager.getConnection(URL, args[1], args[2]);
	System.out.println("got a connection");
	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	while(true)
		{
			boolean cont=true;
			String query = br.readLine();
			System.out.println("got a query: " + query);
			if(query.equalsIgnoreCase("quit")) break;
			Statement s= c.createStatement();
		   
			s.executeUpdate("BEGIN");
			s.executeUpdate("declare tcqcursor cursor for " + query);
			while(cont)
				{
					ResultSet r = s.executeQuery("fetch 1 from tcqcursor");
					cont=printResults(r);
				}
			s.executeUpdate("close tcqcursor");
			s.executeUpdate("commit");
			s.close();
		}
	c.close();
	}

	static boolean printResults(ResultSet r) throws Exception
	{
		boolean ret=false;
		String s;
		int numcols=r.getMetaData().getColumnCount();
		while(r.next())
			{
				ret = true;
				s="";
				for(int i=1; i <= numcols; i++)
					{
						if(i != 1) s+=",";
						s+=r.getString(i);
					}
				System.out.println(s);
			}
	return ret;
	}
}


		


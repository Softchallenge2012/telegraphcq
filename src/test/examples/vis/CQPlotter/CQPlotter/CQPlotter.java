package CQPlotter;

import javax.swing.*;
import java.awt.*;
import java.sql.*;

import java.io.*;

public class CQPlotter extends Thread {
    public int WINDOW_SIZE;
    
    Connection con = null;
    Statement s = null;
    ResultSet rset = null;
    Integer lock = new Integer(0);
    
    BarChart plot;
    JFrame frame;

    boolean ready_to_exit = false;

    String host, user, password, query, xfield, yfield;
    public CQPlotter(String title, String xlabel, String ylabel, 
		     String query, String xfield, String yfield,
		     String host, String user, String password,
		     int expirationTimeMs, int historySize, int windowSize) {
	plot = new BarChart();
	frame = new JFrame();
	plot.clear(true);
	
	plot.setSize(400,400);
	frame.setContentPane(plot);
	frame.setSize(400,400);
	frame.setVisible(true);
	plot.setXLabel(xlabel);
	plot.setYLabel(ylabel);
	plot.setTitle(title);
	plot.historySize = historySize;
	plot.setExpiration(expirationTimeMs);
	
	this.host = host;
	this.user = user;
	this.password = password;
	this.xfield = xfield;
	this.yfield = yfield;
	this.query = query;
	WINDOW_SIZE = windowSize;

    }

    public void run() {
	addData(host, user,password, query, xfield, yfield);
	exit();
 }

    public void exit() {
	final CQPlotter plotter=this;
	Runnable r = new Runnable() {
		CQPlotter p = plotter;
		public void run()
		{
		    System.err.println("Thread " + Thread.currentThread().getName() + " created to kill CQPlotter for query" + p.query);
		    ready_to_exit = true;
		    p.interrupt();
		    p.cleanup();
		    System.err.println("Thread " + Thread.currentThread().getName() + " finished cleanup ");
		}
	    };
	    new Thread(r).start();
    }

    public void addData(String dbUrl, String dbUser, String dbPassword, String query,
			String xfield, String yfield) {
	try {
	    con = DriverManager.getConnection(dbUrl, dbUser, dbPassword);
	    String fetch = "fetch " + WINDOW_SIZE + " from tcqcursor";
	    s = con.createStatement();
	    
	    while (! ready_to_exit) {
            if (con == null) break;
            s.executeUpdate("BEGIN");
            s.executeUpdate("declare tcqcursor cursor for " + query);
            rset = s.executeQuery(fetch);
            int i = WINDOW_SIZE;
            System.out.println("RUNNING");
            
            while (true) {
                String x;
                int y;
                
                
                synchronized(lock) {
                    if(ready_to_exit) break;
                    if (con == null) break;
                    if (!rset.next()) break;
                    x = rset.getString(xfield);
                    y = rset.getInt(yfield);
                }
                //System.out.println("GOT " + xfield + " = " + x +  " , " + yfield + "  = " + y);
                if (x != null) 
                    plot.addPoint(0,x,y);
                
                if (--i == 0) {
                    synchronized(lock) {
                        if (con == null || ready_to_exit) break;
                        rset = s.executeQuery(fetch);
                    }
                    i = WINDOW_SIZE;
                }
                
            }
	    }
	} catch (Exception e) {
	    e.printStackTrace();
	}
	
    }

    public void cleanup() {
	System.err.println("in cleanup... about to get lock . for thread" + Thread.currentThread().getName());
	synchronized(lock) {
	    System.err.println("in cleanup... got lock. for thread" + Thread.currentThread().getName());
	    if (con != null) {
		try {
		    
		    if (rset != null) 
			{
			    System.err.println("in cleanup... result set is not null. for thread" + Thread.currentThread().getName());
			    rset.close();
			}
		    if (s != null) 
			{
			    System.err.println("in cleanup... statement  is not null.... sending cancel.  for thread" + Thread.currentThread().getName());
			    s.cancel();
			    System.err.println("in cleanup... statement  is not null.... cancel returned.  for thread" + Thread.currentThread().getName());
			}
		    con.close();
		    con = null;
		    
		} catch (SQLException e) {
		    System.err.println("in cleanup...  got exception  for thread" + Thread.currentThread().getName() + " exception is " + e.getMessage());
		}
	    }
	}
	if (plot != null) {
	    plot.stop();
	}
    }
    
    public static void main(String argv[]) {
	/*
	try {
	    Class.forName("org.postgresql.Driver");
	} catch(ClassNotFoundException cnfe) {
	    System.out.println(cnfe);
	    cnfe.printStackTrace();
	}	    
	*/
	/*
	CQPlotter test = new CQPlotter(cf.getParam("Title"),
				       cf.getParam("X-axis"),
				       cf.getParam("Y-axis"),
				       cf.getParam("Query"),
				       cf.getParam("X-field"),
				       cf.getParam("Y-field"),
				       cf.getParam("Host"),
				       cf.getParam("User"),
				       cf.getParam("Password"),
				       Integer.valueOf(cf.getParam("ExpirationTime")).intValue(),
				       Integer.valueOf(cf.getParam("HistorySize")).intValue());
	*/
	/*
	CQPlotter test = new CQPlotter("Dest IP vs Traffic", "Dest IP", "Packets", 
				       "select dst, count(*) as c from network.tcpdump as st group by dst window st ['5 seconds']",
				       "dst", "c", "jdbc:postgresql://datafix.cs.berkeley.edu:8765/sample", "tcq", "", 10000, 10);
	
	*/	
    }

}

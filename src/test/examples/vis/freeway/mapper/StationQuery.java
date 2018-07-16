package mapper;

import javax.swing.*;
import javax.swing.event.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.awt.image.*;

import java.net.*;
import java.io.*;
import java.util.*;
import java.util.zip.*;
import java.math.*;
import java.text.DateFormat;

import edu.gui.*;
import edu.gui.view.QuerySelect;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;


/*
 * class to query a station for its current speed
 */
public class StationQuery implements Runnable {
    ModelData mdata;
    Mapper map;
    Vector pointVector;
    Model model;
    boolean stop = false;
    
    Statement s;
    Connection con = null;
    public StationQuery(Model m, Mapper map) {
	this.mdata = m.getModelData();
	pointVector = map.pointVector;
	this.map = map;
	model = m;

    }  
    public void run() {
	Hashtable stations = new Hashtable();
	StationPoint p;

	Enumeration e = pointVector.elements();
	Date start = mdata.getStartDate(), end = mdata.getEndDate();
	//System.out.println("IN STATIONQUERY");
	/*
	while (e.hasMoreElements() && !stop) {
	    Object o = e.nextElement();
	    if (o instanceof StationPoint) {
		StationPoint sp = (StationPoint)o;
		
		stations.put(new Integer(sp.stationId), sp);
	    }
	}
	*/

	double val;

	
	try {
	    con = model.getConnection();
	    s = con.createStatement();

	    String query = QuerySelect.query1; 
	    System.out.println("QUERY: " + query);

	    s.executeUpdate("BEGIN");
	    System.out.println("cursor for query:" + query);
	    s.executeUpdate("declare tcqcursor cursor for " + query);
	    
	    ResultSet rset = s.executeQuery("fetch " + Mapper.fetchtuplenumstations + " from tcqcursor");
	    System.out.println("After rset");
	    int i = Mapper.fetchtuplenumstations; //20;
	    System.out.println("Here is i: " + i);

	    while (rset.next()) {

		Color col;
		String sid = rset.getString("stationid");
		val = rset.getDouble("speed");
		System.out.println ("stationid = " + sid +" speed = " + val);
		
		// do colors.
		if (val > 50) col = Color.green;
		else if (val > 30) col = Color.yellow;
		else if (val > 0) col = Color.red;
		else col = new Color(255,255,255,0); //Color.blue;  //no data

		float lat = rset.getFloat("latitude");
		float lon = rset.getFloat("longitude");

		p = (StationPoint)stations.get(new Integer(rset.getInt("stationid")));
		if (p != null) {
		    map.changePoint(p.name, null, "Station " + p.stationId + " (" + (int)val + " mph)", col , null, null);
		} else {
		    map.addStationPoint("Station"+sid, null, "Station " + sid + " (" + (int)val + " mph)", lon, lat, col, 4,8);
		}
		i--;
		if (i==0) { 
		    rset = s.executeQuery("fetch " + Mapper.fetchtuplenumstations+ " from tcqcursor");
		    i = Mapper.fetchtuplenumstations; //fetch the next 20
		}
	    }
	    s.executeUpdate("close tcqcursor");
	    s.executeUpdate("commit");
	    s.close();
	    System.out.println("DONE WITH QUERY");

	    
	} catch (SQLException se) {
	    se.printStackTrace();
	} finally {
	    if (con != null) {
		try {
		    con.close();
		} catch (SQLException se) {
		    se.printStackTrace();
		}
	    }
	}		   
    }
    
    public void stop() {
	stop = true;
	Main.cleanup(con, s);
	con=null;
	s=null;
    }
}

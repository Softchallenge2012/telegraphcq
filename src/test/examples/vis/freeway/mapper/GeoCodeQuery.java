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
 * 
 * class to query a geocode for its data.
 */
public class GeoCodeQuery implements Runnable {

ModelData mdata;
    Mapper map;
    Vector pointVector;
    Model model;
    boolean stop = false;

  public static final int ncolors = 20;
  public static final int srgbspace = 255;

  Color cols[];

    Statement s=null;
    Connection con = null;
    public GeoCodeQuery(ModelData data, Model m, Vector points, Mapper map) {
	this.mdata = data;
	pointVector = points;
	this.map = map;
	model = m;
	//currentIncidents = new Vector();

	Thread cIThread = new Thread();
    }

  private Color getColor(int num)
  {
    int srgb;
    Color col;

    srgb = (5*num);

    if (srgb > 255) 
    {
      srgb = 255;
    }
  
    col = Mapper.palette[srgb];
    
    System.out.println("num: " + num + " color: " + col);
    return col;
  }
  
    public void run() {
	
	Hashtable incidents = new Hashtable();
	IncidentPoint p;

	Enumeration e = pointVector.elements();
	Date start = mdata.getStartDate(), end = mdata.getEndDate();
	//System.out.println("IN STATIONQUERY");
	/*
	while (e.hasMoreElements() && !stop) {
	    Object o = e.nextElement();
	    if (o instanceof IncidentPoint) {
		IncidentPoint sp = (IncidentPoint)o;
		
		incidents.put(new Integer(sp.incidentID), sp);
	    }
	}
	*/


	try {
	    con = model.getConnection();
	    s = con.createStatement();

	    String query = QuerySelect.query3;
	    System.out.println("QUERY: " + query);

	    s.executeUpdate("BEGIN");
	    System.out.println("cursor for query:" + query);
	    s.executeUpdate("declare tcqcursor cursor for " + query);
	    
	    ResultSet rset = s.executeQuery("fetch " + Mapper.fetchtuplenumgeocode + " from tcqcursor");
	    System.out.println("After rset");
	    int i = Mapper.fetchtuplenumgeocode; 
	    System.out.println("Here is i: " + i);

	    Image image;
	    while (rset.next()) {
		
		if (stop) {
		    break;
		}

		float latitude = rset.getFloat("latitude");
		float longitude = rset.getFloat("longitude");
		int connections = rset.getInt("connections");
		Color col;
		// do colors.

		
// 		if (connections > 30) col = Color.green;
// 		else if (connections > 20) col = Color.yellow;
// 		else if (connections > 0) col = Color.red;
// 		else col = new Color(255,255,255,0); //Color.blue;  //no data

		System.out.println("connections: " + connections);
		col = getColor(connections);

		String name = "GeoCode"+latitude+","+longitude;

		System.out.println("GEOCODE: name="+name+" lat="+latitude+" lon="+longitude+" count:"+connections);

		String descr = name + " connects:" + connections;

		map.addGeoCodePoint(name, descr, longitude, 
				    latitude, col, 4,4);
		//map.changePoint(name, Mapper.accidentImage, "Incident " + incidentID + " (location:" + location +", descr:"+ descr+")", null , new Integer(1000), new Integer(1000));
		
		i--;
		if (i==0) { 
		    rset = s.executeQuery("fetch " + Mapper.fetchtuplenumgeocode + " from tcqcursor");
		    i = Mapper.fetchtuplenumgeocode; //fetch the next 20
		}
	    }
	    s.executeUpdate("close tcqcursor");
	    s.executeUpdate("commit");
	    s.close();
	    System.out.println("DONE WITH QUERY");

	    
	} catch (SQLException se) {
	    se.printStackTrace();
	} catch (NoSuchElementException se) {
	    System.err.println(">>> No Such Element Exception in Incident Query.");
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
	Main.cleanup(con,s);
	con=null;
	s=null;
	
    }
}





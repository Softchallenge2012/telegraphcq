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
 * class to query an incident for its data.
 */
public class IncidentQuery implements Runnable {

    ModelData mdata;
    Mapper map;
    Vector pointVector;
    Model model;
    boolean stop = false;
    
    Statement s=null;
    Connection con = null;
    public IncidentQuery(Model m, Mapper map) {
	this.mdata = m.getModelData();
	pointVector = map.pointVector; 
	this.map = map;
	model = m;
	//currentIncidents = new Vector();

	Thread cIThread = new Thread();
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

	    String query = QuerySelect.query2;
	    System.out.println("QUERY: " + query);

	    s.executeUpdate("BEGIN");
	    System.out.println("cursor for query:" + query);
	    s.executeUpdate("declare tcqcursor cursor for " + query);
	    
	    ResultSet rset = s.executeQuery("fetch " + Mapper.fetchtuplenumincidents + " from tcqcursor");
	    System.out.println("After rset");
	    int i = Mapper.fetchtuplenumincidents; 
	    System.out.println("Here is i: " + i);

	    Image image;
	    while (rset.next()) {
		
		if (stop) {
		    break;
		}

		int incidentID = rset.getInt("incidentID");
		Timestamp tcqtime = rset.getTimestamp("tcqtime");
		String location = rset.getString("location");
		String city = rset.getString("city");
		String descr = rset.getString("description");;
		descr = descr + " (location:"+location+")";
		String hwy = rset.getString("highway");
		String street = rset.getString("street");
		float latitude = rset.getFloat("latitude");
		float longitude = rset.getFloat("longitude");
		
		String name = "Incident"+incidentID;
		
		System.out.println("INC: "+name+" street="+street+" city="+city+"descr="+descr+" lat="+latitude+" lon="+longitude);

		//map.addIcon(name, Mapper.accidentImage, descr, longitude, latitude);
		map.addIncidentPoint(name, Mapper.accidentImage, descr, longitude, 
			     latitude, tcqtime, location, city, hwy, street, 50, 50 );
		//map.changePoint(name, Mapper.accidentImage, "Incident " + incidentID + " (location:" + location +", descr:"+ descr+")", null , new Integer(1000), new Integer(1000));
		
		i--;
		if (i==0) { 
		    rset = s.executeQuery("fetch " + Mapper.fetchtuplenumincidents + " from tcqcursor");
		    i = Mapper.fetchtuplenumincidents; //fetch the next 20
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
	    Main.cleanup(con, s);
	}
}




/**
 * Thread for cleaning up incidents.
 *
 */
/*
class CleanUpIncident implements Runnable {
    
    Vector currentIncidents;
    Vector toAdd;
    Vector toRemove;
    
    public CleanUpIncident () {
	currentIncidents = new Vector();
	toAdd = new Vector();
	toRemove = new Vector();
    }
    
    public void run() {
	while (true) {
	    
	    try {
		//Thread.sleep(3600); // sleep for 1 min.
		//Thread.sleep(3600); // sleep for 1 more min.
	    } catch (Exception e) {
	    }
	    
	    // identify those to remove
	    int size = currentIncidents.size();
	    for (int i = 0; i < size; i += 1) {
		IncidentPoint p = new IncidentPoint();
		if (p.count == 1) {
		    map.removePoint(p.name);
		} 
	    }
	    
	    
	    
	    // delete remove from currentIncidents
	    //deleteRemove();
	    
	    // add to toAdd
	    
	}
    }
    
    
}


*/

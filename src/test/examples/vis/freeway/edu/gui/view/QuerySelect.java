package edu.gui.view;

import edu.gui.*;
import ptolemy.plot.*;
import mapper.*;

import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;


/**
 *
 * @author emach
 * @version 
 *
 * Allows the user to select which query to view.
 */
public class QuerySelect extends JPanel implements ItemListener {

  Mapper map;
  Model model;

  IncidentQuery incq;
  StationQuery sq = null;
  StationIncidentQuery siq = null;
  GeoCodeQuery gcq;

  QueryView qv;

  public static String query1 =
    " SELECT ts.stationid, tm.speed, ts.latitude,ts.longitude "+
    " FROM   traffic.measurements AS tm, traffic.stations AS ts "+
    " WHERE  tm.stationid = ts.stationid ";
  private JCheckBox button1 = new JCheckBox("S-T Join (Measurements/Stations)");

  public static String query2 =
    " SELECT tcqtime, incidentID, location, city, description, tinc.highway, tinc.street, latitude, longitude "+
    " FROM traffic.incidents AS tinc, traffic.intersections AS tint "+
    " WHERE (tinc.highway||tinc.street) = (tint.highway||tint.street) ";
  private JCheckBox button2 = new JCheckBox("S-T Join (Incidents/Intersections)");

  public static String query3 =
    " SELECT gs.latitude, gs.longitude, count(*) as connections "+
    " FROM   network.geoserver gs, network.tcpdump td "+
    " WHERE gs.addr = td.dst "+
    " GROUP BY gs.latitude, gs.longitude WINDOW td ['5 seconds'];";
  private JCheckBox button3 = new JCheckBox("S-T Join (TCP Dump/GeoCoding)");

  public static String query4 =
    "select ts.stationid, ts.speed, ts.tcqtime as stcqtime, tinc.incidentID, tinc.location,"+
    "       tinc.city, tinc.description,tinc.highway,tinc.street,tinc.tcqtime as itcqtime,"+
    "       tint.latitude, tint.longitude " +
    "from   traffic.incidents     as tinc, "+
    "	traffic.measurements  as tm, "+
    "	traffic.intersections as tint, "+
    "	traffic.stations      as ts "+
    "where  (tinc.highway||tinc.street) = (tint.highway||tint.street) and "+
    "	tm.stationid = ts.stationid                               and "+
    "	(round(cast(ts.latitude as numeric),1)||round(cast(ts.longitude as numeric),1))  "+
    "			 = "+
    "	(round(cast(tint.latitude as numeric),1)||round(cast(tint.longitude as numeric),1)) "+
    "window tinc ['15 minutes'], tm ['5 minutes']; ";
  private JCheckBox button4 = new JCheckBox("S-S-T-T Join (Measurements/Incidents/Stations/Intersections)");

  public final int QUERY1 = 1;
  public final int QUERY2 = 2;
  public final int QUERY3 = 3;
  public final int QUERY4 = 4;
    
  private Thread t = new Thread();
  private Thread queryt1 = null;
  private Thread queryt2 = null;
  private Thread queryt3 = null;
  private Thread queryt4 = null;

  
  public static String currentQuery = "";
      
  public QuerySelect(Mapper map, Model model, QueryView qv,
		     String mappermode) {
    this.map = map;
    this.model = model;
    this.qv = qv;

    setLayout(new FlowLayout(FlowLayout.CENTER));
    setBackground(Color.white);
	

    Thread t = new Thread() {
	public void run() {
	  if(incq !=  null) incq.stop();
	  if(sq != null) sq.stop();
	  if(siq != null) siq.stop();
	  if(gcq != null) gcq.stop();
	}
      };
    Runtime.getRuntime().addShutdownHook(t);


    if(mappermode.equals(Main.MAPPERMODE_INCIDENTS))
    {
      add(button1);
      add(button2);
      add(button4);
      button1.addItemListener(this);
      button2.addItemListener(this);
      button4.addItemListener(this);
    }
    else if(mappermode.equals(Main.MAPPERMODE_GEOCODE))
    {
      add(button3);
      button3.addItemListener(this);
    }
    else 
    {
      System.err.println("bad mappermode must be " + Main.MAPPERMODE_GEOCODE + " or " + Main.MAPPERMODE_INCIDENTS);
      System.exit(1);
    }



  }

    
  public void itemStateChanged(ItemEvent e) {
    // need to end the previous query...

    try 
    {
      Object item = e.getItemSelectable();
      if (item == button1) {
	if(e.getStateChange() == ItemEvent.DESELECTED)
	{
	  sq.stop();
	  queryt1.stop();
	  qv.clear(1);
	}
	else
	{
	  sq = new StationQuery(model, map);
	  qv.insertText(1,query1);
	  queryt1 = new Thread(sq);
	  queryt1.start();
	  System.out.println("Clicked on button1! " + currentQuery);
	}
      }
      else if (item == button2) {
	if(e.getStateChange() == ItemEvent.DESELECTED) 
	{
	  incq.stop();
	  qv.clear(2);
	  queryt2.stop();
	}
	else
	{
	  qv.insertText(2,query2);
	  incq = new IncidentQuery(model,map);
	  queryt2 = new Thread(incq);
	  queryt2.start();
		    
	  System.out.println("Clicked on button2! " + currentQuery);
	}
      }
      else if (item == button3) {
	if(e.getStateChange() == ItemEvent.DESELECTED)
	{
	  gcq.stop();
	  qv.clear(3);
	  queryt3.stop();
	}
	else
	{
	  qv.insertText(3,query3);
	  // change map to US map.
	  map.setGeoCodeMapper();
	  gcq = new GeoCodeQuery(model.getModelData(), model, map.pointVector, map);
	  queryt3 = new Thread(gcq);
	  queryt3.start();
		    
	  System.out.println("Clicked on button3! " + currentQuery);
	}
      }
      else if (item == button4) {
	if(e.getStateChange() == ItemEvent.DESELECTED) 
	{
	  siq.stop();
	  qv.clear(4);
	  queryt2.stop();
	}
	else
	{
	  qv.insertText(4,query4);
	  siq = new StationIncidentQuery(model,map);
	  queryt4 = new Thread(incq);
	  queryt4.start();
		    
	  System.out.println("Clicked on button4! " + currentQuery);
	}
      }
    }
    catch (Throwable t)
    {
      t.printStackTrace();
    }
    System.err.println("ItemEvent handled");
  }

    

}

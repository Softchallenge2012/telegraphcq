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
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

/* =================================================================================================  */
/* ======================================= CLASS:  ConfigFile ======================================  */
/* =================================================================================================  */
//config file reads in a file of the format:
/*
  variable=value
  %comment

  variable2=value
  ...
*/
// and allows the values of variables to be fetched (simple preferences file)

public class ConfigFile {
    Hashtable params;

    public ConfigFile(String file) throws IOException {
      String line, var, value;
      StringTokenizer stk;
      int lineno = 0;

      BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));

      params = new Hashtable();
      try {
	System.out.println("Reading config file: " + file);
	while (true) {
	  line = reader.readLine();
	  if (line == null) break;
	  lineno++;
	  if (line.trim().length() == 0) continue;
	  if (line.charAt(0) == '%') continue;
	  stk = new StringTokenizer(line, "=");
	  try {
	    var = stk.nextToken().trim();
	    value = stk.nextToken().trim();
	    while (stk.hasMoreTokens()) {
	      value += "=" + stk.nextToken();
	    }
	  } catch (NoSuchElementException e) {
	    System.out.println("Error on line " + lineno + " of configuration file " + file + ", invalid format, " + line);
	    break;
	  }
	  params.put(var,value);
	  System.out.println("read var = " + var + ", value = " + value);
	}
      } catch (IOException e) {
	
      }
    }

    public String getParam(String key) throws NoSuchElementException {
      System.out.println("lookup " + key);
      String param = (String)params.get(key);
      if (param == null)
	  {
	      param = System.getProperties().getProperty("tcq."+key);
	      if(param == null) 
		  throw new NoSuchElementException("unknown parameter " + key);
	  }
      System.out.println("got " + param);
      return param;
    }
    
  }



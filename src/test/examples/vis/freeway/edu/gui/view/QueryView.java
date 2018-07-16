package edu.gui.view;

import edu.gui.*;
import ptolemy.plot.*;
import mapper.*;

import java.util.*;
import javax.swing.*;
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
 * Allows the user to view query from QuerySelect.java.
 */
public class QueryView extends JPanel {

    Mapper map;
    Model model;
    JTextArea [] areas;
    static final String[] delimeters = {"select", "from", "where", "group by", "order by", "window" };


    public static String query2Display = "";

    public QueryView(int numqueries) {
	//this.map = map;
	//this.model = model;


	setLayout(new FlowLayout(FlowLayout.CENTER));
	setBackground(Color.white);
	areas = new JTextArea[numqueries];
	for(int i=0; i < numqueries; i++)
	    {
		areas[i] = new JTextArea(query2Display);
		add(areas[i]);
	    }



    }


    public void insertText(int querynum, String s) {
	String result = "";

	int len = 25; // width of text area
	int lines = s.length() / len + 1;
	int curPos = 0;
	int curLength = s.length() -1;
	
	areas[querynum-1].setRows(lines);
	areas[querynum-1].setColumns(20);
	areas[querynum-1].setLineWrap(true);

	/**
	 * Garrett's wrapping code.
	 */

	String query = s;
	query = query.toLowerCase();
	String formatted_query = new String();
	
	/* get all the positions that separate major clauses */
	int[] delimeter_positions = new int[delimeters.length];
	for(int j = 0; j < delimeters.length; j++) {
	    delimeter_positions[j] = query.indexOf(delimeters[j]);
	}
	
	/* reconstruct query with formatting between delimeters */
	for(int j = 0; j < delimeters.length; j++) {
	    if(delimeter_positions[j] == -1) {
		System.out.println("absent clause" + delimeters[j]);
		continue;
	    }
	    else {
		int k = j+1;
		while (k < delimeters.length &&
		       delimeter_positions[k] == -1) {
		    k++;
		}
		if (k >= delimeters.length) {
		    /* final clause */
		    //printdbg("Formatting Query", "Final clause: " + delimeters[j]);
		    formatted_query = formatted_query.concat(query.substring(delimeter_positions[j]));
		    formatted_query = formatted_query.concat("\n");
		}
		else {
		    /* get substr between j and k and concat */
		    formatted_query = formatted_query.concat(query.substring(delimeter_positions[j],
									     delimeter_positions[k]));
		    formatted_query = formatted_query.concat("\n");
		}
	    }
	    System.out.println(formatted_query);
	}
	
	
	result = formatted_query;
	areas[querynum-1].setText(result);
    }

    public void clear(int querynum) {
	areas[querynum-1].setText("");
    }

}

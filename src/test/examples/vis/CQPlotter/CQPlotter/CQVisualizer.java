package CQPlotter;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.Properties;

import java.io.*;

class MyProperties extends java.util.Properties
{
    public String getProperty(String key) 
    {
	String value = super.getProperty(key);
	if(value == null)
	    {
		value = System.getProperties().getProperty("tcq." + key);
	    }
	return value;
    }

    public String getProperty(String key, String def)
    {
	String value = getProperty(key);
	if(value == null)
	    return def;
	return value;
    }
}

public class CQVisualizer implements ActionListener {
    
    public CQVisualizer(String config_file) {
	/* Set Hook to catch Ctrl-C to close connections */
	Runtime.getRuntime().addShutdownHook(new Thread() {
		public void run() {
		    cleanup();
		}
	    });;
	
	/* Load information from the configuration file */
	try {
	    FileInputStream fis = new FileInputStream(config_file);
	    prop.load(fis);
	} catch (Exception e) {
	    e.printStackTrace();
	}
	
	/* Start parsing information */
	try {
	    num_queries_to_display = Integer.valueOf(prop.getProperty("NumQueries")).intValue();
	    host = prop.getProperty("Host");
	    user = prop.getProperty("User");
	    password = prop.getProperty("Password");

	    /* Initialize the variable arrays */
	    titles = new String[num_queries_to_display];
	    xlabels = new String[num_queries_to_display];
	    ylabels = new String[num_queries_to_display];
	    xfields = new String[num_queries_to_display];
	    yfields = new String[num_queries_to_display];
	    queries = new String[num_queries_to_display];
	    expiration_time_ms = new int[num_queries_to_display];
	    history_size = new int[num_queries_to_display];
	    window_size = new int[num_queries_to_display];
	    
	    for(int i = 0; i < num_queries_to_display;i++) {
		titles[i] = prop.getProperty("Title" + i);
		xlabels[i] = prop.getProperty("XLabel" + i);
		ylabels[i] = prop.getProperty("YLabel" + i);
		xfields[i] = prop.getProperty("XField" + i);
		yfields[i] = prop.getProperty("YField" + i);
		queries[i] = prop.getProperty("Query" + i);
		expiration_time_ms[i] = Integer.valueOf(prop.getProperty("ExpirationTime" + i)).intValue();
		history_size[i] = Integer.valueOf(prop.getProperty("HistorySize" + i)).intValue();
		window_size[i] = Integer.valueOf(prop.getProperty("WindowSize"+i)).intValue();
	    }

	} catch (Exception e) {
	    e.printStackTrace();
	}

	/* Load the PostgreSQL Driver */
	try {
	    Class.forName("org.postgresql.Driver");
	} catch(ClassNotFoundException cnfe) {
	    System.out.println(cnfe);
	    cnfe.printStackTrace();
	}	
		
	/* Create all the CQPlotters */
	cqplots = new CQPlotter[num_queries_to_display];


	/* set up the frames */
	command_content_panel = new JPanel();
	sql_content_panel = new JPanel();
	sql_content_panel.setLayout(new BoxLayout(sql_content_panel, BoxLayout.Y_AXIS));
	command_content_panel.setOpaque(true);
	sql_content_panel.setOpaque(true);

	command_frame = new JFrame("TelegraphCQ Data Visualizer");
	command_frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	command_frame.setContentPane(command_content_panel);

	sql_frame = new JFrame("SQL Viewer");
	// sql_frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	sql_frame.setContentPane(sql_content_panel);
	
	/* set up buttons */
	sql_buttons = new JRadioButton[num_queries_to_display];
	query_buttons = new JButton[num_queries_to_display];
	exit_button = new JButton("Exit");

	/* set up sql display window */
	sql_pane = new JTextPane();
	sql_pane.setPreferredSize(new Dimension(700,400));
	sql_pane.setEditable(false);
	sql_font = new Font("Serif", Font.PLAIN, 30);
	sql_pane.setFont(sql_font);
	
	for(int i = 0; i < num_queries_to_display; i++) {
	    /* set up the query buttons */
	    query_buttons[i] = new JButton(titles[i]);
	    query_buttons[i].setActionCommand(String.valueOf(i));
	    query_buttons[i].addActionListener(this);
	    query_buttons[i].setText("Start \"" + titles[i] + "\" Query");

	    /* set up the SQL buttons */
	    sql_buttons[i] = new JRadioButton(titles[i]);
	    sql_buttons[i].addActionListener(this);
	    sql_buttons[i].setActionCommand(String.valueOf(i+1000));
	}
	
	/* get any extra buttons */
	JButton temp_button;
	int num_buttons = Integer.valueOf(prop.getProperty("NumExtraButtons")).intValue();
	extra_commands = new String[num_buttons];
	extra_buttons = new JButton[num_buttons];
	for(int i = 0; i < num_buttons; i++) {
	    extra_buttons[i] = new JButton(prop.getProperty("ButtonTitle"+i));
	    extra_buttons[i].setActionCommand(String.valueOf(i+100));
	    extra_buttons[i].addActionListener(this);
	    extra_commands[i] = new String(prop.getProperty("ButtonCommand"+i));
	}	    
	exit_button.setActionCommand("exit");
	exit_button.addActionListener(this);

	sql_button_group = new ButtonGroup();
	
	/* add the buttson to the command_frame */
	for(int i = 0; i < num_queries_to_display; i++) {
	    command_frame.getContentPane().add(query_buttons[i]);
	    sql_frame.getContentPane().add(sql_buttons[i]);
	    sql_button_group.add(sql_buttons[i]);
	}
	for(int i = 0; i < num_buttons; i++) {
	    command_frame.getContentPane().add(extra_buttons[i]);
	}
	command_frame.getContentPane().add(exit_button);
	sql_frame.getContentPane().add(sql_pane);

	/* set up and show the frame */
	command_frame.pack();
	command_frame.setVisible(true);
	sql_frame.pack();
	sql_frame.setVisible(true);
       
    }

    public void exit() {
	for(int i = 0; i < num_queries_to_display; i++) {
	    if (cqplots[i] != null) {
		cqplots[i].frame.setVisible(false);
		cqplots[i].exit();
		cqplots[i] = null;
	    }
	}
	System.exit(0);
    }

    public void actionPerformed(ActionEvent ae) {
	if ("exit".equals(ae.getActionCommand())) {
	    /* exit the entire thing... */
	    exit();
	}
	else {
	    int i = Integer.valueOf(ae.getActionCommand()).intValue();
	    if (i >= 1000) {
		/* SQL Radio Buttons */
		printdbg("ActionPerformed", "Displaying new SQL");
		String query = queries[i-1000];
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
			    printdbg("Formatting Query", "Final clause: " + delimeters[j]);
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

		sql_pane.setText(formatted_query);
	    }
	    else if (i >= 100) {
		printdbg("ActionPerformed", "Extra button clicked, executing " + extra_commands[i-100]);
		try {
		    Runtime.getRuntime().exec(extra_commands[i-100]);
		} catch (Exception e) {
		    e.printStackTrace();
		}
	    }
	    else {
		/* SQL Queries */
		if (cqplots[i] == null) {
		    /* Query already destroyed, restart it */
		    printdbg("ActionPerformed", "Creating new Query");
		    cqplots[i] = new CQPlotter(titles[i], xlabels[i], ylabels[i], queries[i],
					       xfields[i], yfields[i], host, user, password,
					       expiration_time_ms[i], history_size[i], window_size[i]);
		    cqplots[i].start();
		    query_buttons[i].setText("Stop \"" + titles[i] + "\" Query");
		}
		else {
		    /* Query active, destroy it */
		    printdbg("ActionPerformed", "Destroying Old Query");
		    cqplots[i].frame.setVisible(false);
		    cqplots[i].exit();
		    cqplots[i] = null;
		    query_buttons[i].setText("Start \"" + titles[i] + "\" Query");
		}
	    }
	}
    }

    public void cleanup() {
	System.out.println("Terminating CQVis");
 	for(int i = 0;i < num_queries_to_display; i++) {
	    if (cqplots[i] != null) {
		cqplots[i].stop();
		cqplots[i].cleanup();
	    }
	}
    }
    
    /* Main Statement */
    public static void main(String argv[]) {
	if (argv.length > 0) {
	    CQVisualizer cqv = new CQVisualizer(argv[0]);
	}
	else {
	    CQVisualizer cqv = new CQVisualizer("CQVisualizer.config");	
	}
    }
    
    /* Class Variables */
    public int num_queries_to_display;
    public String[] titles, xlabels, ylabels, xfields, yfields, queries;
    public String host, user, password;
    public int[] expiration_time_ms, history_size, window_size;
    public Properties prop = new MyProperties();
   
    CQPlotter[] cqplots;

    JFrame command_frame, sql_frame;
    JPanel command_content_panel, sql_content_panel;


    Font sql_font;
    JRadioButton[] sql_buttons;
    ButtonGroup sql_button_group;
    JTextArea sql_display;
    JTextPane sql_pane;
    JButton[] query_buttons;
    JButton exit_button;
    JButton[] extra_buttons;
    String[] extra_commands;

    static final String[] delimeters = {"select", "from", "where", "group by", "order by", "window" };

    /* Debugging Stuff */
    private static final boolean DEBUG = true;

    static void printdbg(String loc, String msg) {
	if (DEBUG) {
	    System.out.println(loc + ": " + msg);
	}
    }
}

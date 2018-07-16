package HtmlGet;
import java.lang.reflect.Constructor;
import org.w3c.www.protocol.http.*;
import org.w3c.www.protocol.http.cookies.*;
import TeSS.*;
import TeSS.util.*;
import gnu.regexp.*;
import org.w3c.www.mime.*;
import org.gnu.readline.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.sql.*;

public class HtmlGet
{

    static final String formbetweentags[][] = {
	{"<OPTION", "<[^>]*>"}
    };
    static final String formtags[] = 
    {
	"<HTML", "</HTML", "<BODY", "</BODY",
	"<FORM", "</FORM",
	"<INPUT", "</INPUT",
	"<SELECT", "</SELECT",
	"<TEXTAREA", "</TEXTAREA",
    };

    static final String jdbcDriver ="org.postgresql.Driver";
    static final String PROTOCOLHANDLERPROP ="java.protocol.handler.pkgs";
    static final String SSLPROTOCOLHANDLER = "com.sun.net.ssl.internal.www.protocol"; 
    static 
    {
		
	try {
	    HtmlGet.sUseReadline=true;
	    Readline.load(ReadlineLibrary.GnuReadline);
	    Readline.initReadline("monster");
	    System.out.println("using readln");

	}
	catch (Throwable ignore_me) {
	    System.err.println("couldn't load readline lib. Using simple stdin.");
	    HtmlGet.sUseReadline=false;
	}
	
	try 
	    {
		Class.forName(jdbcDriver);
	    }
	catch(Exception e)
	    {
		System.out.println("couldn't load jdbc driver:" +e.getMessage());
	    }

	Properties p = System.getProperties();
	String urlHandlers = p.getProperty(PROTOCOLHANDLERPROP);
	if(urlHandlers == null)
	    {
		urlHandlers = SSLPROTOCOLHANDLER;
	    }
	else
	    {
		urlHandlers = urlHandlers + File.pathSeparator + SSLPROTOCOLHANDLER;
	    }
	p.setProperty(PROTOCOLHANDLERPROP, urlHandlers);
	try {
	    Class.forName(SSLPROTOCOLHANDLER + ".https.Handler");
	}
	catch (Throwable t)
	    {
		System.out.println("SSL is turned off");
	    }

	
	
				
    }




    public static void main(String [] args) throws Exception
    {
	boolean isDebug =false;
	String jscdir=null;
	String cookiefile="/tmp/cookies";
	String historyfile="/tmp/history";
	switch(args.length)
	    {
	    case 4:
		isDebug = args[3].equalsIgnoreCase("debug");
	    case 3:
		jscdir = args[2];
	    case 2:
		historyfile = args[1];
	    case 1:
		cookiefile = args[0];
		defualt:
		break;
	}
		
		   

	HtmlGet h = new HtmlGet(cookiefile, historyfile,
				jscdir,
				isDebug);

	while(true)
	{
	    try
		{
		    h.enterCommands(System.in);
		    System.exit(0);
		}
	    catch (java.io.EOFException e)
		{
		    break;
		}	
	    catch (Throwable t)
		{
		    t.printStackTrace();
		    System.out.println("Command exited with an error: " + t.getMessage() + " continuing....");
		}
	}
    
	System.exit(0);
    }

    String getSelection() throws Exception
    {
	if(mHtmlArea == null) return "";
	int startsel = mHtmlArea.getSelectionStart();
	int endsel = mHtmlArea.getSelectionEnd();
	int length = endsel -startsel;
	if(endsel <= startsel ||  length <= 0)
	    {
		return "";
	    }
	else
	    {

		StringWriter sw = new StringWriter();
		HTMLWriter hw = new HTMLWriter(sw, (HTMLDocument)mHtmlArea.getDocument(), startsel, length);
		hw.write();

		return sw.toString();
	    }

    }
    void submitForm(Form f, String props, boolean dontencode) throws Exception
    {
	if(f==null) return;
	if(props==null)props="";
	Properties formProperties = f.getProperties();
	Properties p = convertStringToPostProperties(props);

	for(Enumeration argProperties = p.propertyNames(); argProperties.hasMoreElements(); )
	    {
		String key = (String)argProperties.nextElement();
		String value = (String)p.get(key);
		formProperties.put(key, value);
	    }
	URL u  = new URL(mCurrentURL, f.getAction()+"?"+convertPropertiesToPostString(formProperties,false));
	String formcommand = ((f.getIsPost()) ? "POST " : "GET ") + u.toExternalForm();
	if(f.getIsPost() && dontencode)
	    formcommand += ",false";
	System.out.println(formcommand);
	parseAndProcessCommand(formcommand, false);

    }
    void getFormsNew(String page)
    {
	String justformspage = justtags(page, 
					formtags,
					formbetweentags
					);
	System.out.println("justforms page is: " +justformspage);
	HTMLView h = new HTMLView(justformspage, mCurrentURL, this);
	h=null;
    }

    private String justtags(String page, 
			    String [] keeptags,
			    String [][] betweentags
			    )
    {
	String tagstr="<[^>]*>";
	REMatch startmatch=null;
	StringBuffer output = new StringBuffer();
	REMatch endmatch=null;
	int startpos=0;
	int endpos=0;
	int [] betweencount=null;
	RE keeptagsRE[] = null;
	RE betweentagsRE[][]=null;

	if(keeptags != null)
	  keeptagsRE = compileRE(keeptags, RE.REG_ICASE); 
	if(betweentags != null)
	{
		betweentagsRE = new RE[betweentags.length][2]; 

		for(int i=0;
		    i < betweentags.length;
		    i++)
		{
			if(betweentags[i] == null) continue;
			RE temp[] = compileRE(betweentags[i], RE.REG_ICASE);
			betweentagsRE[i][0] = temp[0];
			betweentagsRE[i][1] = temp[1];
		}
	}

	try 
	    {
		RE startRE=new RE(tagstr);
		int idx=0;
		while(true)
		    {
	
			startmatch = startRE.getMatch(page, idx);
			if(startmatch==null) break;
			tagstr=startmatch.toString();
			tagstr = removeWS(tagstr);
			startpos = startmatch.getStartIndex();
			endpos = startpos + tagstr.length();
	
			if(betweentags != null)
			    {
				if(betweencount == null) 
					  betweencount = new int[betweentags.length];
				for(int betweenidx=0;
				    betweenidx < betweentags.length;
				    betweenidx++)
				    {
					if(betweentags[betweenidx][0] == null) continue;	
					if(isMatchingTag(tagstr, 
							 betweentagsRE[betweenidx][0])) 
					    {
						REMatch m = betweentagsRE[betweenidx][1].getMatch(page, endpos);
					
						if(m==null) continue;
						
						endpos = m.getEndIndex();
						tagstr = page.substring(startpos, endpos);
						removeWS(tagstr);
						output.append("<!-- between tags instance " + betweencount[betweenidx] + " START for tag " + betweenidx +" >\n ");
						output.append(tagstr + "\n");
						output.append("<!-- between tags instance " + betweencount[betweenidx] + " END for tag " + betweenidx +" >\n ");
						idx=m.getEndIndex();
						betweencount[betweenidx]++;
						continue;
					    }
				    }
			    }

			if(keeptags != null)
			    {
				for(int i=0; i < keeptags.length; i++)
				    {
					if(isMatchingTag(tagstr, keeptagsRE[i]))
					    {
						output.append(tagstr+"\n");
						
						System.out.println("justtags accepted a tag " + tagstr);
						break;
						
					    }
				    }
			    }
			idx=endpos;
				
		    }
		    
	    }
	catch(Exception e)
	    {
		System.out.println("justtags failed");
		e.printStackTrace();
	    }
	
	return output.toString();
    }

    protected String removeWS(String input)
	{
	   StringBuffer output = new StringBuffer();
	   int inputlen=input.length();
	   for(int i=0; i < inputlen; i++)
	   {
		char c = input.charAt(i);
		if(c=='\n' || c=='\r' || c=='\t')
		 continue;
		else
		  output.append(c);
	}
	return output.toString();		
	}
    protected boolean isMatchingTag(String tagstr, RE template)
    {
	if(template==null) return false;
	return template.getMatch(tagstr) != null;
    }

    RE[] compileRE(String [] args, int reoptions)
	{
	   RE resultRE[] = new RE[args.length];
		for(int i=0;
		    i < args.length;
		    i++)
		{
		    try
			{
			    resultRE[i] = new RE(args[i], reoptions);
			}
		    catch(Exception e)
			{
			    resultRE[i] = null;
			}
		}
		return resultRE;
        }

			 
    void getForms(String page)
    {
	StringBuffer sb = new StringBuffer(page);
	filter(sb, "<img", ">");
					   
	HTMLView h = new HTMLView(sb.toString(), mCurrentURL, this);
	h=null;
    }

    public int runTess(String htmlPage, Vector results, String jscfilestr, Vector args)
 	throws Exception
    {
    URLScan scan=null;
	Object [] row=null;
	int count=0;
	try 
	    {
		File jscfile = new File(jscfilestr);
		if(!jscfile.exists()) 
			throw new java.io.FileNotFoundException("file : " + jscfile + " does not exist");
		URLConfig urlconfig = new URLConfig(jscfile);
		SQLProperties tp = new SQLProperties();
		if(args != null)
		    {
			for(int i =0; i < args.size() ; i++)
			    {
				String value=(String)args.get(i);
				String name = urlconfig.getArgs()[i];
				
				tp.addProperty(name, value);
			    }
		    }
		if(htmlPage != null)
		{
		
			scan = new URLScan(urlconfig, new StringBufferInputStream(mCurrentPage.toString()));
		}
		else
		{ 
			scan = new URLScan(urlconfig, tp);
		}
		while((row = scan.getNextRow()) != null)
		    {
			results.add(row);
			count++;
		    }
	    }
	catch (Exception t)
	    {
			if(count==0) throw t;
	    }
	return count;
    }
    void addToHistory(String command, boolean alreadyInReadline)
    {
	if(sUseReadline && !alreadyInReadline)
	    {
		Readline.addToHistory(command);
	    }
	mCommandHistory.add(command);
    }
    public HtmlGet(String cookieFilePath, String historyfile,
		   String jscdir,
		   boolean isDebug) throws Exception
    {
	Properties managerProps = new Properties();
	managerProps.setProperty(CookieFilter.COOKIES_FILE_P, cookieFilePath);
	mManager = HttpManager.getManager(managerProps) ;
	mCookieFilter = new CookieFilter();
	mCookieFilter.initialize(mManager);
	if(isDebug)
	{
		mDebugFilter = new DebugFilter();
		mDebugFilter.initialize(mManager);
        }
	mCommandHistory=new Vector();
	mTessResults = new Vector();
	mCurrentPage=new StringBuffer();
	mCurrentForms=new java.util.HashMap();
	mCurrentURL=null;
	mUserAgentName="Myzilla";
	mImageFilter=false;
	if(jscdir != null)
	{
	    File f = new File(jscdir);
	    if(!f.isDirectory())
		{
		    throw new Exception ("jscdir points to a file.");
		}
	    else
		{
		    jscdir = f.getCanonicalPath();
		    System.out.println("file references will be relative to: " + jscdir);
		}
	    System.setProperty("user.dir", 
					     jscdir);
	    
        }	

	mHistoryfile=historyfile;
		
    }
	
    public boolean processResultsWithJava(String classname)
    throws Exception
    {
		Class [] signature = new Class[2];
		Object [] actualArguments = new Object[2];
		actualArguments[0] = mTessResults;
		actualArguments[1] = this;
		signature[0]=Class.forName("java.util.Vector");
		signature[1]=Class.forName("HtmlGet.HtmlGet");
		Class classobj = Class.forName(classname);
		Constructor constructor = classobj.getConstructor(signature);
		if(constructor== null) return false;
		Object myObject = constructor.newInstance(actualArguments);
		if(!(myObject instanceof java.lang.Runnable))
		    return false;
		Runnable r = (Runnable)myObject;
		r.run();
	return true;
    }

    public boolean enterCommands(InputStream in) throws Exception
    {
	String s;
	boolean isPost=false;
	boolean print=false;
	boolean cookietest=false;
	String command=null;
	Reply       reply=null;
	BufferedReader inreader=null;
	boolean alreadyInReadline=false;

	try
	    {
		if((in == System.in) && (sUseReadline == true))
		    {
			inreader= new BufferedReader(new ReadlineReader("HtmlGet>",new File(mHistoryfile),ReadlineLibrary.GnuReadline));
			alreadyInReadline=true;
		    }
		else
		    inreader = new BufferedReader(new InputStreamReader(in));
		while((command = inreader.readLine()) != null)
		    {
			addToHistory(command.trim(), alreadyInReadline);
			if(!parseAndProcessCommand(command, true))
			    return false;
	            }	
		return true;
	    }
	finally
	    {
		if(inreader != null) inreader.close();
	    }
    }
    

    public boolean  parseAndProcessCommand(String input, boolean alreadyInHistory) throws Exception
    {
	input.trim();
	if(alreadyInHistory == false) addToHistory(input,false);
	Vector v = new Vector();
	int idx = input.indexOf(' ');
	if(idx == -1)  idx = input.length();
	if(idx == 0) return true;
	String command = input.substring(0,idx).trim();
	input = input.substring(idx);
	StringTokenizer cmdtokenizer = new StringTokenizer(input, ",");
	while(cmdtokenizer.hasMoreElements())
	    {
		v.add(((String)cmdtokenizer.nextElement()).trim());
	    }

		
	return processCommand(command, v);
    }

    void usage()
    {
	System.out.println("GETTING AND SCRAPING PAGES\n");
	System.out.println("get url: load the named url using the GET method");
	System.out.println("post url: load the named url using the post method");
	System.out.println("file filename: process the above commands from the named file");

	System.out.println("runtess use_current_page(true or false), newscrape(true,false),jscfile,outfile(or null for no file output)");
	System.out.println("\t- first argument is true or false and determines if the current page is scraped, or the page named in the .jsc file is scraped");
	System.out.println("\t-  second argument is true or false and determines if results\n\t  are addded to the previous results.");


	System.out.println("\n FOLLOWING LINKS \n");

	System.out.println("crawlandscrape jscfile, qcontains, qnotcontains, linktext: crawl a series of similar pages extracting results");
	System.out.println("cleariterator: clear the page iterator");
	System.out.println("nextpage qcontains, qnotcontains, linktext: set up a page crawler");
	System.out.println("showpage: display a graphical representation of the page using swings html renderer");
	System.out.println("hidepage: don't show the graphical representation of the page");
	System.out.println("savepage file: save current page to a file");
	System.out.println("dumppage: display the html for the current page");
	System.out.println("currenturl: shows the url that is currently loaded");
	System.out.println("savehistory: save the list of urls visited in the session, including redirections");

	System.out.println("getselection: print html for the highlighted section of the doc");

	System.out.println("getforms: parse the current html page to extract the forms");
	System.out.println("listforms: list parsed forms");
	System.out.println("listformproperties form: print out the properties for this form");
	System.out.println("submitform name, values: submit the named form.  argument 2 is a query string that specifies form values");


	System.out.println("DATA OUTPUT\n");

	System.out.println("resultstodb table, jdbc-url, user, password: place current results into a database");
	System.out.println("resultstocsv file: output the current results ");  
	System.out.println("resultstojava class: run a java class over the results vector.  The named class must implement the Runnable interface, and must have a constructor which takes a Vector and a HtmlGet object as arguments.");

	System.out.println("\nDEBUGGING\n");
	System.out.println("print : toggle debugging output");
	System.out.println("printcookies url: print the cookies for the given url");

    }

    void substituteBindings(Vector args, Vector variableBindings) throws Exception
    {
	String arg;
	REMatch match;
	int argno=0;
	int end=0;
		
	if(variableBindings == null ||
	   variableBindings.size() == 0) 
	    return;
	RE binding = new RE(("[$]ARG:([0-9]+)[$]"));;
	for(int i=0; i < args.size(); i++)
	    {
		end=0;
		arg = (String)args.get(i);
		int nummatches=0;
		String oarg="";
		REMatch [] matches = binding.getAllMatches(arg);
		for(int matchidx=0; matchidx < matches.length;
		    matchidx++
		    )
		    {
			nummatches++;
			match = matches[matchidx];
			System.out.println("end="+end+", matchstart=" + match.getStartIndex());
			oarg += arg.substring(end, match.getStartIndex());
			argno = Integer.parseInt(arg.substring(match.getStartIndex(1),
							       match.getEndIndex(1)));
			if(argno >= variableBindings.size())
			    {
				throw new Exception("variable out of range: " + arg);

			    }
			else
			    {
				oarg += variableBindings.get(argno);
			    }
			end=match.getEndIndex();
		    }
		if(end < arg.length())
		    {
			oarg += arg.substring(end);
		    }
		if(nummatches==0)
		    {
			oarg=arg;
		    }
		args.set(i, oarg);
	    }

    }
								
	
				

    public boolean processCommand(String command, Vector args) throws Exception
    {
	substituteBindings(args, mCurrentBindings);
	boolean isPost=false;
	String url="";
	if(command.equalsIgnoreCase("currenturl"))
	    {
		System.out.println(mCurrentURL.toExternalForm());
	    }
	if(command.equalsIgnoreCase("cleariterator"))
	    {
		mResultCrawler=null;
	    }

		if(command.equalsIgnoreCase("nextpage"))
		{
                    int nargs = args.size();
		    String arg1=null;
		    String arg2=null;
		    String arg3=null;

		    if(mResultCrawler == null)
			{
			    if(nargs >0)
				{
				    arg1 = (String)args.get(0);
				    if(arg1.equalsIgnoreCase("null"))
					arg1=null;
				}
			    if(nargs >1)
				{
				    arg2 = (String)args.get(1);
				    if(arg2.equalsIgnoreCase("null"))
					arg2=null;
				}

			    if(nargs >2)
				{
				    arg3 = (String)args.get(2);
				    if(arg3.equalsIgnoreCase("null"))
					arg3=null;
				}

			    mResultCrawler=new ResultCrawler(mCurrentURL,arg1,arg2, arg3);
			
		    mResultCrawler.processPage(mCurrentPage.toString());
		}
		URL u = mResultCrawler.getNext();
		if(u == null)
		    {
			mResultCrawler=null;
			System.out.println("result crawler didn't find another page");
			return true;
		    }
		else
		    {
			parseAndProcessCommand("GET " + u.toExternalForm(), false);
			mResultCrawler.processPage(mCurrentPage.toString());
		    }
    }

		if(command.equalsIgnoreCase("runtess"))
		    {
			int count=0;
			PrintStream outpr = null;
			if(args.size() <  3) 
				{
				System.out.println("usage: runtess currentpage,reset,jscfile,[args...]");
				return true;
				}

			Vector tessargs = new Vector();
			for(int argno=3; argno < args.size(); argno++)
			{
				tessargs.add(args.get(argno));
			}
			if(mTessResults == null)
				mTessResults=new Vector();
			
			if(((String)args.get(1)).equalsIgnoreCase("true"))
				mTessResults.clear();
			String rowstr="";
			Object [] row=null;
			if(((String)args.get(0)).equalsIgnoreCase("true"))
			{
				count =runTess(mCurrentPage.toString(), mTessResults, (String)args.get(2), tessargs);

			}
			else
			{
				count =runTess(null, mTessResults, (String)args.get(2), tessargs);

			}
			
		}		
		
    

    if(command.equalsIgnoreCase("tcqsource"))
    {
	// host
	// port
	// sourcename
	// frequency
	// duration 
	// file
	// args...
	int j=0;
	String host; 
	int port;
	String sourcename;
	int frequency;
	int duration;
	String queryfile;

	if(args.size() >= 6)
	    {
		Vector fileargs=new Vector();
		for(j=6; j < args.size(); j++)
		    {
			fileargs.add(args.get(j));
		    }
		host = (String)args.get(0);
		port = Integer.parseInt((String)args.get(1));
		sourcename = (String)args.get(2);
		frequency = Integer.parseInt((String)args.get(3));
		duration = Integer.parseInt((String)args.get(4));
		queryfile = (String)args.get(5);

		System.out.println("Calling TelegraphCQSource");
		    System.out.println("frequency="+frequency+",duration="+duration);
			TelegraphCQSource(host, port, sourcename, 
					  queryfile, fileargs,
					  frequency, duration);
			System.out.println("done with source");
	    }
    }
    

	if(command.equalsIgnoreCase("resulttodb"))
	    {
		// table, url, user, password
		if(args.size() == 4 && mTessResults.size() != 0)
		    {
			Object [] row=null;
			try
			{
			    System.out.println("connecting to url: "+ args.get(1) + ", user="+args.get(2)+",passwd="+args.get(3)); 
			    java.sql.Connection c = DriverManager.getConnection((String)args.get(1), 
										(String)args.get(2), 
										(String)args.get(3));
			    String sql = generateSqlStatement((String)args.get(0),
							      ((Object[])mTessResults.get(0)).length+1);
			    PreparedStatement stmt = c.prepareStatement(sql);
			    stmt.setString(1, new java.util.Date().toString());
			    for(int i=0; i < mTessResults.size() ; i++)
				{
				    row =(Object[]) mTessResults.get(i);
				    for(int j=1; j <= row.length; j++)
					{
					    stmt.setObject(j+1, row[j-1]);
					    if(row[j-1].equals("null"))
						{
						    stmt.setNull(j+1, Types.VARCHAR);
						}
					}
				    stmt.executeUpdate();
				}
			    stmt.close();
			    c.close();
			}
			catch (SQLException e)
			    {
				System.out.println(e.getMessage());
			    }
			    
		    }
		  
	    }
	else if (command.equalsIgnoreCase("resultstocsv"))
	    {
		if(mTessResults.size() == 0) return true;
		PrintStream out = System.out;
		if(args.size() == 1)
		    {
			out=new PrintStream(new FileOutputStream((String)args.get(0)));
		    }
		for(int i=0; i < mTessResults.size(); i++ )
		    {
			out.println(objectArrayToCSV((Object[])mTessResults.get(i)));
		    }
		if(out != System.out) out.close();
	    }
	else if (command.equalsIgnoreCase("resultstojava"))
	    {
		if(args.size() != 1)
		    {
			System.out.println("usage: resultstojava class");
			return true;
		    }
		else
		    {
			return processResultsWithJava((String)args.get(0));
		    }
	    }
	if(command.equalsIgnoreCase("showpage"))
	    {
		render(mCurrentPage.toString(),mCurrentURL, true);
	    }
	else if(command.equalsIgnoreCase("savepage"))
	    {
		FileWriter fw =new FileWriter(new File((String)args.get(0)));
		fw.write(mCurrentPage.toString());
		fw.close();
	    }
	else if(command.equalsIgnoreCase("dumppage"))
	    {
		System.out.println(mCurrentPage.toString());
	    }
	else if(command.equalsIgnoreCase("savehistory"))
	    {
		if(args.size() ==0)
		{
		   System.out.println("usage: savehistory filename");
		   return true;
		}
		savehistory(new File((String)args.get(0)));
	    }
	else if(command.equalsIgnoreCase("dumphistory"))
	    {
		System.out.println(mCommandHistory.toString());
	    }
	if(command.equalsIgnoreCase("hidepage"))
	    {
		mHtmlFrame.hide();
	    }
		
	else
	    if(command.equalsIgnoreCase("getforms"))
		{
		    getFormsNew(mCurrentPage.toString());
		}
		if(command.equalsIgnoreCase("justforms"))
		{
			String page = justtags(mCurrentPage.toString(), 
					       formtags,
					       formbetweentags);	
			mCurrentPage = new StringBuffer(page);
		}
		else if(command.equalsIgnoreCase("betweentags"))
		    {
			if(args.size() == 0) return true;
			String [][] tags = new String[args.size()][2];
			for(int i=0,idx=0; i < args.size(); idx++,i+=2)
			    {
				tags[idx][0] = (String)args.get(i);
				tags[idx][1] = (String)args.get(i+1);
				System.out.println("betweentags pair is: " +args.get(i) +","+ args.get(i+1));
			    }
			String page = justtags(mCurrentPage.toString(),
					       null,
					       tags);
			mCurrentPage = new StringBuffer(page);
		    }
		else if(command.equalsIgnoreCase("justtags"))
		    {
			if(args.size() == 0) return true;
			String [] tags = new String[args.size()];
			for(int i=0; i < args.size(); i++)
			    {
				tags[i] = (String)args.get(i);
			    }
			String page = justtags(mCurrentPage.toString(),
					       tags, null);
			mCurrentPage = new StringBuffer(page);
		    }
	    else if(command.equalsIgnoreCase("getselection"))
		{
		    System.out.println(getSelection());
		}				  
	    else
		if(command.equalsIgnoreCase("submitform") 
		   || command.equalsIgnoreCase("noencodesubmitform"))
		    {
			if(args.size() < 1)
			    {
				System.out.println("usage: submitform form, [args...]");
				return true;
			    }

			boolean dontencode = command.equalsIgnoreCase("noencodesubmitform");
			String props="";
			String name=(String)args.get(0);
			if(args.size()==2)props=((String)args.get(1));
			submitForm((Form)mCurrentForms.get(name), props, dontencode);

		    }
		else if(command.equalsIgnoreCase("listforms"))
		    {
			for(Iterator i = mCurrentForms.keySet().iterator(); i.hasNext(); )
			    {
				Form f = (Form)mCurrentForms.get(i.next());
				System.out.println(f.getName());
			    }
		    }
		else if(command.equalsIgnoreCase("listformproperties"))
		    {
			if(args.size() == 1)
			    {
				Form f = (Form)mCurrentForms.get(args.get(0));
				if(f != null)
				    {
					System.out.println(f.printForm(true));
													   
				    }
			    }
		    }
	if(command.equalsIgnoreCase("GET"))
	    {
		if(args.size() == 0) return false;
		isPost=false;
		url = (String)args.get(0);
		processUrl(isPost, url, mPrint?System.out:null, true);
	    }
	else if(command.equalsIgnoreCase("POST"))
	    {
		boolean encodeValues=true;
		if(args.size() == 0) return false;
		isPost=true;
		url = (String)args.get(0);
		if(args.size() == 2)
		    {
			encodeValues = args.get(1).equals("true");
		    }
		processUrl(isPost, url, mPrint?System.out:null,encodeValues);
	    }
	else if(command.equalsIgnoreCase("imagefilter"))
	    {
		mImageFilter = !mImageFilter;
		System.out.println("imagefilter is " + mImageFilter);
	    }
	else if(command.equalsIgnoreCase("PRINT"))
	    {
		mPrint=!mPrint;
		System.out.println("print responses sent to:" + mPrint);
	    }
	else if(command.equalsIgnoreCase("printcookies"))
	    {
		if(args.size() == 0) 
		    {
			System.out.println("usage: printcookies url");
			return false;
		    }
		url =(String)args.get(0);
		getCookieFilter().LookupURLAndPrintCookies(new URL(url));
	    }
	else if(command.equalsIgnoreCase("COOKIEDUMP"))
	    {
		mManager.sync();
		System.out.println("forcing a sync on the manager");
	    }
	else if(command.equalsIgnoreCase("help"))
	    {
		usage();
		return true;
	    }
	else if(command.equalsIgnoreCase("FILE"))
	    {
		if(args.size() == 0) return false;
		String fname = ((String)args.get(0)).trim();
		File f = new File(fname);
		if(processFile(f, args) == false)
		    {
			throw new Exception("file " + f + " does not exist");			
			
		    }
	    }
	else if(command.equalsIgnoreCase("crawlandscrape"))
	    {
		if(args.size() < 4)
		    {
			System.out.println("usage: crawlandscrape jscfile, querynotcontains, query_notcontains, linktext, maxlinkstofollow");
			return true;
		    }
		if(getCurrentURL() == null)
		    {
			System.out.println("you must load the starting page before running crawlandscrape");
			return true;
		    }

		
		String arg1 = (String)args.get(0);
		String arg2 = (String)args.get(1);
		String arg3 = (String)args.get(2);
		String arg4 = (String)args.get(3);
		int linkstofollow=-1;
		if(args.size() == 5)
		    {
			linkstofollow =Integer.parseInt((String)args.get(4));
		    }

		if(arg2.equalsIgnoreCase("null"))
		    arg2=null;
		if(arg3.equalsIgnoreCase("null"))
		    arg3=null;
		if(arg4.equalsIgnoreCase("null"))
		    arg4=null;

		crawlAndScrape(arg1,arg2,arg3,arg4, linkstofollow);
	    }
	
	return true;
    }


    public void crawlAndScrape(String jscfile, 
				 String query_contains,
				 String query_doesnotcontain,
				 String linktext, 
			       int linkstofollow) throws Exception
    {
	URL nexturl=null;
	int totalrows=0;
	int rowsfound=0;
	int count=0;
	
		ResultCrawler resultiter = new ResultCrawler(getCurrentURL(),
							     query_contains,
							     query_doesnotcontain,
							     linktext);
		
		resultiter.processPage(getCurrentPage());
		do
		    
		    {
			System.out.println("running Tess on url " + getCurrentURL());
			rowsfound=runTess(getCurrentPage(),mTessResults, jscfile, null );
			System.out.println(" found " + rowsfound + " data items on page " + getCurrentURL());
			totalrows+=rowsfound;
			
			nexturl = resultiter.getNext();
			if(nexturl != null)
			    {
				System.out.println("found next page: " + nexturl.toExternalForm());
				parseAndProcessCommand("GET " + nexturl.toExternalForm(),false);
				System.out.println("running process page");
				resultiter.processPage(getCurrentPage());
				System.out.println("finished process page");
				if(linkstofollow > 0 &&
				   (count++ > linkstofollow)) 
				    {
					System.out.println("stopping crawlandscrape because the maximum number of links have been followed");
					    break; 
				    }
			    }
			

 
		    }
		while (nexturl != null);
		System.out.println("FINISHED: found " + totalrows +" items");
    }
   public String generateSqlStatement(String tableName, int numparams)
       {
	   String ret = "insert into " + tableName + " values (";
	   for(int i=0; i < numparams; i++)
	       {
		   ret += "?";
		   if(i != numparams-1) ret +=",";
	       }
	ret +=")";
	   return ret;
       }

    
    public boolean  processFile(File f, Vector args) throws Exception
    {

		
	if(!f.exists()) return false;

		
	FileInputStream fis = new FileInputStream(f);
	Vector oldBindings = mCurrentBindings;
	mCurrentBindings = args;
	try
	    {
		enterCommands(fis);
	    }
	finally 
	    {
		mCurrentBindings = oldBindings;
	    }
	return true;
    }


    public void savehistory(File f) throws IOException
    {
	FileWriter fw =new FileWriter(f);

	for(int i=0; i < mCommandHistory.size(); i++)
	    {
		fw.write(mCommandHistory.get(i) + " \n");
	    }
	fw.close();
    }
    /* page output routines */
    public  void consumeInput(Reply r, OutputStream os) throws Exception
    {
	BufferedWriter writer = null;
	if(os != null) writer = new BufferedWriter(new OutputStreamWriter(os));
	consumeInput(r, writer);
	if(writer != null) writer.flush();
    }

    public  void consumeInput(Reply r, BufferedWriter w) throws Exception
    {
	mCurrentForms.clear();
	mCurrentPage.delete(0, mCurrentPage.length());
	String s;
	BufferedReader br  = new BufferedReader(new InputStreamReader(r.getInputStream()));
	while((s=br.readLine()) != null)
	    {
		mCurrentPage.append(s+"\n");
		if(w != null)
		    {w.write(s); w.newLine();}
	    }
	br.close();
		filter(mCurrentPage, "<script", "</script[^>]*>");
		filter(mCurrentPage, "<meta", ">");
		filter(mCurrentPage, "<style", "</style[^>]*>");
	if(mImageFilter) filter(mCurrentPage, "<img",">");
    }
    public void processUrl(boolean isPost, String url, OutputStream os, boolean encodevalues) throws Exception
    {
	StringBuffer finalURL = new StringBuffer();
	Reply reply=null;
	if(isPost)
	    {
		URL u = new URL(mCurrentURL,url);				
		mCurrentURL=u;
		Properties p = convertStringToPostProperties(u.getQuery());
		int queryidx = url.indexOf('?');
		if(queryidx > 0) url = url.substring(0, queryidx);
		u = new URL(url);
		reply= requestWithRedirect(u, 1, isPost, p,finalURL, encodevalues);
		mCurrentURL=new URL(finalURL.toString());
	    }
	else
	    {
		URL u = new URL(mCurrentURL,url);
		mCurrentURL=u;
		reply= requestWithRedirect(u, 1, isPost,null, finalURL, encodevalues);
		mCurrentURL=new URL(finalURL.toString());
	    }
	consumeInput(reply, os);

    }


    public Reply requestWithRedirect(URL url, int redircount, boolean isPost, Properties postParams, boolean encodevalues) throws Exception
    {
	return requestWithRedirect(url, redircount, isPost, postParams, null, encodevalues);
    }
   


    public  Reply requestWithRedirect(URL url, int redircount, boolean isPost, Properties postParams, StringBuffer finalurl, boolean encodevalues) throws Exception
    {
	if(finalurl != null)
	    {
		finalurl.delete(0, finalurl.length());
		finalurl.append(url.toExternalForm());
		if(url.getFile().length() == 0 && url.getQuery() == null && 
		   (finalurl.charAt(finalurl.length()-1) != '/'))
		    {
			url = new URL(finalurl + "/");
		    }
		System.out.println("url is: " + finalurl);
	    }
	String queryString=null;
	if(redircount > 10) 
	    {
		System.out.println("more than 10 redirects, returning null");
		return null;
	    }
	Request request = mManager.createRequest() ;

	request.setUserAgent(mUserAgentName);
	if(isPost == false)
	    {
		request.setMethod("GET") ;
		request.setURL(url);
	    }
	else
	    {
		queryString = convertPropertiesToPostString(postParams,encodevalues);
		request.setMethod("POST");
		request.setURL(url);
		request.setContentLength(queryString.length());
		request.setOutputStream(new StringBufferInputStream(queryString));
		request.setContentType(new
				       MimeType("application/"+"x-www-form-urlencoded"));

	    }
	Reply    reply = getManager().runRequest(request) ;
	String redirect = getNewLocationIfRedirected(reply);
	if(redirect != null)
	    {
		consumeInput(reply, mPrint?System.out:null);
		URL n = new URL(request.getURL(), redirect);
		System.out.println(" redirecting to " + redirect);
		if(isPostRedirect(reply))
		    {
			reply=null;
			return requestWithRedirect(n, redircount+1, isPost, postParams, finalurl, encodevalues);
		    }
		else
		    {
			reply=null;
			return requestWithRedirect(n, redircount+1, false, null, finalurl, encodevalues);
		    }

	    }
	return reply;
    }





    /* redirection support */
    private boolean isPostRedirect(Reply r)
    {
	switch(r.getStatus())
	    {
	    case 307:
		return true;
	    }
	return false;
    }
    private static String getNewLocationIfRedirected(Reply r) 
    {
	switch(r.getStatus())
	    {
	    case 301:
	    case 302:
	    case 303:
	    case 307:
		return r.getLocation();
	    default:
		return null;
	    }

    }
    /* convert between properties based url attributes and string forms */
    public static String convertPropertiesToPostString(Properties p, boolean encode)
    {
	String ret="";
	String name="";
	String val="";
	boolean first=true;
	for(Enumeration e = p.propertyNames(); e.hasMoreElements(); )
	    {
		if(first==false) ret += "&";
		name = (String)e.nextElement();
		val = p.getProperty(name);
		System.out.println(name+"="+val);
		if(encode)val = URLEncoder.encode(val);
		ret += name + "=" + val;
		first=false;
	    }
	return ret;
    }

  
    public static  Properties convertStringToPostProperties(String s)
    {
	String queryString = s;
	Properties ret = new java.util.Properties();
	String name="";
	String value="";
	int start=0;
	int eqidx=0;
	int end=0;
	if(queryString == null || queryString.length() == 0)
	    return ret;
	do
	    {
		end   =  queryString.indexOf('&',start);
		if(end == -1) end = queryString.length();
		eqidx  = queryString.indexOf('=',start);
		name = queryString.substring(start,eqidx);
		value = queryString.substring(eqidx+1, end);
		ret.put(name, value);
		start=end+1;
				
	    }
	while((end < queryString.length()));
	return ret;
    }


    public void render(String html, URL baseurl,boolean visible) throws Exception
    {
	if(mHtmlArea == null)
	    {
		mHtmlArea = new HTMLView( html, baseurl, this);
		mHtmlScrollableArea = new JScrollPane(mHtmlArea);
		mHtmlFrame = new JFrame();
		mHtmlFrame.setContentPane(mHtmlScrollableArea);
		mHtmlArea.setEditable(false);


	    }
	else
	    {
		mHtmlArea.setNewPage(html,baseurl);
		mHtmlArea.setEditable(false);
	    }

	if(visible) mHtmlFrame.show();

    }

    private void filter(StringBuffer page, String start, String end)
    {
	REMatch startmatch=null;
	REMatch endmatch=null;
	int startpos=0;
	int endpos=0;
	try 
	    {
		RE startRE=new RE(start);
		RE endRE=new RE(end);
		

				
		while(true)
		    {
			String pagestr = page.toString();
			startmatch = startRE.getMatch(pagestr);
			if(startmatch==null) break;
			startpos = startmatch.getStartIndex();
			endmatch = endRE.getMatch(pagestr,startpos);
			if(endmatch == null) 
			    endpos = page.length();
			else
			    endpos =endmatch.getEndIndex();
						
			if(startpos !=-1)
			    {
				page.delete(startpos, endpos);
			    }
		    }
				
	    }
	catch(Exception e)
	    {
		System.out.println("filter failed");
		e.printStackTrace();
	    }
    }

    protected void addFormInformation(String formName, String formAction, String formMethod, String name, String value, Object model)
    {
	boolean shouldOutput=true;
	if(model instanceof DefaultButtonModel)
	    {
		shouldOutput=((DefaultButtonModel)model).isSelected();
	    }
	Form f = (Form)mCurrentForms.get(formName);
	if(f == null)
	    {
		boolean isPost=false;
		if(formMethod != null)
		    {
			isPost = formMethod.equalsIgnoreCase("post");
		    }
		f = new Form(formName, formAction, isPost);
				
				
		f.addProperty(name, new FormValue(value,model,shouldOutput));
		mCurrentForms.put(formName, f);
	    }
	else
	    {

		f.addProperty(name,new FormValue(value, model, shouldOutput));
	    }
    }

    public String objectArrayToCSV(Object [] o)
    {
	int i=0;
	boolean first=true;
	String ret ="";
	for(i=0; i < o.length; i++)
	    {
			if(first == false) 
				ret+=",";
			else
				first=false;
		ret += o[i];
	    }
	ret +="\n";
	return ret;
    }
    public void TelegraphCQSource(String hostname,
				  int port, 
				  String sourcename, 
				  String queryfilename,
				  Vector fileargs,
				  int frequency, 
				  int duration)
	throws Exception
    {
	int i=0;
	String sourcestring = computeSourceString(sourcename);
	System.out.println("sourcestring is:"+sourcestring);

	InetAddress hostaddr = InetAddress.getByName(hostname);
	File queryfile = new File(queryfilename);

	if(sourcestring == null) return;
	if(!queryfile.exists()) return;

	Socket sock = new Socket(hostaddr,port);
	DataOutputStream dos = new DataOutputStream(sock.getOutputStream());
	System.out.println("sending source string:"+sourcestring);
	dos.writeBytes(sourcestring);
	dos.flush();
	System.out.println("finished sending sourcename");
	mTessResults=null;
	while(duration > 0 && 
	      processFile(queryfile,fileargs) &&
	      mTessResults != null)
	    {
		System.out.println("ran the query.  There were " + mTessResults.size() + " results");
		for(i=0; i < mTessResults.size() ; i++)
		    {
			dos.writeUTF(objectArrayToCSV((Object [] )mTessResults.get(i)));
		    }
		dos.flush();
		System.out.println("going to sleep until next time");
		Thread.sleep(frequency);
		duration-=frequency;
		mTessResults=null;
	    }
	dos.close();
	sock.close();
	return;

    }

    String computeSourceString(String sourcename)
    {
	int length = sourcename.length()+1;

	if(length < 10)
	    {
		sourcename ="   "+length+sourcename;
	    }
	else if(length < 100) 
	    {
		sourcename="  "+length+sourcename;
	    }
	else if(length < 1000)
		{
		    sourcename=" "+length+sourcename;
		}
	else 
	    return null;
	return sourcename;
    }

    public HttpManager getManager() { return mManager;}
    public Vector getResults() { return mTessResults; }
    public CookieFilter getCookieFilter() { return mCookieFilter;}
    public URL getCurrentURL() { return mCurrentURL; }
    public String getCurrentPage() { return mCurrentPage.toString();}



    private HttpManager mManager=null;
    private CookieFilter mCookieFilter=null;
    private DebugFilter mDebugFilter=null;
    Vector mCurrentBindings=null;
    StringBuffer mCurrentPage;
    URL mCurrentURL;
    private java.util.HashMap mCurrentForms;
    private Vector mCommandHistory;
    Vector mTessResults;
    private boolean mPrint=false;
    private boolean mImageFilter=false;
    private JFrame mHtmlFrame;
    private JScrollPane mHtmlScrollableArea;
    private HTMLView  mHtmlArea;
    protected static boolean sUseReadline;
    private String mUserAgentName;
    private boolean mEncodePost=true;
    private ResultCrawler mResultCrawler=null;
    private String mHistoryfile=null;
}

class MyHTMLDocument extends HTMLDocument
{
    public MyHTMLDocument()
    {
	super();
    }
    public HTMLEditorKit.ParserCallback getReader(int pos) { return new MyHTMLReader(pos);}
    class MyHTMLReader extends HTMLReader
    {
	public MyHTMLReader(int offset)
	{
	    super(offset);
	}

	public void handleSimpleTag(HTML.Tag t, MutableAttributeSet a, int pos)
	{
	    //System.out.println("simple tag: " + t + ","+ a.toString() + "," +pos);
	    super.handleSimpleTag(t, a, pos);
	}
	public void handleStartTag(HTML.Tag t, MutableAttributeSet a, int pos)
	{
		
	    //	System.out.println("start tag: " + t + ","+ a.toString() + "," +pos);
	    super.handleStartTag(t, a, pos);
	}
	public void handleEndTag(HTML.Tag t, int pos)
	{
	    //	System.out.println("end tag: " + t +pos);
	    super.handleEndTag(t, pos);
	}
	public void handleError(String error, int pos)
	{
	    //System.out.println("error: " + error + "," + pos);
	    super.handleError(error, pos);
	}


	class FormAction extends HTMLReader.FormAction
	{
	    FormAction()
	    {
		super();
	    }
	    public void start(HTML.Tag t, MutableAttributeSet a)
	    {
		//System.out.println("MyFormAction start: "+ t+ ":" + a.toString());
		super.start(t,a);
	    }
	    public void end(HTML.Tag t)
	    {
		//System.out.println("MyFormAction end: "+ t);
		super.end(t);
	    }
			
	}
    }
}
class HTMLView extends JEditorPane
{
    public HTMLView(String text, URL base, HtmlGet h)
    {
	super();
	HTMLDocument htmldoc = new MyHTMLDocument();
	setEditorKit(new MyHTMLEditorKit(h));
	htmldoc.setBase(base);
	setDocument(htmldoc);
	setText(text);
	mHtmlGet=h;
    }

    public void setNewPage(String html, URL base)
    {
	HTMLDocument htmldoc = (HTMLDocument)getDocument();
	htmldoc.setBase(base);
	setText(html);
    }
    public void fireHyperlinkUpdate(HyperlinkEvent e) 
    {

	try
	    {
		if(e instanceof HTMLFrameHyperlinkEvent)
		    {
			HTMLFrameHyperlinkEvent hle =(HTMLFrameHyperlinkEvent)e;

		    }
		if(e.getEventType() == HyperlinkEvent.EventType.ACTIVATED)
		    {
			URL u = e.getURL();
			if(u != null)
			    {
				String urlstr = u.toExternalForm();
				u = new URL(mHtmlGet.getCurrentURL(), urlstr);
				urlstr = u.toExternalForm();

				mHtmlGet.parseAndProcessCommand("GET " + urlstr, false);
				mHtmlGet.parseAndProcessCommand("showpage", false);
			    }
		    }
	    }
	catch (Throwable t)
	    {
		System.out.println(" could not fillow link");
		t.printStackTrace();
	    }
				
    }

    HtmlGet mHtmlGet=null;
}

class MyHTMLEditorKit extends HTMLEditorKit
{
    public MyHTMLEditorKit(HtmlGet h) { super();
    mHtmlGet=h;}

    class MyHTMLFactory extends HTMLFactory
    {
	public MyHTMLFactory() { super(); }

	public View create(Element e)
	{
	    if(e.getName().equalsIgnoreCase("input"))
		{
		    return new MyFormView(e, mHtmlGet);
		}
	    if(e.getName().equalsIgnoreCase("select"))
		{
		    return new MyFormView(e, mHtmlGet);
		}
	    if(e.getName().equalsIgnoreCase("textarea"))
		{
		    return new MyFormView(e, mHtmlGet);
		}

	    return super.create(e);
	}
    }
	
    public ViewFactory getViewFactory() { return new MyHTMLFactory(); }
    private HtmlGet mHtmlGet=null;



} 

	

class MyFormView extends FormView
{
    public MyFormView(Element e, HtmlGet h)
    {
	super(e);
	String name="";
	String value="";
	String type="";
	AttributeSet form=null;
	String formName="";
	String formAction="";
	String formMethod="";

	mHtmlGet=h;
	AttributeSet elementAttributes = e.getAttributes();
	HTML.Tag t = (HTML.Tag)elementAttributes.getAttribute(StyleConstants.NameAttribute);
	Object model = elementAttributes.getAttribute(StyleConstants.ModelAttribute);
	Element formElement = getFormElement(e);
	type = (String)elementAttributes.getAttribute(HTML.Attribute.TYPE);
	name = (String)elementAttributes.getAttribute(HTML.Attribute.NAME);
	value =(String)elementAttributes.getAttribute(HTML.Attribute.VALUE);
	form  = (AttributeSet) ((formElement !=null) ? formElement.getAttributes() : null);
	if(model instanceof ComboBoxModel)
	    {
		Object selectedItem = ((ComboBoxModel)model).getSelectedItem();
		if(selectedItem != null)
		    {
			value = ((Option)selectedItem).getValue();
		    }


	    }
	if(type != null && type.equalsIgnoreCase("submit"))
	    name="submit";
						
	if(form != null)
	    {
		formName = (String)form.getAttribute(HTML.Attribute.NAME);
		if(formName==null) formName="unnamedForm";
		formAction = (String)form.getAttribute(HTML.Attribute.ACTION);
		formMethod = (String)form.getAttribute(HTML.Attribute.METHOD);
	    }

	try {
				 	
	    h.addFormInformation(formName, formAction, formMethod, name, value,model);
	}
	catch(Throwable tt)
	    {
		tt.printStackTrace();
	    }
    }
		

    public void submitData(String data)
    {
			
	AttributeSet form = getFormElement(getElement()).getAttributes();
	if(form == null)
	    {
		System.out.println(getElement());
		System.out.println(getElement().getAttributes());
		return;
	    }
	String name = (String)form.getAttribute(HTML.Attribute.NAME);
	String command = "noencodesubmitform " + (name==null?"unnamedForm":name) + "," + data;
	System.out.println(command);
	try 
	    {
		mHtmlGet.parseAndProcessCommand(command, false);
		mHtmlGet.parseAndProcessCommand("showpage", false);
	    }
	catch (Exception e)
	    {
		System.out.println("error running command");
	    }
			
    }
    private HtmlGet mHtmlGet=null;


    public Element getFormElement(Element e)
    {
	while(e != null)
	    {
		if(getTag(e) == HTML.Tag.FORM)
		    return e;
		else
		    e = e.getParentElement();
	    }
	return e;
    }

    public HTML.Tag getTag(Element e)
    {
	return (HTML.Tag)e.getAttributes().getAttribute(StyleConstants.NameAttribute);
    }
}

class Form
{
    Form(String name, String action, boolean isPost)
    {
	mEntries = new Hashtable();
	mIsPost=isPost;
	mAction=action;
	mFormName=name;
    }
    void setAction(String action)
    {
	mAction = action;
    }
    boolean getIsPost() { return mIsPost;}
    String getAction() { return mAction;}
    String getName() { return mFormName;}
    void  setIsPost(boolean b)
    {
	mIsPost=b;
    }
    void addProperty(String name, FormValue value)
    {
	mEntries.put(name, (value==null)? null: value);
    }

    Properties getProperties()
    {
	String key="";
	String value="";
	FormValue currentValue=null;
	Properties p = new Properties();
	for(Enumeration e = mEntries.keys(); e.hasMoreElements(); )
	    {
			
		key = (String)e.nextElement();
		currentValue=((FormValue)mEntries.get(key));
		value = currentValue.getValue();
		if(currentValue.getShouldOutput())
		    p.put(key, (value==null)?"":value);
	    }
	return p;
    }


    public String  printForm(boolean withOptions)
    {
	String ret = "";
	String optvalue="";
	String optname="";
	ret += "form name is: " +getName()+"\n";
	ret += "form method is: " +( getIsPost() ? "POST":"GET")+"\n";
	ret += "form action is: " + getAction() +"\n";



	for(Enumeration e = mEntries.keys(); e.hasMoreElements(); )
	    {
		String key =(String) e.nextElement();
		FormValue value = (FormValue)mEntries.get(key);
		ret += "key="+key+"="+value.getValue()+":"+value.getModel()+"\n";
		if(withOptions)
		    {
			if(value.getModel() instanceof ListModel)
			    {
				Object lmvalue=null;
				ListModel lm = (ListModel)value.getModel();
				ret += "key is a selection with options:\n";
				for(int lvidx=0; lvidx < lm.getSize(); lvidx++)
				    {
					lmvalue = lm.getElementAt(lvidx);
					if(lmvalue instanceof Option)
					    {
						optname = ((Option)lmvalue).getLabel();
						optvalue =((Option)lmvalue).getValue();
					    }
					ret += "optname=" +optname + ", optvalue=" + optvalue +"\n";
										
				    }
								
								
			    }
			else if(value.getModel() instanceof DefaultButtonModel)
			    {
				ButtonGroup g = ((DefaultButtonModel)value.getModel()).getGroup();
				if(g!=null)
				    {
					for(Enumeration buttons=g.getElements(); buttons.hasMoreElements();)
					    {
						AbstractButton ab = (AbstractButton)buttons.nextElement();
						System.out.println("button value:"+ab.getText()+"="+ab.getActionCommand());
					    }
				    }
			    }

		    }
	    }
	return ret;
    }
	
    private Hashtable  mEntries;
    private String mAction;
    private String mFormName;
    private boolean mIsPost;
}

class FormValue
{

    String getValue() {return mValue; }
    Object getModel() {return mModel;}
    boolean getShouldOutput() { return mShouldOutput;}
    private String mValue;
    private Object mModel;
    private boolean mShouldOutput;
    public FormValue(String value, Object model, boolean shouldOutput) 
    {
	mValue=value;
	mModel=model;
	mShouldOutput=shouldOutput;
    }
}




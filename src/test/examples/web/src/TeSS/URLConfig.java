package TeSS; 

import java.net.*;
import java.io.*;
import java.util.*;
import gnu.regexp.*;

import TeSS.util.*;
import TeSS.util.exceptions.*;
import HtmlGet.*;


/** A Site searcher class.  When given a site definition it will 
 *  go out and grab the site and then parse it based on that definition.
 *  See the documentation for more information about writting site defintions.
 *  <BR><BR>
 *  The fields that can be specified in a .jsc file are:<BR>
 *	host 	- 	The host you want to contact (i.e. www.yahoo.com)	<BR>
 *	url 	- 	the subdirectory on the host you want (i.e. /search.cgi)<BR>
 *	method 	-	get/post/questionmark (see documentation)<BR>
 *	arguments 	-	The arguments to the web page<BR>
 *	columns	-	The columns that this scan returns<BR>
 *	defaultx	-	The default value of argument x (1 indexed)<BR>
 *	prefix	-	The text that marks the start of the table<BR>
 *	termination	-	The text that marks the end of the table<BR>
 * termination_exclude - exclude the text in the termination from matches
 *  prefix_exclude - exclude the prefix text from matches
 *	rowprefix	- The text that marks the beginning of each row<BR>
 * 	- The text that marks the end of each row<BR>
 *	columnincludesignature - Include the signature for all columns<BR>
 *	ColName_prefix	- The text that marks the beginning of the column named ColName<BR>
 *	ColName_term	- The text that marks the end of the column named ColName
 *	ColName_transform -	A perlesque transform, s/a/b would replace all a's in ColName with b's<BR>
 *	ColName_includeprefix	- Include the prefix of this column in the result<BR>
 *	ColName_includeterm	- 	Include the term of this column in the result<BR>
 *	ColName_noconsume	-	Don't consume the text that was parsed to find this column.<BR>
 *	filter	-	Ignore any rows found that have the text specified in them<BR>
 *	bracket	-	If this is found at the beginning of a column it must also be at the end<BR>
 *	nodatatext 		-	If this is found somewhere on the page then do data should be returned<BR>
 *	strip 			-	true or false.  Should html and &x; type tags be stripped from the results.<BR>
 *  <BR><BR>
 *  @author <A HREF="mailto:nickl@db.cs.berkeley.edu">Nick Lanham</A><BR>
 *  @author rshankar
 */
public class URLConfig {
    
  /** Sets up all the needed info to scan the given site.
   *  @param configFile - The file you want this config to scan for directives */
  public URLConfig(File configFile) 
  throws GeneralTelegraphException
  {
	  try {
		  htmlget = new HtmlGet("TeSS", "TessHistory", null, false);
	  }
	  catch (Throwable t)
		  {
			  throw new GeneralTelegraphException("could not create HtmlGet");
		  }
    String inputLine;
    try {
      /* This whole first bit just sets up all the fields based on the
       * information in the site definition */
      FileInputStream f = new FileInputStream(configFile);
      in = new BufferedReader(new InputStreamReader(f));
      in.mark(5000);
      while((inputLine = in.readLine()) != null){
        processLine(inputLine);
      }
    }catch (Exception e) {
      e.printStackTrace();
      Debug.error(e + " : could not parse config file " + configFile);
    }
    if(host == null) {
       if (doDump) Debug.warn("host tag not specified");
    }
    if(url == null) {
       if (doDump) Debug.warn("url tag not specified!");
    }
    if(prefix == null) {
       if (doDump) Debug.warn("prefix tag not specified");
    }
    if(termination == null) {
       if (doDump) Debug.warn("termination tag not specified");
    }
    if(rowprefix == null) {
       if (doDump) Debug.warn("rowprefix tag not specified");
    }
    if(rowterm == null) {
       if (doDump) Debug.warn("rowtermination tag not specified");
    }

    try {
      in.reset();
      while((inputLine = in.readLine()) != null) {
        processCols(inputLine);
      }  
    }catch (Exception e) {
      e.printStackTrace();
      Debug.error("could not parse config file " + configFile);
    }

    if(incallrows) {
	for(int i = 0; i < includepre.length; i++)
	    includepre[i] = true;
    }
    
			

    if(colnames == null) {
      Debug.warn("columns tag not specified");
    }  

    colpos = 0;
    Debug.debugln("Parsed " + configFile + " successfully");
  }

  /** Process a line of text */
  private void processLine(String line) 
  throws REException, GeneralTelegraphException
  {
    if(line.equalsIgnoreCase("columnincludesignature")) {
	incallrows = true;
	return;
    }

    if(line.equalsIgnoreCase("rowincludesignature")) {
      rowincsig = true;
      return;
    }
    
    else if(line.equalsIgnoreCase("dump")) {
      System.out.println("dumping");
      doDump = true;
    }

    if(line.indexOf("=") < 0)
	return;
    String fname = line.substring(0, line.indexOf("="));
    fname = fname.trim();
    String val = line.substring(line.indexOf("=") + 1);
    val = val.trim();
    // Set up all the query parameters
      if(fname.equalsIgnoreCase("prefix exclude")) {
	excludeprefix =true;
      }

    switch(fname.charAt(0)) {
    case 'b': 
      if (fname.equalsIgnoreCase("bracket")) {
        bracket = val.charAt(0);
//        Debug.debugln("Bracket is " + bracket);
      }
      break;
    case 'f':
      if (fname.equalsIgnoreCase("filter")) {
        filterRX = new RE(val, RE.REG_MULTILINE | RE.REG_ICASE);
//        Debug.debugln("Filter is " + val);
      }
      break;
    case 'h': 
      if (fname.equalsIgnoreCase("host")) {
        host = val;
//        Debug.debugln("Host is " + host);
      }
      break;
    case 'n':
      if(fname.equalsIgnoreCase("nodatatext")) {
	nodatatext = new RE(val);
      }
      break;
    case 'u': 
      if(fname.equalsIgnoreCase("url")) { 
        url = val;
//        Debug.debugln("Url is " + url);
      }
      else if(fname.equalsIgnoreCase("urlclass"))
	urlClass = val;
      break;
    case 'p': 
      if(fname.equalsIgnoreCase("prefix") || fname.equalsIgnoreCase("prefix text")) {
	prefix = val;
        prefixRX = new RE(prefix, RE.REG_MULTILINE | RE.REG_ICASE);
      }
      break;
    case 'a':
      if(fname.equalsIgnoreCase("arguments")) {
	processArgs(val);
      }
      else if(fname.equalsIgnoreCase("auth")) {
	String user = val.substring(0,val.indexOf(":"));
	String pass = val.substring(val.indexOf(":")+1);
	Authenticator.setDefault(new URLAuthenticator(user,pass));
      }	
      else if (fname.equals("doNotAppendBindings")) {
        mAppendInputBindings = false;
      }
      break;
    case 's':
      if(fname.equalsIgnoreCase("strip")) {
	if(val.equalsIgnoreCase("false")) {
	   strip = 0;
        }
      }else if (fname.equalsIgnoreCase("script")) {
        script = val;
//        Debug.debugln("Script is " + script);
      }
      break;
    case 'd':
      if(args == null) {
	Debug.warn("Warning: Arguments must be specified before defaults");
      }
      else
        if((fname.substring(0,7)).equalsIgnoreCase("default")) {
          if(avals == null) {
            avals = new String[args.length];
            avals[Integer.parseInt(fname.substring(7)) - 1] = val;
            //		Debug.debugln("Adding default " + fname.substring(7,8) + " with value " + val);
          }
          else {
            avals[Integer.parseInt(fname.substring(7)) - 1] = val;
            //		Debug.debugln("Adding default " + fname.substring(7,8) + " with value " + val);
          }
        }
      break;
    case 't': 
      if (fname.equalsIgnoreCase("termination") || fname.equalsIgnoreCase("termination text")) {
        termination = val;
        terminationRX = new RE(termination, RE.REG_MULTILINE | RE.REG_ICASE);
//        Debug.debugln("Term is " + termination);
      }
      if (fname.equalsIgnoreCase("termination exclude")) {
        excludetermination = true;

      }
      break;
    case 'm':
      if (fname.equalsIgnoreCase("method")) {
        if(val.equalsIgnoreCase("post"))
          doPost=true;
        //System.out.println("M here, doPost is: " + doPost);
	if(val.equalsIgnoreCase("questionmark"))
	  doQuest=true;
	if(val.equalsIgnoreCase("none"))
	  doNone=true;
	if(val.equalsIgnoreCase("class"))
	  doClass=true;
      }
      break;
    case 'r':
      if(fname.equalsIgnoreCase("rowprefix") || fname.equalsIgnoreCase("row prefix")) {
	rowprefix = val;
        rowprefixRX = new RE(rowprefix, RE.REG_MULTILINE | RE.REG_ICASE);
//	Debug.debugln("Row Prefix is " + rowprefix);
      }
      else if(fname.equalsIgnoreCase("rowterm") || fname.equalsIgnoreCase("row termination")) {
	rowterm = val;
        rowtermRX = new RE(rowterm, RE.REG_MULTILINE | RE.REG_ICASE);
//	Debug.debugln("Row term is " + rowterm);
      }
      else if(fname.equalsIgnoreCase("requiredtext")) {
	try {
	  requiredText = new RE(val);
	}catch(Exception e){e.printStackTrace();}
      }
      else if(fname.equalsIgnoreCase("rowscanner")) 
	rowScanner = val;
      break;	
    }


  }
  
  /** Processes the arguments= tag */
  private void processArgs(String val) {
    StringTokenizer s = new StringTokenizer(val, ",");
    int i = 0;
    args = new String[s.countTokens()];
    while(s.hasMoreTokens())
      args[i++] = s.nextToken().trim();
    for(int j = 0; j < args.length; j++) {
//	Debug.debugln("Adding arg: " + args[j]);
    }
  }

  /** Process the columns */
  private void processCols(String line) throws Exception {
    //Debug.debugln("line is: " + line);
    //Debug.debugln(line.equals("end"));
    String fname, val = null;
    if(line.indexOf("=") > 0) {
	fname = line.substring(0, line.indexOf("="));
	val = line.substring(line.indexOf("=") + 1);
    }
    else
	fname = line;
    if(colnames != null) {
      if(colpos >= colnames.length)
	return;
      if(line.equalsIgnoreCase("end")) {
//	Debug.debugln("At an end");
	colpos++;
	in.reset();
      }
      if((fname.indexOf("_") < 0) && (fname.indexOf("includeprefix") < 0)) 
	  return;
      if(line.equalsIgnoreCase("rowincludesignature"))
	  return;
      String n = colnames[colpos];
      String name = fname.substring(0,fname.indexOf("_"));
      if(name.equalsIgnoreCase(n)) {
	String sp = fname.substring(fname.indexOf("_")+1);
	if(sp.equalsIgnoreCase("prefix") && colprefs[colpos] == null) {
	  colprefs[colpos] = val;
          colprefsRX[colpos] = new RE(val, RE.REG_MULTILINE | RE.REG_ICASE);
//	  Debug.debugln("Setting " + n + "'s prefix to " + val);
	  //if(colterms[colpos] != null)
	  //colpos++;
	  //in.reset();
	}
	else if(sp.equalsIgnoreCase("term") && (colterms[colpos] == null)) {
	  colterms[colpos] = val;
          coltermsRX[colpos] = new RE(val, RE.REG_MULTILINE | RE.REG_ICASE);
          coltermsBracketRX[colpos] = new RE(bracket + val, RE.REG_MULTILINE | RE.REG_ICASE);
//	  Debug.debugln("Setting " + n + "'s term to " + val);
	  //if(colprefs[colpos] != null)
	  //colpos++;
	  //in.reset();
	}
	else if(sp.equalsIgnoreCase("includeprefix")) {
//	    Debug.debugln("Setting an include prefix here");
	    includepre[colpos] = true;
	    //in.reset();
	}
	else if(sp.equalsIgnoreCase("includeterm")) {
	  includeterm[colpos] = true;
	}
	else if(sp.equalsIgnoreCase("transform")) {
	  String ret = val.substring(val.indexOf("/")+1,val.indexOf("/",val.indexOf("/")+1));
	  transforms[colpos] = new RE(ret, RE.REG_MULTILINE | RE.REG_ICASE);
//	  Debug.debugln("Setting " + n + "'s transform to " + ret);
	  String repto =
	    val.substring(val.indexOf("/",val.indexOf("/")+1)+1,val.lastIndexOf("/"));
	  //Debug.debugln(val.indexOf("/",val.indexOf("/")+1)+1);
//	  Debug.debugln("To be replaced with: " + repto);
	  repstrings[colpos] = repto;
	}
	else if(sp.equalsIgnoreCase("nullifmissing")) {
	  nullifmiss[colpos]=true;
	}
	else if(sp.equalsIgnoreCase("noconsume")) {
	  noconsume[colpos]=true;
	}
      }
    }	 
    else {
      if(fname.equalsIgnoreCase("columns")) {
	StringTokenizer st = new StringTokenizer(val, ",");
	numcols = st.countTokens();
	colnames = new String[numcols];
	colprefs = new String[numcols];
	colprefsRX = new RE[numcols];
	colterms = new String[numcols];
	coltermsRX = new RE[numcols];
	coltermsBracketRX = new RE[numcols];
	includepre = new boolean[numcols];
	includeterm = new boolean[numcols];
	transforms = new RE[numcols];
	repstrings = new String[numcols];
	nullifmiss = new boolean[numcols];
	noconsume = new boolean[numcols];
	//for(int i = 0; i < numcols;i++)
	//includepre[i] = false;
	for(int i = 0; i < numcols;i++) {
	    colnames[i] = st.nextToken().trim();
//	  Debug.debugln("adding column" + colnames[i]);
	}
      }
    }
  }  

  /* Fields */
  private BufferedReader in;
  private int colpos;
  private String host;

  /** Return the host this config will contact */
  public String getHost() { return host; }
	public boolean getExcludePrefix() { return excludeprefix;}

  /** The name of a custom script class to load, instead of URLScan. A value
   * of null signifies that we must use URLScan to parse the webpage. */
  private String script = null;
  public String getScript() {return script;}

  /** Return the url this config will go to */
  private String url;
  public String getURL() {return url;}
	public void setURL(String u) {url=u;}
  private String prefix;
  private RE prefixRX;
  private boolean excludeprefix=false;
  /** Return the table prefix of this config */
  public String getPrefix() {return prefix;}
  RE getPrefixRX() {return prefixRX;}
  private String termination;
  private RE terminationRX;
	private boolean excludetermination=false;
  /** Return the termination tag of this config */
  public String getTermination() {return termination;}
  RE getTerminationRX() {return terminationRX;}
	public boolean getExcludeTermination() { return excludetermination;}
  private String rowprefix;
  private RE rowprefixRX;
  /** Return the row prefix of this config */
  public String getRowPrefix() {return rowprefix;}
  RE getRowPrefixRX() {return rowprefixRX;}
  private String rowterm;
  private RE rowtermRX;
  /** Return the row terminator of this config */
  public String getRowTerminator() {return rowterm;}
  RE getRowTerminatorRX() {return rowtermRX;}

  private String[] colnames;
  /** Return the array of column names this config is using */
  public String[] getColNames() {return colnames;}
  /** Return the name of the column id */
  public String getColName(int id) {return colnames[id];}
  private String[] colprefs;
  /** Return the prefix for column id */
  public String getColPrefix(int id) {return colprefs[id];}
  private RE[] colprefsRX;
  /** Return the regular expresion prefix for column id
   *  @param id - the column you want the prefix for */
  RE getColPrefixRX(int id) {return colprefsRX[id];}
  private String[] colterms;
  /** Return the column terminator for column id */
  public String getColTerm(int id) {return colterms[id];}
  private RE[] coltermsRX;
  private RE[] coltermsBracketRX;
  /** Return the regular expresion terminator for column id
   *  @param id - the column you want the terminator for */
  RE getColTerminatorRX(int id) {return coltermsRX[id];}
  /** Return the regular expresion terminator with bracket for column id	
   *  @param id - the column you want the terminator for */
  RE getColTerminatorWithBracketRX(int id) {return coltermsBracketRX[id];}
  private int numcols;
  /** Return the number of columns this config has */
  public int getNumCols() {return numcols;}
  private int strip = 1;
  /** Is this config doing html/newline striping */
  public int getStrip() {return strip;}
  private String[] args;
  /** Return the array of arguments this config is using */
  public String[] getArgs() {return args;}
  /** Get the number of arguments this config has */
  public int getArgCount() {return args==null?0:args.length;}
  /** Get the array of default values for the arguments */
  private String[] avals;
  /** Return the default value for argument id
   *  @param id - the argument number you want the default value for */
  public String getDefaultVal(int id) {return avals[id];}
  private boolean incallrows = false;

  //For including column delims
  private boolean[] includepre;
  private boolean[] includeterm;

  /** Are we including the prefix for column id
   *  @param id - The column you want to check */
  public boolean getIncludePre(int id) {return includepre[id];}
  /** Are we including the term for column id	
   *  @param id - The column you want to check */
  public boolean getIncludeTerm(int id) {return includeterm[id];}

  private RE[] transforms;
  /** Get the transform for column id *
   *  @param id - the column you want the transfor for */
  RE getTransform(int id) {return transforms[id];}
  private String[] repstrings;
  /** Get the string we will use for transforming column id *
   *  @param id - The column you want the transform string for */
  public String getRepString(int id) {return repstrings[id];}
  private boolean doPost = false;
  /** Is this config posting data */
  public boolean doPost() {return doPost;}
  private boolean[] nullifmiss;
  /** Is the column id a nullifmissing column
   *  @param id - the column you want to check */
  public boolean getNullIfMissing(int id) {return nullifmiss[id];}
  private boolean rowincsig = false;
  /** Are we including the signature of rows */
  public boolean getRowIncSig() {return rowincsig;}
  private boolean doQuest = false;
  /** Are you doing a questionmark type query (see docs) */
  public boolean doQuest() {return doQuest;}
  private RE filterRX = null;
  /** Get the filter text for rows we don't want to include */
  RE getFilterRX() {return filterRX;}
  private char bracket = 22; 
  /** Get the bracket for this config (see docs) */
  public char getBracket() {return bracket;}  
  private RE nodatatext = null;
  /** Get the text that says this page has no data */
  public RE getNoDataText() {return nodatatext;}
  private RE requiredText = null;
  /** Get the text that this page MUST have to have data on it */
  public RE getRequiredText() {return requiredText;}
  private boolean doDump = false;
  /** Are we just doing an HTML dump */
  public boolean getDump() {return doDump;}
  /** Is the column id a noconsume column */
  private boolean[] noconsume;
  public boolean getNoConsume(int id) {return noconsume[id];}
  private boolean doNone = false;
  /** Do we want to do no url modification */
  public boolean doNone() {return doNone;}
  private boolean doClass = false;
  /** Will a class give us our url */
  public boolean doClass() {return doClass;}
  private String urlClass = null;
  /** What class will we use to form our url */
  public String getURLClass() {return urlClass;}
  /* Stuff to handle rowscanners */
  private String rowScanner = null;
	protected HtmlGet htmlget=null;
  public boolean useRowScanner() {return !(rowScanner==null);}
  public String getRowScanner() {return rowScanner;}
  /** A delimiter for columns if we want to use one.
      if not this will be null */
  private RE mDelim = null;
  public boolean doColDelim() {return !(mDelim==null);}
  public RE getColDelim() {return mDelim;}
  
  private boolean mAppendInputBindings = true;
  boolean appendInputBindings() {return mAppendInputBindings;}
}

class URLAuthenticator extends Authenticator {  
  String user;
  char[] pass;

  URLAuthenticator(String u,String p) {
    user = u;
    pass = p.toCharArray();
  }

  protected PasswordAuthentication getPasswordAuthentication() {
    return new PasswordAuthentication (user, pass);
  }
}


package TeSS;

import java.net.*;
import java.io.*;
import java.util.*;
import java.util.Arrays;
import gnu.regexp.*;

import TeSS.util.*;
import TeSS.util.exceptions.*;
//import telegraph.source.tupleReader.TupleReader;
//import telegraph.source.URLSourceView;
//import telegraph.source.Source;
//import telegraph.tuple.TupleIF;
//import telegraph.memory.TupleContainerIF;


/** A Site searcher class.  When given a site definition it will 
 *  go out and grab the site and then parse it based on that definition.
 *  See the documentation for information about writting site defintions.
 *  @author <A HREF="mailto:nickl@db.cs.berkeley.edu">Nick Lanham</A>
 *  @author rshankar
 */
public class NativeWrapper {
    
  /** 
   * This sets up all the needed info to scan the given site 
   * Starts a new scan on this web page with specified search arguments.
   * @param urlConfig Config information about the web page used to parse it
   * @param properties URL properties (name-value pairs)
   * @param cache Whether to cache read tuples
   */
  public NativeWrapper(URLConfig urlConfig, SQLProperties properties, 
                       boolean cache) 
  throws GeneralTelegraphException 
  {
    //super(source);
    mURLConfig = urlConfig;
    mProperties = properties;
    String scriptName = mURLConfig.getScript();
    AbstractURLScan urlScan;
    if (scriptName==null) {
      urlScan = new URLScan(urlConfig, properties);
    }else {
      //Debug.println("Trying a script");
      try {
        Class scriptc = Class.forName(scriptName);
        Class base = 
          Class.forName("TeSS.AbstractURLScan");
        if (!base.isAssignableFrom(scriptc)) {
          Debug.error("script class " + scriptName + 
                      " not subclass of AbstractURLScan");
        }
        Object[] cArgs = {urlConfig, properties};
        Class[] cParams = {
          Class.forName("TeSS.URLConfig"),
          Class.forName("TeSS.util.SQLProperties")};
        urlScan = 
          (AbstractURLScan) scriptc.getConstructor(cParams).newInstance(cArgs);
      }catch (Exception e) {
	e.printStackTrace();
        Debug.warn(e + " -- cannot load script, using URLScan");
	Debug.println("Script is: " + scriptName);
        urlScan = new URLScan(urlConfig, properties);
      }
    }
    mURLScan = urlScan;

    mCache = cache;
    mArgs = new String[properties.size()];
    String t = "args are ";
    int i=0;
    for (Iterator values=properties.getValues(); values.hasNext(); )
    {
      mArgs[i] = (String) values.next();
      t += mArgs[i] + " ";
      i++;
    }
    mDone = false;
    Debug.debugln("Args are: " + Arrays.asList(mArgs));
  }

  public Object[] getNextRowE() 
	  throws GeneralTelegraphException   {

      if(mDone) Debug.error("Already done with table");
      return mURLScan.getNextRow();

  }

  public Object[] getNextRow() {
    try {
      if(mDone) Debug.error("Already done with table");
      return mURLScan.getNextRow();
    }catch(Exception e) {e.printStackTrace();}
    return null;
  }


  /** Converts a nae-value url string "&amp;name=val..." into a Properties
   * list */
  public static SQLProperties getProperties(String args) {
    StringTokenizer t = new StringTokenizer(args, "&=");
    SQLProperties result = new SQLProperties();
    while (t.hasMoreTokens()) {
      result.addProperty(t.nextToken(), t.nextToken());
    }
    return result;
  }


  /** test routine */
  public static void main(String[] args) {
    try {
      URLConfig config = new URLConfig(new File(args[0]));
      int num = 0;
      SQLProperties arg = new SQLProperties();
      if(!(args.length > 1)) {
	String[] cargs = config.getArgs();
	for(int i = 0;i<config.getArgCount();i++) {
	  //System.out.println(config.getArgCount());
	  Debug.println("Getting default " + i + " with val: " + config.getDefaultVal(i));
          arg.addProperty(cargs[i], config.getDefaultVal(i));
	}
      }
      else {
	arg = NativeWrapper.getProperties(args[1]);
      }
      /*
      long startTime = System.currentTimeMillis();
      for (int i=0; i<1000; i++) {
        NativeWrapper scan = new NativeWrapper(config, arg, null, false);
        while (!scan.mDone) {
          Debug.println("OUTPUT " + num + " -- " + scan.getNextTuple());
          num++;
        }
        if (i%10==0) Debug.println(((System.currentTimeMillis() - startTime)/(num+0.0)) + 
              ":" + num);
      }
      */
      System.out.println("Args are: " + arg);
      NativeWrapper s = new NativeWrapper(config,arg,false);
      Object[] sr;
      long startTime = System.currentTimeMillis();
      while((sr = s.getNextRow()) != null) {
	System.out.println("Found: " + Arrays.asList(sr));
      }
      Debug.println("Time: " + (System.currentTimeMillis() - startTime));
    }catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }
  }

  /** Checks whether this reader has finished reading all tuples. */
  public final boolean isDone() { return mDone; }


  /** Whether to cache retrieved results */
  private final boolean mCache;

  /** The scanner on the website */
  private final AbstractURLScan mURLScan;

  /** Metadata to parse webpages */
  private final URLConfig mURLConfig;

  /** arguments passed in as bindings to the url source */
  private final String[]  mArgs;

  /** number of result tuples given so far */
  private int mNumOutputs = 0;

  /** Whether we have finished giving results */
  private boolean mDone;

  private final SQLProperties mProperties;

}



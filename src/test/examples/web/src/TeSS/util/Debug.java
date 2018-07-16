/*
 * Author: Steve Gribble <gribble@cs.berkeley.edu>
 * Inception Date: August 13th, 1999
 *
 * This software is copyrighted by Steven Gribble and the Regents of
 * the University of California.  The following terms apply to all
 * files associated with the software unless explicitly disclaimed in
 * individual files.
 * 
 * The authors hereby grant permission to use this software without
 * fee or royalty for any non-commercial purpose.  The authors also
 * grant permission to redistribute this software, provided this
 * copyright and a copy of this license (for reference) are retained
 * in all distributed copies.
 *
 * For commercial use of this software, contact the authors.
 * 
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABI LITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWA RE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HA VE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS , OR
 * MODIFICATIONS.
 */

package TeSS.util;

import java.io.*;
import java.util.*;
import TeSS.util.exceptions.*;

/**
 * Common set of debugging routines. Adapted from the Ninja debug code. 
 * The action taken upon a debug request (like
 * exiting, prinitng a message, logging a message) depends on 
 * two orthogonal things: the debug level associated with the request, and the debug
 * mode in which telegraph is run. 
 *
 * <p>Additionally we can specify "debug modules"
 * -- all debug messages from these modules will be printed out irrespecteve
 * of the telegaph debug mode. This allows module-level control of debug
 * messages. 
 *
 * @author rshankar
 */

public final class Debug {
  
    /** the log file */
    public static PrintWriter mLogStream = null;

    /* These are all of the possible debugging levels. Note that the action to be taken
     * for a debug message on a particular level will depend on the current mDebugMode */
    
    /** fatal error has occured (such as some system error). Telegraph must stop */
    public static final DebugLevel FATAL = new DebugLevel("FATAL", 60);

    /** some error has occurred (typically inferred within the code by violation of some
     * constraints */
    public static final DebugLevel ERROR = new DebugLevel("ERROR",   40);

    /** something has happened that needs to be noted and analyzed/fixed later -- this
     * will typically not be printed out but only logged */
    public static final DebugLevel WARN  = new DebugLevel("WARN",  30);
    
    /** an error in executing a particular query that <i>does not</i> 
     * compromise further execution of Telegraph */
    public static final DebugLevel QUERY_INITIALIZATION_ERROR = new DebugLevel("QUERY_INITIALIZATION_ERROR", 20);
    /** an error in executing a particular query that <i>does not</i> 
     * compromise further execution of Telegraph */
    public static final DebugLevel QUERY_PROCESSING_ERROR = new DebugLevel("QUERY_PROCESSING_ERROR", 20);
    
    /** any arbitrary message printed out for debugging/performance testing purposes */
    public static final DebugLevel DEBUG = new DebugLevel("DEBUG", 10);

    /** any arbitrary message printed out always, even in demo mode. e.g. Test Results */
    public static final DebugLevel PRINT = new DebugLevel("PRINT", 5);


  /** mode for avoiding all print statements */
  public static final int BENCHMARK_MODE = 8;

    /** debug mode: print all messages and exit, or go to debugger, on the first
     * fatal/error/warn level request */
    public static final int DEBUG_MODE = 4;
    
    /** normal execution mode: log warning messages, print out debug messages, exit on
     * fatal/error level debug request */
    public static final int NORMAL_MODE = 2;
    
    /** demo mode: avoid crashing as far as possible, and avoid dumping stuff on the
     * screen */
    public static final int DEMO_MODE = 0;
    
    /** the current telegraph debug mode */
    private static int mDebugMode = DEBUG_MODE;

    /** Set of (names of) debug modules. All debug messages from these
     * modules are printed out irrespective of the Telegraph debug mode. */
    private static final HashSet mDebugModules = new HashSet();
    
//    private static NinjaLinkedList debug_modules = new NinjaLinkedList();
    
    /** whether to print callee method names with debug requests */
    private static boolean show_method_names = true;
    
    /** whether to print callee package names with debug requests */
    private static boolean show_package_names = true;

  
    private static int tab = 0;

    public final static void incTab(int w) {
      tab += w;
    }

    public final static void decTab(int w) {
      tab -= w;
    }

    private final static void tabOver(PrintStream out) {

      /* Cheesy wat to tab over */

      for (int i = 0 ; i < tab; i++) {
        out.print(" ");
      } 
      
    }


    /** Adds the given module to the list of debug modules, {@link
     * #mDebugModules}. */
    public  static void addDebugModule(String moduleName) {
      mDebugModules.add(moduleName);
    }

    /** Sets the current telegraph debug mode to DEMO_MODE, NORMAL_MODE, or
     * DEBUG_MODE */
    public static void setDebugMode(int mode) {
      if (mode==DEMO_MODE || mode==NORMAL_MODE || mode==DEBUG_MODE) {
        mDebugMode = mode;
      }else {
        System.err.println("bad debug mode " + mode);
        System.exit(1);
      }
    }
    
    /**
     * set whether to prefix printed debug msgs 
     * with the calling method name
     */
    public static void showMethodNames(boolean b) {
        show_method_names = b;
    }


    /**
     * Set whether or not to display the entire package name, or just
     * the class
     **/
    public static void showPackageNames(boolean b) {
        show_package_names = b;
    }


    /**
     * generic debuging request, calls the appropriate routine depending on the debug level.
     * @param message The message/object to print
     * @param debugLevel The debugging level.
     **/
    public static void flag(Object message, DebugLevel debugLevel) 
    throws GeneralTelegraphException, QueryProcessingException, QueryInitializationException, FatalTelegraphException
    {
      if      (debugLevel.equals(PRINT))       print(message);
      else if (debugLevel.equals(DEBUG))       debug(message); 
      else if (debugLevel.equals(QUERY_PROCESSING_ERROR)) queryProcessingError(message);
      else if (debugLevel.equals(QUERY_INITIALIZATION_ERROR)) queryInitializationError(message);
      else if (debugLevel.equals(WARN))        warn(message);
      else if (debugLevel.equals(ERROR))       error(message);
      else if (debugLevel.equals(FATAL))       fatal(message);
      else throw new RuntimeException("bad debug mode");
    }

    
    /** request at level PRINT: print out message always */
    public static void print(Object message) {
//      String module = getParentName(true,false);
      tabOver(System.err);
      System.err.print(/* module + ": " + */ message);
    }

    
    /** request at level PRINT: print out message always with newline*/
    public static void println(Object message) {
//      String module = getParentName(true,false);
      tabOver(System.err);
      System.err.println(/* module + ": " + */ message);
    }

    
    /** Request at level DEBUG: prints out the message if
     * we are in DEBUG_MODE, or the parent (caller) module has been designated
     * as a debug module. */
    public static void debug(Object message) 
    {
      switch(mDebugMode) {
      case DEBUG_MODE: 
	tabOver(System.err);	  
	/*String module = getParentName(false,false);*/
	System.err.print(/*"DBG " + module + ": " + */ message);
	return;
      case NORMAL_MODE:
	String module = getParentName(false,false);
	if (mDebugModules.contains(module)) {
	  System.err.print(message);
	}
	return;
      case BENCHMARK_MODE:
	return;
      case DEMO_MODE:
	/* print no debug messages */
	return;
      default:
	throw new RuntimeException("bad debug mode" + mDebugMode);
      }
    }

    
    /** Request at level DEBUG: prints out the message if we are in
     * DEBUG_MODE, or the parent (caller) module has been designated as a
     * debug module. */
    public static void debugln(Object message) {
      /* code repetition with debug; done so as to preserve name of parent
       * module */
      switch(mDebugMode) {
        case DEBUG_MODE: 
	  tabOver(System.err);	  
          /*String module = getParentName(false,false);*/
          System.err.println(/*"DBG " + module + ": " + */ message);
          return;
        case NORMAL_MODE:
	  String module = getParentName(false,false);
	  if (mDebugModules.contains(module) || module.equals("TestHarness")) {
            System.err.println(message);
	  }
          return;
        case DEMO_MODE:
          /* print no debug messages */
          return;
        default:
          throw new RuntimeException("bad debug mode" + mDebugMode);
      }
    }

    
    /** print out message always. throw
     * QueryInitializationException unless DEMO_MODE */
    public static void queryInitializationError(Object message) 
    throws QueryInitializationException 
    {
      String module = getParentName(true,false);
      switch(mDebugMode) {
        case DEBUG_MODE: case NORMAL_MODE: 
          throw new QueryInitializationException(module + ": " + message);
        case DEMO_MODE:
	  tabOver(System.err);
          System.err.println("QUERY_INITIALIZATION_ERROR: " + module + 
                             ": " + message);
          return;
        default:
          throw new RuntimeException("bad debug mode" + mDebugMode);
      }
    }

    
    /** print out message always. throw
     * QueryProcessingException unless DEMO_MODE */
    public static void queryProcessingError(Object message) 
    throws QueryProcessingException 
    {
      String module = getParentName(true,false);
      switch(mDebugMode) {
        case DEBUG_MODE: case NORMAL_MODE: 
          throw new QueryProcessingException(module + ": " + message);
        case DEMO_MODE:
	  tabOver(System.err);	  
          System.err.println("QUERY_PROCESSING_ERROR: " + module + ": " + message);
          return;
        default:
          throw new RuntimeException("bad debug mode" + mDebugMode);
      }
    }

    public static void createLogWriter() {
        if(mLogStream == null) {
            try {
                FileOutputStream logStream = new FileOutputStream("TeleLog", true /*append*/);
                mLogStream = new PrintWriter(logStream);
            } catch (Exception e) {
                System.out.println("\nUNABLE TO OPEN LOG FILE TeleLog in Debug:log");
                return;
            }
        }
    }

    /** Log these messages in the special log file */
    public static void log(Object message) {
        String module = getParentName(true,false);
        Date cur = new Date();
        String mesg = "Log " + cur + ":" + module + ": " + message;
        System.err.println(mesg);
        createLogWriter();
        if(mLogStream == null)
            return;
        mLogStream.println(mesg);
        mLogStream.flush();
    }

    public static void log(Throwable t) {
        String module = getParentName(true,false);
        System.err.println("LOG: " + module + ": Stack Trace");
        createLogWriter();
        if(mLogStream == null)
            return;
        mLogStream.println("LOG: " + module + ": Stack Trace");
        t.printStackTrace(mLogStream);
        mLogStream.flush();
    }

    
    /** 
     * request at level WARN: log message in DEMO_MODE and NORMAL_MODE, 
     * print out message on screen in DEBUG_MODE 
     */
    public static void warn(Object message) 
    {
      String module = getParentName(true,false);
      switch(mDebugMode) {
        case DEBUG_MODE: case NORMAL_MODE:  
	  tabOver(System.err);	  
          System.err.println("**** WARN: " + module + ": " + message);
          return;
        case DEMO_MODE: 
          log("WARN: " + module + ": " + message);
          return;
        default:
          throw new RuntimeException("bad debug mode" + mDebugMode);
      }
    }

    
    /** 
     * request at level ERROR: simply  print message and throw
     * GeneralTelegraphException .
     */
    public static void error(Object message) 
    throws GeneralTelegraphException
    {
      String module = getParentName(true,false);
      switch(mDebugMode) {
        case NORMAL_MODE: case DEBUG_MODE: case DEMO_MODE:
	  tabOver(System.err);	  
//          System.err.println("ERROR: " + module + ": " + message);
          throw new GeneralTelegraphException(module + ": " + message);
        default:
          throw new RuntimeException("bad debug mode" + mDebugMode);
      }
    }

    
    /** request at level FATAL: always throw FatalTelegraphException
     */
    public static void fatal(Object message) 
    throws FatalTelegraphException
    {
      String module = getParentName(true,false);
      Debug.println("FATAL: " + module + ": " + message);
      throw new FatalTelegraphException(module + ": " + message);
    }

    
//    /**
//     * Print a debugging message, assuming the the module name
//     * is parent name.
//     *
//     * @param module The module that this message is in
//     * @param message The message/object to print
//     **/
//    public static void msg(DebugLevel level, Object message) {
//
//        String module = getParentName();
//        msg(module, level, message);
//    }
//
//    /**
//     * Print a debugging message, assuming debug level == DEBUG
//     *
//     * @param module The module that this message is in
//     * @param message The message/object to print
//     **/
//    public static void msg(String module, Object message) {
//        msg(module, DEBUG, message);
//    }
//  
//    /**
//     * Print a debugging message
//     *
//     * @param module The module that this message is in
//     * @param level The debug levels at which this message should be printed
//     * @param message The message to print
//     **/
//    public static void msg(String module, DebugLevel level, Object message) {
//        if (level == null) return;
//        if (level.level < min_required_level) return;
//        if ( !show_module(module,level.level) ) return;
//        //System.err.println(module+"."+level.name+ ":  " + message);
//        doprintln(module, level, message);
//    }
//  
//    /**
//     * Print a debugging message, and an associated exception.
//     *
//     * @param module The module that this message is in
//     * @param level The debug levels at which this message should be printed
//     * @param message The message to print
//     * @param t A Throwable to include in the message output
//     * @param traceLevel The debug level at which to show the stack
//     *  trace from the throwable.  
//     **/
//    public static void msg(String module, DebugLevel level, Object message, 
//                           Throwable t, DebugLevel traceLevel) {
//        if (level == null) return;
//        if (level.level < min_required_level) return;
//        if ( !show_module(module,level.level) ) return;
//
//        // if the exception is null, just print a normal message
//        if( t == null ) {
//            doPrint(module, level, message);
//            return;
//        }
//
//        // print the main message
//        doPrint(module, level, message + ": " + t);
//
//        // print the stack trace, if required
//        if (traceLevel != null && (show_module(module,traceLevel.level)))
//            t.printStackTrace(System.err);
//    }
//
//    /**
//     * Print the debugging message and the exception information.  The
//     * module name is determined automatically.
//     **/
//    public static void msg(DebugLevel level, Object message, 
//                           Throwable t, DebugLevel traceLevel) {
//        if (level == null) return;
//        if (level.level < min_required_level) return;
//        msg(getParentName(), level, message, t, traceLevel);
//    }
//
//
//
//    /**
//     * Add a module to the list of modules.  Only modules explicitly
//     * added will show debugging messages.  A '*' character at the end
//     * of the module name indicates that debug messages should be
//     * printed out for all modules that begin with this prefix.
//     *
//     * @param module The module to add
//     **/
//    public static void addModule(String module, DebugLevel level) {
//        // add the module name and level to the linked list
//        debug_modules.add_to_tail(module);
//        debug_modules.add_to_tail(level);
//
//        // update the max debug level
//        if( min_required_level > level.level) 
//            min_required_level = level.level;
//    }
//
//    /**
//     * Add a module to the list of modules, where the module name is
//     * inferred to be the calling Class's name.  This works well with
//     * the msg(String) version of msg() ....
//     * Again, only modules explicitly added will show debugging messages.  
//     **/
//    public static void addModule() {
//        String module = getParentName();
//        addModule(module, Debug.DEBUG);
//    }
//
//
//    /**
//     * Clear the list of debugging modules
//     **/
//    public static void clearModules() {
//        // clear out the linked list
//        while ( debug_modules.remove_head() != null ) {}
//    
//        // reset the debug mask
//        min_required_level = OFF.level;
//    }
//
//
//    /**
//     * A static initializer to read debugging configuration information
//     * from the Ninja config file.
//     **/
//    static {
//
//        // Add basic debugging for all modules
//        addModule("*",ERR);
//
//    }

  
//    ///////////////////////////////////////////////////////////////////////
//    // Internal Utility functions
//
//    /**
//     * Check to see if we should show messages for the given module
//     **/
//    private static boolean show_module(String module, int query_level) {
//        Enumeration enum = mDebugModules.elements();
//        String check;
//        DebugLevel level;
//
//        while( enum.hasMoreElements() ) {
//            check = (String) enum.nextElement();
//            level = (DebugLevel) enum.nextElement();
//
//            // keep looking, if the masks aren't compatible
//            if(query_level < level.level) continue;
//
//            // check to see if we have an exact match
//            if( module.equals(check) ) return true;
//
//            // check for an asterisk at the end of the module name, and do an
//            // initial substring match
//            if( check.endsWith("*") && 
//                ( check.length()==1 || 
//                  check.regionMatches(0,module,0,check.length()-1) ) )
//                return true;
//        }
//
//        // return false if we didn't find a match
//        return false;
//    }

    /**
     * Get name of calling Class (parent). 
     *
     * <p> Assumes a stack trace looks like:
     * <pre>
     * java.lang.Exception
     *    at WASDS.Debug.getParentName(Debug.java:268)
     *    at WASDS.Debug.addModule(Debug.java:146)
     *    at WASDS.WASim.<init>(WASim.java:16)
     *    at WASDS.WASim.main(WASim.java:89)
     * </pre>
     *
     * @param withMethodName whether to include the method name or not
     * @param qualifiedClassName whether to qualify the class name or not
     * @return the name of the parent
     **/
    public static String getParentName(boolean withMethodName,
                                        boolean qualifiedClassName)
    {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        Throwable t = new Throwable();

        t.fillInStackTrace();
        t.printStackTrace(pw);
        String s = sw.toString();
        sw.getBuffer().setLength(0);

        String myName = s.substring((s.indexOf("at ")+3),(s.indexOf("(")));
        myName = myName.substring(0,myName.lastIndexOf("."));
        //System.out.println("Debug: my Class name: " + myName);

        s = s.substring(s.lastIndexOf(myName)); // cuts out all "at "'s due to me
        int firstAt = s.indexOf("at ");
        int secondAt = s.indexOf("at ", firstAt+1);
        if (secondAt == -1) {
            s = s.substring(firstAt+3);
        } else {
            s = s.substring(firstAt+3,secondAt);
        }

        // Debug.println("Getting parent name as " + s);

        // trim out path and method call name
        s = s.substring(0, s.indexOf("("));

        String retVal = s.substring(0,s.lastIndexOf("."));
        if (!qualifiedClassName) {
          retVal = retVal.substring(retVal.lastIndexOf(".")+1);
        }
        if (withMethodName) {
            retVal += "." + s.substring(s.lastIndexOf(".")+1) + "";
            //System.out.println("Debug: Parent (module) name: " + s);
        } else {
            //System.out.println("Debug: Method name: " + s);
        }

        return retVal;
    }

    public static String getPrevParentName(boolean withMethodName,
                                        boolean qualifiedClassName)
    {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        Throwable t = new Throwable();

        t.fillInStackTrace();
        t.printStackTrace(pw);
        String s = sw.toString();
        sw.getBuffer().setLength(0);

        String myName = s.substring((s.indexOf("at ")+3),(s.indexOf("(")));
        myName = myName.substring(0,myName.lastIndexOf("."));
        //System.out.println("Debug: my Class name: " + myName);

        s = s.substring(s.lastIndexOf(myName)); // cuts out all "at "'s due to me
        int firstAt = s.indexOf("at ");
        int secondAt = s.indexOf("at ", firstAt+1);
	int thirdAt = s.indexOf("at ", secondAt+1);
        if (thirdAt == -1) {
            s = s.substring(secondAt+3);
        } else {
            s = s.substring(secondAt+3,thirdAt);
        }

	return s;

        // trim out path and method call name
//         s = s.substring(0, s.indexOf("("));

//         String retVal = s.substring(0,s.lastIndexOf("."));
//         if (!qualifiedClassName) {
//           retVal = retVal.substring(retVal.lastIndexOf(".")+1);
//         }
//         if (withMethodName) {
//             retVal += "." + s.substring(s.lastIndexOf(".")+1) + "";
//             //System.out.println("Debug: Parent (module) name: " + s);
//         } else {
//             //System.out.println("Debug: Method name: " + s);
//         }

//         return retVal;
    }

    /**
     * convenience routine -- get unqualified parent name without method call
     * is the default
     */
    private static String getParentName() {
        return getParentName(false,false);
    }

    /**
     * do actual printing
     **/
    private static void doPrint(String module, DebugLevel level, Object msg) {
        String head;

        // fetch the method name, if requested
        if (show_method_names) {
            head = getParentName(true, false);
        } else {
            head = module;
        }

        // remove extra package name info, if requested
        if (!show_package_names) {
            if(!show_method_names) {
                if(head.lastIndexOf('.') != -1)
                    head = head.substring(head.lastIndexOf('.'));
            } else {
                if(head.lastIndexOf('.') != -1)
                    head = head.substring( head.lastIndexOf('.', head.lastIndexOf('.')-1)+1 ) ;
            }
        }

	tabOver(System.err);	  
        System.err.println(head + "." + level.name + ": " + msg);
    }

    public static void finish() {
        if(mLogStream != null) {
            mLogStream.close();
        }
    }


    // Internal Utility functions
    ///////////////////////////////////////////////////////////////////////

}






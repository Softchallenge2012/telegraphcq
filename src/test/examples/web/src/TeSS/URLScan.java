package TeSS;

import java.net.*;
import java.io.*;
import java.util.*;
import java.util.Arrays;
import gnu.regexp.*;

import TeSS.util.*;
import TeSS.util.exceptions.*;


/** A Site searcher class.  When given a site definition it will 
 *  go out and grab the site and then parse it based on that definition.
 *  See the documentation for information about writting site defintions.
 *  @author <A HREF="mailto:nickl@db.cs.berkeley.edu">Nick Lanham</A>
 */
public class URLScan extends AbstractURLScan {
    
  /** Contructor. This sets up all the needed info to scan the given site 
   * Starts a new scan on this web page with specified search arguments.
   * @param urlConfig Config information about the web page used to parse it
   * @param properties URL properties (name-value pairs)
   */
	public URLScan(URLConfig urlConfig, SQLProperties properties)
                 
  throws GeneralTelegraphException 
  {
    super(urlConfig, properties);
    p = new PageBuffer(mReader, this);
    Thread t = new Thread(p);
    t.start();
  }

	public URLScan(URLConfig u, InputStream is)
	{
		super(u,is);
		p = new PageBuffer(mReader, this);
		Thread t = new Thread(p);
		t.start();
	}

  /** Lets you explicitly specify a url, overriding the host and url in the urlConfig */
  public URLScan(URLConfig urlConfig, SQLProperties properties, String url) 
  throws GeneralTelegraphException 
  {

    p = new PageBuffer(mReader, this);
    Thread t = new Thread(p);
    t.start();
  }


  /** Searches the given string buffer instead of the one fetched by the pagebuffer */
  public Object[] getNextRow(StringBuffer s) 
    throws GeneralTelegraphException {
    p.stop = true;
    mStringBuf = s;
    return getNextRow();
  }
  
  /** Gets the next row from the web page as an array of strings 
   * @return Array of strings of results, in order specified in mURLConfig */
  public Object[] getNextRow() 
  throws GeneralTelegraphException 
  {

    boolean foundreqtext = false;
    RE reqtext = mURLConfig.getRequiredText();
    if(reqtext == null)
      foundreqtext = true;

    if(mURLConfig.getDump()) {
      Debug.debugln("Can't get next row, I'm just dumping HTML");
      mDone = true;
      return null;
    }

    //System.out.println("Buffer so far: " + mStringBuf);
    if(mDone)
		{
			Debug.debugln("Already done with table.. returning null");
			return null;
		}
    try {
      colpos = 0;  // Need to start off at the beginning =)
      while(mStringBuf.length() < 1) {
	try {
	  synchronized(mStringBuf) {
		  if(readDone)
			  {
				  Debug.debugln("no data in string buffer, and read done == prefix text not found.  returning NULL");
				  return null;
			  }
	    //System.out.println("mSb length: " + mStringBuf.length());
	    if(mNoData) {
	      //System.out.println("Returning for no data");
	      mDone = true;
	      return null;
	    }
	    mStringBuf.wait(100);
	  }
	}catch(Exception e) {System.out.println("error waiting for data"); e.printStackTrace();}
      }

      if(mStringBuf == null) {
	mDone = true;
	Debug.debugln("I'm all the way through this data");
	return null;
      }

      if(colpos == mURLConfig.getColNames().length) {
	colpos = 0;
	return null;
      }

      RE rx;
      /*
      String ndt = mURLConfig.getNoDataText();
      if(ndt != null) {
	rx = null;
	try {
	  rx = new RE(ndt);
	}catch(Exception e) {e.printStackTrace();}
	REMatch m = rx.getMatch(mStringBuf);
	if(m != null) {
	  Debug.debugln("No data text found");
	  mDone = true;
	  return null;
	}
      }


       Don't go on until we find the text the user has said
	  is required for any data to be on the page 
      if(!foundreqtext) {
	REMatch m;
	while((m = reqtext.getMatch(mStringBuf)) == null) {
	  try {
	    synchronized(mStringBuf) {
	      mStringBuf.wait(100);
	    }
	  }catch(Exception e) {System.out.println("error waiting for data"); e.printStackTrace();}
	  if(readDone) {
	    Debug.debugln("Did not find required text, returning null");
	    mDone = true;
	    return null;
	  }
	}
      }
	

      if(inTable) {
	rx = mURLConfig.getTerminationRX();
	REMatch m = rx.getMatch(mStringBuf);
	if(m != null) {
	  int te = m.getStartIndex();
	  rx = mURLConfig.getRowPrefixRX();
	  m = rx.getMatch(mStringBuf);
	  if(m == null) {
	    mDone = true;
	    Debug.debugln("end of table " + mStringBuf + " term " + mURLConfig.getTermination());
	    return null;
	  }
	  int rp = m.getStartIndex();
	  if(te < rp) {
	    mDone = true;
	    Debug.debugln("end of table " + mStringBuf + " term " + mURLConfig.getTermination());
	    return null;
	  }
	}
      }

      if(!inTable) {
	rx = mURLConfig.getPrefixRX();
	REMatch m;
	/System.out.println("Trying first match on: " + n);
	boolean throughOnce = false;
	while((m = rx.getMatch(mStringBuf)) == null) {
	  try {
	    if(readDone) {
	      if(!throughOnce) {
		throughOnce = true;
		continue;
	      }
	      else {
		Debug.debugln("Error, can't find Table Prefix");
		return null;
	      }
	    }
	    synchronized(mStringBuf) {
	      mStringBuf.wait();
	      /System.out.println("Looking in: " + mStringBuf);
	    }
	  }catch(Exception e) {e.printStackTrace();}
	}  
	inTable = true;
	synchronized(mStringBuf) {
	  mStringBuf.delete(0,m.getEndIndex());
	}
	/Debug.println("In a table: " + mStringBuf);
      }*/

      rx = mURLConfig.getRowPrefixRX();
      //System.out.println("About to match on: " + rx);
      REMatch m = rx.getMatch(mStringBuf);
      //System.out.println("m is: " + m);
      while((m = rx.getMatch(mStringBuf)) == null) {
	  try {
	    if(readDone) {
	      if((m = rx.getMatch(mStringBuf)) == null) {
		Debug.debugln("Error, can't find rowstart\nSearching for: " + rx +"in:\n" + mStringBuf);
		return null;
	      }
	      else
		break;
	    }
	    synchronized(mStringBuf) {
	      mStringBuf.wait();
	    }
	  }catch(Exception e) {e.printStackTrace();}
      }
      //Debug.debugln("In a row: " + mStringBuf);
      int rx1;
      if(mURLConfig.getRowIncSig())
	rx1 = m.getStartIndex();
      else
	rx1 = m.getEndIndex();
      synchronized(mStringBuf) {
	mStringBuf.delete(0,rx1);
      }
      rx = mURLConfig.getRowTerminatorRX();
      while((m = rx.getMatch(mStringBuf)) == null) {
	try {
	  if(readDone) {
	    Debug.debugln("Error, can't find rowstart\nSearching for: " + rx +"in:\n" + mStringBuf);
	    return null;
	  }
	  synchronized(mStringBuf) {
	    mStringBuf.wait();
	  }
	}catch(Exception e) {e.printStackTrace();}
      }
      if(mURLConfig.getRowIncSig())
	rx1 = m.getEndIndex();
      else
	rx1 = m.getStartIndex();

      String rowString = null;
      
      synchronized(mStringBuf) {
	rowString = mStringBuf.substring(0,rx1);
	mStringBuf.delete(0,m.getEndIndex()); // Always want to strip off rowterm
      }
      
      //System.out.println("Found a row: " + rowString);
      //Debug.println("End Row");
      if (mURLConfig.getFilterRX()!=null &&
	  mURLConfig.getFilterRX().getMatch(rowString) != null) {
	//Debug.println("ignoring row " + wstring);
	return getNextRow();
      }else {
	return getCols(rowString);
      }  
    }
    catch (Exception e) {
      e.printStackTrace();
      Debug.error("Scan failure in " + this);
    }
    //Debug.debugln("\nDang!\n");
    return null;
  }


  /* Extract the columns from the given string.
   * This method is called from getNextRow and it
   * returns an array that represents the row */
  private Object[] getCols(String n) 
  throws GeneralTelegraphException, REException {

    String[] row = new String[mURLConfig.getNumCols()];   
    RE rx;
    
    while(colpos < mURLConfig.getColNames().length) {
      //Debug.debugln("Here is n: " + n);
      //Debug.debugln("");
      //Debug.debugln("Checking for prefix: " + colprefs[colpos]);
      //Debug.debugln("Checking for term: " + mURLConfig.getColTerminators()[colpos]);
      
      rx = mURLConfig.getColPrefixRX(colpos);
//      Debug.debugln("rx here is: " + rx + " - " + colpos);
      REMatch m = rx.getMatch(n);
      wstring = "";

      if(m != null) {
	inCol = true;
	int e;
        int end = m.getEndIndex();
	if(!mURLConfig.getIncludePre(colpos))
	    e = m.getEndIndex();
	else
	    e = m.getStartIndex();
        boolean bracketed =  (n.charAt(e) == mURLConfig.getBracket());
        if (bracketed) {
          rx = mURLConfig.getColTerminatorWithBracketRX(colpos);
        }else {
          rx = mURLConfig.getColTerminatorRX(colpos);
        }
//        Debug.println("Col " + colpos + ":" + mURLConfig.getColNames()[colpos] +
//                      " in " + n + " positions " + m.getStartIndex() + " to " +
//                      e + " -- " + m.getEndIndex() + " -- " + 
//                      bracketed + " -- " + rx);
	m = rx.getMatch(n, e);
//        Debug.println("Col " + colpos + ":" + mURLConfig.getColNames()[colpos] +
//                      " in " + n + " positions " + m.getStartIndex() + " to " +
//                      e + " -- " + bracketed + " -- " + rx);
	if(m == null || e > m.getStartIndex()) {
	  Debug.debugln("Start found but no term found for column " + colpos + 
                      ":" + mURLConfig.getColNames()[colpos] + " in " + n + 
                      " starting at position " + e);
	  Debug.error("Start found but no term found for column " + colpos + 
                      ":" + mURLConfig.getColNames()[colpos] + " in " + n + 
                      " starting at position " + e);
	}
	else {
          //Debug.println("wstring is " + wstring + " adding " + e + ":" + 
	  //            m.getStartIndex());
	  if(!mURLConfig.getIncludeTerm(colpos))
	      wstring += n.substring(e,m.getStartIndex());
	  else
	      wstring += n.substring(e,m.getEndIndex());
	  // Remove any newlines or html and such from the column
	  //System.out.println("HERE: "+wstring);
	  if(mURLConfig.getStrip() != 0) {
	    //System.out.println("Striping");
	    RE rep = new RE("\n");
	    wstring = rep.substituteAll(wstring, " ");
	    rep = new RE("<[^>]*>");
	    wstring = rep.substituteAll(wstring, "");
	    rep = new RE("&[^;]*;");
	    wstring = rep.substituteAll(wstring, "");
	    wstring = wstring.trim();
	    rep = mURLConfig.getTransform(colpos);
	    if(rep != null)
	      wstring = rep.substituteAll(wstring, mURLConfig.getRepString(colpos));
	  }
	  
	  row[colpos] = wstring;
	  //if(showInfo)
	  if (mURLConfig.getDump()) Debug.println("adding " + wstring + " to column " + colpos);
	  wstring = "";
	  if(!mURLConfig.getNoConsume(colpos))
	    n = n.substring(m.getEndIndex());
	  colpos++;
	  // n = n.substring(m.getStartIndex());  Do this is overlap is set
	}
	continue;
      }
      // This means the column was not found
      // Here I need to check for null if missing tag at some point
      if(mURLConfig.getNullIfMissing(colpos)) {
	row[colpos] = "NULL";
	  if(showInfo)
//	      Debug.debugln("adding null to column " + colpos);
	  wstring = "";
	  colpos++;
	  continue;
      }
      Debug.error("Column " + colpos + ":" + mURLConfig.getColNames()[colpos] +
                  " not found in " + n);
    }

    colpos=0;
    // Make sure we have a valid row
    int x = 0;
    for(int i = 0; i < row.length;i++) {
      if(row[i] == null && (!mURLConfig.getNullIfMissing(i)))
        x=1;
    }    
    if(x == 0) { 
      table.addElement(row);
      //Debug.println("Returning: " + Arrays.asList(row));
      return row;
    }
    else
	return getNextRow();
  }
  
  /** Print a text verion of the table we have for debuging */
  public void printTable() {
    Debug.println("");
    Debug.println("");
    Debug.println("Table:");
    for(int i = 0; i < mURLConfig.getColNames().length;i++)
	Debug.print(mURLConfig.getColNames()[i] + "         ");
    for(int j = 0; j < table.size(); j++) {
	Debug.println("");
	for(int i = 0; i < mURLConfig.getColNames().length;i++)
	    Debug.print(((String[])(table.elementAt(j)))[i] + "  ");
    }
    Debug.println("");
  }


  /** Some testing */
  public static void main(String[] args) {
    showInfo = true;
    try {
      URLConfig config = new URLConfig(new File(args[0]));
      int num = 0;
      long startTime = System.currentTimeMillis();
      for (int i=0; i<1; i++) {
	URLScan scan = new URLScan(config, NativeWrapper.getProperties(args[1]));
	while (!scan.mDone) {
	  Object[] t = scan.getNextRow();
	  if(t != null)
	      Debug.println(Arrays.asList(t));
          num++;
        }
        if (i%10==0) 
          Debug.println(
              ((System.currentTimeMillis() - startTime)/(num+0.0)) + 
              ":" + num);
      }
    }catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }
  }


  /** Return the table this page has */
  public Vector getTable() { return table; }

  /* Fields */

  private Vector table = new Vector();
  
  //private boolean inTable = false;
  private boolean inRow = false;
  private boolean inCol = false;
  private int colpos = 0;
  private String wstring = "";
  private int rpos = 0;
  static private boolean showInfo=false;
  private String restofrow = null;
  protected Vector rowis = new Vector();
  protected Vector colis = new Vector();
  private PageBuffer p;

}



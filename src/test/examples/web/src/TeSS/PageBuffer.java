package TeSS;
import java.io.BufferedReader;
import gnu.regexp.*;

/** A threadable class that, given a reader and a scan will 
    put lines into the scan's pagebuffer (A stringbuffer) as
    fast as it can */
class PageBuffer implements Runnable {

  BufferedReader mR;
  AbstractURLScan mS;
  boolean stop = false;

  final int BUF_SIZE=2048;

  PageBuffer(BufferedReader r, AbstractURLScan s) {
    mR = r;
    mS = s;
  }

  /** Fetch the page and put it into the stringbuffer that 
   *  the seacher is using */
  public void run() {
    int read;

    // The regexps that this class parses
    RE nodatatext = mS.mURLConfig.getNoDataText();
    RE prefix = mS.mURLConfig.getPrefixRX();
	boolean excludeprefix=mS.mURLConfig.getExcludePrefix();
//	System.out.println("excludeprefix is" + excludeprefix);
    RE term = mS.mURLConfig.getTerminationRX();
	boolean excludeterm=mS.mURLConfig.getExcludeTermination();
//	System.out.println("exclude term is " + excludeterm);
    StringBuffer lbuf = new StringBuffer(); // Local buffer to use until we get real data
    boolean inData = false;  // Set once we should start giving data to the more intensive parser
    REMatch match;
    char[] buf = new char[BUF_SIZE];

    try {
      //System.out.println("Reading:" + mS.mTarget);
      while((read = mR.read(buf,0,BUF_SIZE)) > 0) {
	if(stop) return;
	if(inData) { // If we are in real data
	  synchronized(mS.mStringBuf) {
	    mS.mStringBuf.append(buf,0,read);
	    match = term.getMatch(mS.mStringBuf);
	    if(match != null)  { // Found term
			int start=0;
			if(excludeterm)
			{
				start = match.getStartIndex();
//				System.out.println("excludeterm is true, deleting from=" + start);	
			}
			else
			{
				start = match.getEndIndex();

//				System.out.println("excludeterm is false, deleting from=" + start);	
			}
	      mS.mStringBuf.delete(start,mS.mStringBuf.length());
	      break;
	    }
	    //if(c == 10 || c == 13)
	    mS.mStringBuf.notify();
	  }
	}
	else {
	  lbuf.append(buf,0,read);
	  if(nodatatext != null) {
	    match = nodatatext.getMatch(lbuf);
	    if(match != null) { // Found no data text
	      // Perhaps need to set some other stuff
	      mS.mNoData = true;
	      break;
	    }
	  }
	  match = prefix.getMatch(lbuf);
	  if(match != null) { // Found the table prefix
	    inData = true;
		if(excludeprefix == false)
			{
				synchronized(mS.mStringBuf) {
					mS.mStringBuf.append(lbuf.substring(match.getStartIndex()));
				}
			}
	  }    
	}
      }
    } catch(Exception e) { 
      //Debug.println("Some problem fetching page");
      e.printStackTrace();
    }

    mS.readDone = true;
    try {
      mR.close();
    }catch(Exception e){e.printStackTrace();}
    synchronized(mS.mStringBuf) {
      mS.mStringBuf.notify();
      //System.out.println("Done fetching page\n");
    }
    /* Used for wraping purposes */
    if(mS.mURLConfig.getDump()) {
      //System.out.println("Fetched Page Dump:\n");
      System.out.println(mS.mStringBuf);
	}      
  }
}

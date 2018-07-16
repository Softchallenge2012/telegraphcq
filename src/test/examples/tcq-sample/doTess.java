import telegraph.source.tupleReader.URLScan.*;
import java.io.*;
import java.util.*;
import java.text.*;
public class doTess {
    /** One parameter -- the path to the TrafficUpdate.jsc file -- is required */
    public static void main(String[] args) {
	telegraph.util.Debug.setDebugMode(telegraph.util.Debug.DEMO_MODE);
	try {
	    URLConfig config = new URLConfig(new File(args[0]));
	    URLScan scan = new URLScan(config, NativeWrapper.getProperties("&"), Boolean.FALSE);
	    while (true) {
		Object[] t = scan.getNextRow();
		if(t != null) {
		    int i;
		    /* Hardcode knowledge of fields in traffic sensor stuff
		       0: incident number
		       1: time of day
		       2: url
		       3: description
		       4: location
		       5: area
		    */

		       
		    System.out.print(t[0] + ",");
		    /* we get day of week from url */
		    System.out.print(getDate((String)t[1],(String)t[2]) + ",");
		    System.out.print(t[4] + ",");
		    System.out.print(t[5] + ",");
		    System.out.print(t[3] + "\n");
		} else
		    break;
	    }
	}catch (Exception e) {
	    e.printStackTrace();
	    System.exit(1);
    }
	
    }

    static String getDate(String time, String url) {
	String result = "";
	String temp;
	String month, day;
	int timeInt;
	Date d;
	DateFormat df = DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.SHORT);

	//compute an integer which can be used to compare incident times
	temp = time.substring(0, time.length() - 2);
	if (temp.length() == 4) temp = "0" + temp;
	temp = temp.substring(0,2) + temp.substring(3,5);
	timeInt = new Integer(temp).intValue();
	if (time.substring(time.length() - 2).equals("PM")) timeInt += 1200;
	time = time.substring(0, time.length() - 2) + " " + time.substring(time.length() - 2);
	//last 4 digits are MMDD
	month = url.substring(url.length() -4, url.length() - 2);
	day = url.substring(url.length() - 2, url.length()); 
	timeInt += new Integer(day).intValue() * 10000;
	timeInt += new Integer(month).intValue() * 1000000;

	//url = url.substring(url.length() - 5, url.length() - 1);
	//System.out.println(month + "/" + day + "/00 " + time);
	try {
	    String year;
	    
	    //ick -- there's no year information in the logs, so assume
	    //its 2003, unless we're in december (since I'm writing this
	    //script in december 2002!)
	    if (month.equals("12")) year = "/02 ";
	    else year = "/03 ";
	    d = df.parse(month + "/" + day + year + time);
	    return  result + d.toString();
	} catch (ParseException e) {
	    System.out.println("failed parsing date");
	}
	return result;
    }

}

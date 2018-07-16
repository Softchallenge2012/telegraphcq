// CookieDate.java
// $Id: CookieDate.java,v 1.2 2003/07/17 05:19:11 sailesh Exp $
// (c) COPYRIGHT MIT and INRIA, 1998.
// Please first read the full copyright statement in file COPYRIGHT.html
package org.w3c.www.http;

import java.util.Date;

/**
 * @version $Revision: 1.2 $
 * @author  Benoît Mahé (bmahe@w3.org)
 */
public class CookieDate extends HttpDate {

    protected void updateByteValue() {
	// Compute the time zone offset, first call only.
	if ( tz == -1 ) {
	    tz = new Date().getTimezoneOffset();
	}
	// Dump the date, according to Cookie prefered format
	HttpBuffer buf = new HttpBuffer();
	Date d = new Date(date.longValue()+(tz*60*1000));
	buf.append(days[d.getDay()]);
	buf.append(','); buf.append(' ');
	buf.appendInt(d.getDate(), 2, (byte) '0');
	buf.append('-');
	buf.append(months[d.getMonth()]);
	buf.append('-');
	buf.appendInt(d.getYear()+1900, 2, (byte) '0');
	buf.append(' ');
	buf.appendInt(d.getHours(), 2, (byte) '0');
	buf.append(':');
	buf.appendInt(d.getMinutes(), 2, (byte) '0');
	buf.append(':');
	buf.appendInt(d.getSeconds(), 2, (byte) '0');
	buf.append(" GMT");
	raw  = buf.getByteCopy();
	roff = 0;
	rlen = raw.length;
    }

    public CookieDate(boolean isValid, long date) {
	super(isValid, date);
    }

}

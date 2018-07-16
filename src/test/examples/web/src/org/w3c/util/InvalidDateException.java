// InvalidDateException.java
// $Id: InvalidDateException.java,v 1.2 2003/07/17 05:19:10 sailesh Exp $
// (c) COPYRIGHT MIT, INRIA and Keio, 2000.
// Please first read the full copyright statement in file COPYRIGHT.html
package org.w3c.util;

/**
 * @version $Revision: 1.2 $
 * @author  Benoît Mahé (bmahe@w3.org)
 */
public class InvalidDateException extends Exception {

    public InvalidDateException(String msg) {
	super(msg);
    }
    
}

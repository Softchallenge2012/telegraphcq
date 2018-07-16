
package TeSS.util;

/**
 * copied ninja debug file because the bytearray code needs it.
 *
 * The DebugLevel class combines an object name with its debug level.
 * The actual debug levels are defined in Debug.java, and should not
 * be defined elsewhere.  The constructor is made protected, so that
 * classes outside the util package cannot create bogus debug levels.
 **/
public class DebugLevel extends Object { 
  public final String name;
  public final int level ;

  protected DebugLevel(String name, int level) {
    this.name = name;
    this.level = level;
  }
}

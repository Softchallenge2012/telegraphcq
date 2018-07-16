
package TeSS.util;
import TeSS.util.exceptions.*;

import java.util.*;
import java.io.IOException;
import java.io.Serializable;

/**
 *
 * A list of SQL properties consisting of (field name, value) pairs. We cannot
 * use java.util.Properties since it does not maintain any ordering. 
 *
 * @author rshankar
 */
public class SQLProperties implements Serializable {


  public ArrayList mNames;
  public ArrayList mValues;
  
  /** Appends a property with given field name and value */
  public void addProperty(String name, String value) {
    mNames.add(name);
    mValues.add(value);
  }


  /** Removes the specified property from this SQLProperties and returns it's value
      if the property existed or null otherwise.
      @param name - the name of the property to remove.
      @return The value of the removed property or null if no such property existed */
  public String removeProperty(String name) {
    int x;
    String s = null;
    if((x = mNames.indexOf(name)) > 0) {
      mNames.remove(x);
      s = (String)mValues.get(x);
      mValues.remove(x);
    }
    return s;
  }

  /** Dummy hashcode function: needed because of BUG in java.util.HashTable
   * and java.util.HashSet. On set1.contains(object2), they call
   * element.equals(object2) for some element of set1 only if the element
   * satisfies element.hashCode() == object2.hashCode(). This violates the jdk
   * 1.3 specification in java.util.Map
   */
  public int hashCode() {
    return mNames.hashCode() * 31 + mValues.hashCode();
  }

  public boolean equals(Object o) {
  //  Debug.debugln("checking equals this " + this + " o " + o);
    if (!(o instanceof SQLProperties)) return false;
    SQLProperties s = (SQLProperties) o;
    return mNames.containsAll(s.mNames) && mValues.containsAll(s.mValues) &&
           s.mNames.containsAll(mNames) && s.mValues.containsAll(mValues);
  }


  public String toString() {
    return mNames + " -- " + mValues;
  }
  
  /** Makes an empty property list */
  public SQLProperties() {
    mNames = new ArrayList();
    mValues = new ArrayList();
  }

  /** Constructs property list initialized to entries in given list */
  public SQLProperties(SQLProperties aprops) {
    mNames = new ArrayList(aprops.mNames);
    mValues = new ArrayList(aprops.mValues);
  }

  /** Gets total number of entries in this property list */
  public int size() {
    return mNames.size();
  }

  /** Gets a cursor over the field names in this list */
  public Iterator getNames() {
    return mNames.iterator();
  }
  /** Gets a cursor over the field values in this list */
  public Iterator getValues() {
    return mValues.iterator();
  }

  /** writes this list onto given output stream */
  private void writeObject(java.io.ObjectOutputStream out)
  throws IOException
  {
    out.writeObject(mNames);
    out.writeObject(mValues);
  }

  /** reads this list from given output stream */
  private void readObject(java.io.ObjectInputStream in)
  throws IOException, ClassNotFoundException 
  {
    try {
      mNames = (ArrayList) in.readObject();
      mValues = (ArrayList) in.readObject();
    }catch (ClassCastException e) {
      Debug.warn(e);
      throw new ClassNotFoundException(e.toString());
    }
  }

}

package TeSS.util;
import TeSS.util.exceptions.*;
//import telegraph.tuple.TupleIF;

import java.util.Comparator;
import java.util.Random;

/**
 *
 * A comparator that returns a random value as heresult of comparison. Used
 * for sorting data to randomize it.
 *
 * @author rshankar
 */
public final class RandomComparator implements Comparator {

  private static final Random mRandom = new Random();

  /** returns a random comparison value */
  public final int compare(Object o1, Object o2) {
    return ((mRandom.nextInt() & 0x01) == 0) ? 1 : -1;
  }

  public final boolean equals(Object obj) {
    return true;
  }

}


package TeSS.util.exceptions;

/** 
 * exception that is throw when there is a <i>fatal</i> error somewhere in Telegraph
 * but it cannot
 * be classified under any other exception category. For example, if there is an
 * exception somewhere in extracting a value from a field, the function may not know
 * if this excepion has arisen during query processing, quey initialization, or some
 * other task. It will throw this general exception (typically through
 * Debug.debug("",Debug.FATAL), and some ancestor function will decide, say, that this
 * is a query processing exception that needs to stop the query without bringing the
 * rest of Telegraph down.
 *
 * <b> Note:</b> these exceptions cannot be caught
 *
 * @see telegraph.util.Debug 
 */
public class FatalTelegraphException extends RuntimeException {
  public FatalTelegraphException() {
    super("FatalTelegraphException: No detail message given.");
  }

  public FatalTelegraphException(String s) {
    super(s);
  }
}

package TeSS.util.exceptions;

/** 
 * exception that is throw when there is a error somewhere in Telegraph 
 * but it cannot
 * be classified unde any other exception category. For example, if there is an
 * exception somewhere in extracting a value from a field, the function may not know
 * if this excepion has arisen during query processing, quey initialization, or some
 * other task. It will throw this general exception (typically through
 * Debug.debug("",Debug.ERROR), and some ancestor function will decide, say, that this
 * is a query processing exception that needs to stop the query without bringing the
 * rest of Telegraph down.
 *
 * @see telegraph.util.Debug 
 */
public class GeneralTelegraphException extends Exception {
  public GeneralTelegraphException() {
    super("GeneralTelegraphException: No detail message given.");
  }

  public GeneralTelegraphException(String s) {
    super(s);
  }
}

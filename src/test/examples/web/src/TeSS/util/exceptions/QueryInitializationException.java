package TeSS.util.exceptions;

/** 
 * exception that is throw when there is an error in parsing or optimizing a
 * query or creating modules for it or setting up queues or eddies for it
 */
public class QueryInitializationException extends Exception {
  public QueryInitializationException() {
    super("QueryInitializationException: No detail message given.");
  }

  public QueryInitializationException(String s) {
    super(s);
  }
}

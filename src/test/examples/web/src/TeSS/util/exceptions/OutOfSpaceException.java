package TeSS.util.exceptions;

/** 
 * exception that is thrown when there is no more room in a container.
 */

public class OutOfSpaceException extends Exception {

  public OutOfSpaceException() {
    super("QueryProcessingException: No detail message given.");
  }

  public OutOfSpaceException(String s) {
    super(s);
  }
}

package TeSS.util.exceptions;

/** 
 * exception that is throw when there is a error while processing a Telegraph query.
 * This must be used to stop the query without bringing the rest of Telegraph down.
 *
 */
public class SocketConnectionClosed extends GeneralTelegraphException {
  public SocketConnectionClosed() {
    super("QueryProcessingException: No detail message given.");
  }

  public SocketConnectionClosed(String s) {
    super(s);
  }
}

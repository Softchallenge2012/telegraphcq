package TeSS.util.exceptions;

/**
 *
 * Copied from ninja io core -- rshankar.
 **/
public class ObjectPoolClassException extends RuntimeException {
  private String msg;

  public ObjectPoolClassException(String m,Exception e) {
    msg = m + ": " + e.getMessage();
  }

  public ObjectPoolClassException(String m) {
    msg = m;
  }

  public String getMessage() {
    return msg;
  }

  public String toString() {
    return msg;
  }

}

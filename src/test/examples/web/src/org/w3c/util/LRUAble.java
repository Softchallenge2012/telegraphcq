// LRUAble.java
// $Id: LRUAble.java,v 1.2 2003/07/17 05:19:10 sailesh Exp $  
// (c) COPYRIGHT MIT and INRIA, 1997.
// Please first read the full copyright statement in file COPYRIGHT.html

package org.w3c.util ;

public interface LRUAble {
    public LRUAble getNext() ;
    public LRUAble getPrev() ;
    public void setNext(LRUAble next) ;
    public void setPrev(LRUAble prev) ;

}

package org.telegraphcq.webservices.utils;

public class MutableInt
{

    public MutableInt(int initialValue) { mValue = initialValue;}
    public int getValue() { return mValue; }
    public void setValue(int i) { mValue = i; }
    
    int mValue;
}

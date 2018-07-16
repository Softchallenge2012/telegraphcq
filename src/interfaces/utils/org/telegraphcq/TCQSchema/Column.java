package org.telegraphcq.TCQSchema;
import org.telegraphcq.exceptions.*;
public class Column
{
    int mColNo;
    String mColName;
    int  mColType;
    int mColScale;
    int mColPrecision;
    boolean mColIsInput;
    StreamReference mColStreamReference;

    public Column() {
    }
    public void setColNo(int i) { mColNo =i;}
    public void setColName(String name) { mColName=name;}
    public void setColSQLType(int  t) { mColType=t;}
    public void setColScale(int scale) { mColScale= scale;}
    public void setColPrecision(int precision) { mColPrecision = precision;}
    public void setColIsInput(boolean t) {mColIsInput=t;}
    public void setStreamReference(StreamReference r) { mColStreamReference=r;};
    public int getColNo() { return mColNo;}
    public String getColName() { return mColName;}
    public int  getColSQLType() { return mColType;}
    public int getColScale() { return mColScale;}
    public int getColPrecision() { return mColPrecision;}
    public boolean getColIsInput() {return mColIsInput;}
    public StreamReference getStreamReference() { return mColStreamReference;}

}
 

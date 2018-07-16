
package org.telegraphcq.TCQSchema;
import org.telegraphcq.exceptions.*;
public class StreamReference
{
    private Stream mFrom;
    private int mFromColno;
    private Stream mTo;
    private int mToColno;
    
    public StreamReference()
    {
	mFrom = null;
	mTo = null;
	mFromColno=-1;
	mToColno=-1;
    }
    public void setFromStream(Stream f)
    {
	mFrom=f;
    }
    public void setToStream(Stream t)
    {
	mTo = t;
    }
    public void setToColumn( int i) {mToColno=i;}
    public void setFromColumn (int i) {mFromColno=i;}
    public int getToColumn() { return mToColno;}
    public int getFromColumn() { return mFromColno;}
    public Stream getFromStream() {  return mFrom;}
    public Stream getToStream() { return mTo;}
}

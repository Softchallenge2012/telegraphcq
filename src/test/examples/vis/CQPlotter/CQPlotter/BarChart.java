package CQPlotter;

import ptolemy.plot.*;
import java.awt.*;
import javax.swing.*;
import java.util.*;
import java.awt.event.*;

public class BarChart extends Plot implements Runnable {

    
    boolean ready_to_exit = false;
    Thread t;

    public BarChart() {
	t = new Thread(this);
	//ToolTipManager ttm = ToolTipManager.sharedInstance();
	t.start();
	
	//setToolTipText("hi");
	//ttm.registerComponent(this);
	//addMouseMotionListener(ttm);
	
    }

    public void stop() {
	ready_to_exit = true;
    }

    public void run() {
	while (! ready_to_exit) {
	    try {
		Thread.currentThread().sleep(1000);
		expireOld();
	    } catch (InterruptedException e) {
	    }
	}
    }

    Integer lock = new Integer(0);

    Vector toDraw = new Vector();
    boolean didSchedule = false;
    boolean amPainting = false;
    boolean repaintAll = true;
    int i = 0;
    public void paint(Graphics g) {
	//System.out.println("IN PAINT, THREAD = " + Thread.currentThread() + "OBJECT = " + this);
	if (amPainting) {
	    return;
	}
	amPainting = true;
	boolean sched;
	if (!didSchedule || repaintAll) {
	    //	    System.out.println("super");
	    repaintAll = false;
	    super.paint(g);
	} else {
	    //  System.out.println("us, size = " + toDraw.size());
	    synchronized(lock) {
		Enumeration e = toDraw.elements();
		while (e.hasMoreElements()) {
		    //long start = System.currentTimeMillis();

		    Vector v = (Vector)e.nextElement();
		    _addPoint(((Integer)v.elementAt(0)).intValue(), 
			      (String)v.elementAt(1), 
			      ((Integer)v.elementAt(2)).intValue(), 
			      ((Double)v.elementAt(3)).doubleValue());
		    //try {
		    //long elapsed = System.currentTimeMillis() - start;
		    //Thread.currentThread().sleep(20 - elapsed);
		    //} catch (InterruptedException ex) {
		    //}
		}
		toDraw.clear();
	    }
	    didSchedule = false;
	}
	amPainting = false;
    }
    
    //add a point at the specified category
    public void addPoint(final int dataset, 
				       final String label, 
				       final int category, 
				       final double value) {
	//Runnable doAddPoint = new Runnable() {
	//public void run() {
		    
	//	    _addPoint(dataset, label, category, value);
	//	}
	//   };
	 checkSizes(dataset);
 	 Vector row = new Vector();
 	 row.addElement(new Integer(dataset));
 	 row.addElement(label);
 	 row.addElement(new Integer(category));
 	 row.addElement(new Double(value));

	 synchronized(lock) {
	     //some ugliness to ensure we don't enqueue too many
	     //update events

 	     if (toDraw.size() > 100 ) toDraw.remove(0);
	     toDraw.addElement(row);
	 }
	 if (!didSchedule) {
	     didSchedule = true;
	     repaint(50);
	 }
	 
	 Integer pointNo = (Integer)datasetCounts.elementAt(dataset);
	 if (category > pointNo.intValue()) 
	     datasetCounts.setElementAt(new Integer(category), dataset);


     }

    //add a point at the next available category
    public void addPoint(final int dataset,
				      final String label,
				      final double value) {
	checkSizes(dataset);
	Integer pointNo = (Integer)datasetCounts.elementAt(dataset);
	Integer labelLoc;
	if ((labelLoc = (Integer)labelMap.get(label)) != null)
	    pointNo = labelLoc;
	else {
	    datasetCounts.setElementAt(new Integer(pointNo.intValue() + 1), dataset);
	}
	labelMap.put(label,pointNo);
	addPoint(dataset, label, pointNo.intValue(), value);
    }

    public /*synchronized*/ void setBinOffset(double offset) {
        _binOffset = offset;
    }
    
    public void setBinWidth(double width) {
        _binWidth = width;
    }

    /** Set the amount of time before a column expires and is
	removed from the data set 
    */
    public void setExpiration(long expireMs) {
	_expireMs = expireMs;
    }

    
    /** Set the width and offset of the bars.  Both are specified
     *  as a fraction of a bin width.  The offset is the amount by which the
     *  i < sup>th</sup> data set is shifted to the right, so that it
     *  peeks out from behind the earlier data sets.
     *  @param width The width of the bars.
     *  @param offset The offset per data set.
     */
    public void setBars(double width, double offset) {
        _barwidth = width;
        _baroffset = offset;
    }

    protected /*synchronized*/ void _drawPlot(Graphics graphics,
					  boolean clearfirst) {
	setGrid(false);	 
	addXTick("", 0d);
        super._drawPlot(graphics, clearfirst);

        // We must call PlotBox._drawPlot() before calling doPlot
        // so that _xscale and _yscale are set.
	// super._drawPlot(graphics, clearfirst);

        _showing = true;

        // Plot the histograms in reverse order so that the first colors
        // appear on top.
        for (int dataset = datasets.size() - 1; dataset >= 0 ; dataset--) {
            Vector data = (Vector)datasets.elementAt(dataset);
	    for (int bin = 0; bin < data.size(); bin++) {
		PlotPoint pt = (PlotPoint)data.elementAt(bin);
		if (pt != null) {
		    //System.out.println("_drawPlot, bin = " + bin);
		    int count;
		    count = (int)((Double)pt.history.getLast()).doubleValue();
		    doPlot(graphics, dataset,
			   bin, count, (int)pt.maxValue);
		}
            }
        }
	
	
	  Vector[] xticks = getXTicks();
	  if (xticks != null && xticks.length == 2) {
	      
	      //    xticks[0].clear();
	      //    xticks[1].clear();
	  }
	

	
	Set labelCols = labelMap.entrySet();
	Iterator colIter = labelCols.iterator();
	int maxCol = -1;
	while (colIter.hasNext()) {
	    Map.Entry e = (Map.Entry)colIter.next();
	    
	    int col = ((Integer)e.getValue()).intValue();
	    //	    addXTick((String)e.getKey(), (double)col + .5);

 	    if (xticks != null && xticks.length == 2) {
 		while (xticks[1].size() <= col) {
 		    xticks[1].addElement(null);
 		    xticks[0].addElement(null);
 		}
 		xticks[1].setElementAt(e.getKey(),col);
 		xticks[0].setElementAt(new Double((double)col + .5), col);
		if (col > maxCol) maxCol = col;
 	    }

	    // System.out.println("col = " + col + ", LABEL : " + (String)e.getKey());
	}

	if (maxCol != -1) {
	    xticks[0].setSize(maxCol + 1);
	    xticks[1].setSize(maxCol + 1);
	}


    }

    private void checkSizes(int dataset) {
	
	while (dataset >= datasets.size()) {
	    datasets.addElement(new Vector());
	    datasetCounts.addElement(new Integer(0));
	}
    }


    private void expireOld() {
	if (_expireMs <= 0) return;
	Long pivot = new Long(System.currentTimeMillis() - _expireMs);
	SortedMap toRemove = expiryMap.headMap(pivot);
	Collection dels = toRemove.values();
	Iterator i = dels.iterator();
	int numLabels = labelMap.size();

	//toRemove now contains all the columns we need to remove
	// from the map

	while (i.hasNext()) {
	    Integer delCol = (Integer)i.next();
	    System.out.println("Expired : " + delCol);
	    int ic = delCol.intValue();
	    //need to remove this from all of the datasets and decremenet datasetCounts
	    for (int dataset = 0; dataset < datasets.size(); dataset++) {
		Vector ds = (Vector)datasets.elementAt(dataset);
		int count = ((Integer)datasetCounts.elementAt(dataset)).intValue();
		if (ds.size() > ic) {
		    ds.removeElementAt(ic);
		    datasetCounts.setElementAt(count > 0 ? (new Integer(count - 1)) : (new Integer(0)), dataset);
		}
	    }
	    // as well as the lastUpdates vector
	    lastUpdates.removeElementAt(ic);

	    //also need to shift up the labelMap
	    Set labelCols = labelMap.entrySet();
	    Iterator colIter = labelCols.iterator();
	    while (colIter.hasNext()) {
		Map.Entry e = (Map.Entry)colIter.next();
		int col = ((Integer)e.getValue()).intValue();
		if (col == ic) colIter.remove();
		if (col > ic) e.setValue(new Integer(col -1));
	    }
	 
	    //also need to shift the columns in the expiryMap 

	    Set expiryCols = expiryMap.entrySet();
	    Iterator expIter = expiryCols.iterator();
	    while (expIter.hasNext()) {
		Map.Entry e = (Map.Entry)expIter.next();
		int col = ((Integer)e.getValue()).intValue();
		if (col > ic) e.setValue(new Integer(col -1));
	    }

	}

	if (toRemove.size() > 0) {
	    //finally, remove all the expired stuff from the expiry map
	    //if (toRemove.size() != 0)
	    //    super.clear(true);
	    //for (int j = labelMap.size(); j < numLabels; j++) {
	    //    addXTick("", (double)j + .5);
	    //}
	    
	    //System.out.println("EXPIRING");
	    toRemove.clear();
	    repaintAll = true;
	    repaint(300);
	}
// 	Vector delVector = new Vector();
// 	for (int i = 0; i < v.size(); i++) {
// 	    PlotPoint pt = (PlotPoint)v.elementAt(i);
// 	    if (pt != null && System.currentTimeMillis() - pt.createTime > _expireMs)
// 		delVector.addElement(new Integer(i));
// 	}
// 	for (int i = 0; i < delVector.size(); i++) {
// 	    removeKey(dataset, i);
// 	}
    }
    
    private void _addPoint(int dataset, String label, int category, double value) {
	PlotPoint pt = new PlotPoint(label, value);
	Vector points;
	Long updateTime;
	Integer cat = new Integer(category);
	boolean newPt = false;

	checkSizes(dataset);
	points = (Vector)datasets.elementAt(dataset);

	
	while (category >= points.size())
	    points.addElement(null);
	
	if (points.elementAt(category) != null) {
	    PlotPoint old = (PlotPoint)points.elementAt(category);
	    old.merge(pt);
	} else {
	    points.setElementAt(pt, category);
	    newPt = true;
	}
	pt = (PlotPoint)points.elementAt(category);
	
	while (category >= lastUpdates.size()) 
	    lastUpdates.addElement(null);

	//update the expiryMap sorted list so that we
	//know what to expire
	if ((updateTime = (Long)lastUpdates.elementAt(category)) != null) {
	    expiryMap.remove(updateTime);
	}
	updateTime = new Long(System.currentTimeMillis());
	expiryMap.put(updateTime, cat);
	lastUpdates.setElementAt(updateTime, category);
	expireOld();
       
        // For auto-ranging, keep track of min and max.
        double x = category*_binWidth + _binOffset;
        if (x < _xBottom) _xBottom = x;
        double xtop = x + _binWidth/2.0;
        if (xtop > _xTop) _xTop = xtop;
        if (value > _yTop) _yTop = value;
        _yBottom = 0.0;

        // Draw the point on the screen only if the plot is showing.
        // Need to check that graphics is not null because plot may have
        // been dismissed.
        Graphics graphics = getGraphics();
	if (newPt) {
	    repaintAll = true;
	    repaint(50);
	}
        else if (_showing  && graphics != null) {
            // In swing, updates to showing graphics must be done in the
            // event thread, not here.  Thus, we have to queue the request.
            final int pendingDataset = dataset;
            final int pendingBin = category;
            final int pendingCount = (int)value;
	    final int pendingMax = (int)pt.maxValue;
	    final String theLabel = label;
            // We are in the event thread, so this is safe...
	    //addXTick(theLabel, (double)pendingBin + .5);
	    //System.out.println("doing " + pendingBin + " count = " + pendingCount);
	    doPlot(getGraphics(), pendingDataset, pendingBin,
		   pendingCount, pendingMax);

	    //doPlot(graphics, pendingDataset, pendingBin, pendingCount);
        } 
    }

    private void _clear(boolean format) {
	System.out.println("CLEAR");
        super.clear(format);
	setGrid(false);

	datasets = new Vector();
	datasetCounts = new Vector();
	labelMap = new Hashtable();
	_showing = false;
        if (format) {
            // Reset format controls
            _binWidth = 1.0;
            _binOffset = 0.5;
            _barwidth = 0.5;
            _baroffset = 0.15;

        }
    }

    /* Draw the specified histogram bar.
     * Note that paint() should be called before
     * calling this method so that it calls _drawPlot(), which sets
     * _xscale and _yscale. Note that this does not check the dataset
     * index.  It is up to the caller to do that.
     *
     * Note that this method is not synchronized, so the caller should be.
     * Moreover this method should always be called from the event thread
     * when being used to write to the screen.
     */
    private void doPlot(Graphics graphics,
            int dataset, int bin, int count, int maxCount) {
        double y = (double)count;
	double yMaxWin = (double)maxCount;
        double x = _binWidth*bin + _binOffset;

	//System.out.println("bin = " + bin + ", count = " + count);

        if (_xlog) {
            if (x <= 0.0) {
                System.err.println("Can't plot non-positive X values "+
                        "when the logarithmic X axis value is specified: " +
                        x);
                return;
            }
            x = Math.log(x)*_LOG10SCALE;
        }
        if (_ylog) {
            if (y <= 0.0) {
                System.err.println("Can't plot non-positive Y values "+
                        "when the logarithmic Y axis value is specified: " +
                        y);
                return;
            }
            y = Math.log(y)*_LOG10SCALE;
	    yMaxWin =  Math.log(yMaxWin)*_LOG10SCALE;
        }

        // Use long here because these numbers can be quite large
        // (when we are zoomed out a lot).
        long ypos = _lry - (long)((y - _yMin) * _yscale);
        long xpos = _ulx + (long)((x - _xMin) * _xscale);
	long ymax = _lry - (long)((_yTop - _yMin) * _yscale);
	long windowMax = _lry - (long)((yMaxWin - _yMin) * _yscale);
        // Set the color
        if (_usecolor) {
            int color = dataset % _colors.length;
            graphics.setColor(_colors[color]);
        } else {
            graphics.setColor(_foreground);
        }


        _drawBar(graphics, dataset, xpos, ypos, windowMax, ymax, true);
	

        // Restore the color, in case the box gets redrawn.
        graphics.setColor(_foreground);
    }


    /** Draw bar from the specified point to the y axis.
     *  If the specified point is below the y axis or outside the
     *  x range, do nothing.  If the <i>clip</i> argument is true,
     *  then do not draw above the y range.
     *  Note that paint() should be called before
     *  calling this method so that _xscale and _yscale are properly set.
     *  @param graphics The graphics context.
     *  @param dataset The index of the dataset.
     *  @param xpos The x position.
     *  @param ypos The y position.
     *  @param clip If true, then do not draw outside the range.
     */
    boolean painting = false;


    Hashtable offscreen_v = new Hashtable();

    protected void _drawBar(Graphics graphics, int dataset,
            long xpos, long ypos, long windowMax, long ymax, boolean clip) {


	long bot;

	//if (!painting) return;
	//xpos += 1;
	ymax += 1;
	
	if (ymax < _uly) {
	    repaintAll = true;
	    repaint(50); //rescale it all
	    return;
	}
	
        if (clip) {
            if (ypos < _uly) {
                ypos = _uly;
            } if (ypos > _lry) {
                ypos = _lry;
            }
        }
        if (ypos <= _lry && xpos <= _lrx && xpos >= _ulx) {
            // left x position of bar.
            int barlx = (int)(xpos - _barwidth *_binWidth * _xscale/2 +
                    dataset * _baroffset *_binWidth * _xscale);
            // right x position of bar
            int barrx = (int)(barlx + _barwidth *_binWidth * _xscale);
            if (barlx < _ulx) barlx = _ulx;
            if (barrx > _lrx) barrx = _lrx;
            // Make sure that a bar is always at least one pixel wide.
            if (barlx >= barrx) barrx = barlx+1;
            // The y position of the zero line.
            long zeroypos = _lry - (long) ((0-_yMin) * _yscale);
            if (_lry < zeroypos) zeroypos = _lry;
            if (_uly > zeroypos) zeroypos = _uly;

	    

	    //_drawBar(graphics,dataset,xpos, ymax - 1, true); 

	
            if (_yMin >= 0 || ypos <= zeroypos) {
		bot = ypos;
		
            } else {
		bot = zeroypos;
            }
	    Image offscreen = null;

	    try {
		offscreen = (Image)offscreen_v.get(new Integer(dataset));
	    } catch (NoSuchElementException e) {
		
	    }
	    if (offscreen == null ||
		offscreen.getWidth(null) != barrx-barlx ||
		offscreen.getHeight(null) != (int)zeroypos - (int) ymax) {
		//System.out.println("Draw bar");
		offscreen = createImage(barrx - barlx, Math.max(1,(int)zeroypos -  (int)ymax));
		offscreen_v.put(new Integer(dataset), offscreen);
	    }

	    Graphics offgr = offscreen.getGraphics();
	    
	    offgr.setColor(_background);
	    offgr.fillRect(0,0, offscreen.getWidth(null), offscreen.getHeight(null));
	    offgr.setColor(graphics.getColor());
			  
				      
	    //graphics.fillRect(barlx, (int)bot,
	    //barrx - barlx, (int)Math.abs(zeroypos - ypos));
	    offgr.fillRect(0,(int)(bot-ymax),barrx-barlx, (int)Math.abs(zeroypos - ypos));
	    //Color c = graphics.getColor();


	    
	    
	    //graphics.setColor(_background);
	    
	    //int clearBot = (int)ymax;
	    //int clearHgt = (int)bot - (int)ymax;
	    //System.out.println("clearBot = " + clearBot + ", clearHgt = " + clearHgt);
	    //graphics.fillRect(barlx, clearBot, barrx - barlx, clearHgt);
	    //off.fillRect(barlx, clearBot, barrx - barlx, clearHgt);
	    
	    
	    //paint the max
	    offgr.setColor(Color.black);

	    offgr.fillRect(0, (int)(windowMax - ymax), barrx - barlx, 2);
	    //graphics.setColor(c);

	    graphics.drawImage(offscreen, barlx,(int)ymax, null);
        }
    }



    public String getBarName(int x, int y) {
	int col;

	if (x < _ulx || x > _lrx) return null;
	if (y < _uly || y > _lry) return null;
	col = (int)(((x - _ulx )/_xscale) + _xMin);
	
	
	
	Set set = labelMap.entrySet();
	Iterator it = set.iterator();
	while (it.hasNext()) {
	    Map.Entry m= (Map.Entry)it.next();
	    if (((Integer)m.getValue()).intValue() == col)
		return (String)m.getKey();
	}
	return null;
    }
    
    public String getToolTipText(MouseEvent e) {
	String barName = getBarName(e.getX(), e.getY());
	if (barName == null) return null;

	int bar = ((Integer)labelMap.get(barName)).intValue();
	LinkedList h = ((PlotPoint)((Vector)(datasets.elementAt(0))).elementAt(bar)).history;
	int value = (int)(((Double)h.getLast()).doubleValue());

	barName += ": " + value;
	
	return barName;
    }

    public Point getToolTipLocation(MouseEvent e) {
	nextToLastEvent = lastEvent;
	lastEvent = e;
	
	return new Point(e.getX() - 80, 20);
    }
    
    static public long historySize = 1;


    private Vector datasets = new Vector();
    private Vector datasetCounts = new Vector();
    private double _binOffset = 0.5;
    private double _binWidth = 1;
    private boolean _showing = false;
    private Hashtable labelMap = new Hashtable();

    /** @serial The width of a bar. */
    private double _barwidth = 0.5;

    /** @serial The offset between bars. */
    private double _baroffset = 0.15;

    private long _expireMs = 0;
    private Vector lastUpdates = new Vector();
    private TreeMap expiryMap = new TreeMap();
    MouseEvent lastEvent = null;
    MouseEvent nextToLastEvent = null;
}

class PlotPoint {
    String label;
    double maxValue;
    LinkedList history = new LinkedList();

    
    public PlotPoint(String label, double value) {
	
	this.label = label;	
	this.maxValue = value;
	history.add(new Double(value));
    }

    public PlotPoint merge(PlotPoint pt) {
	if (pt == null) return this;
	
	Double newVald = (Double)pt.history.getLast();
	double newVal = newVald == null ? 
	    Double.MIN_VALUE:newVald.doubleValue();
	if (newVal >= maxValue) 
	    maxValue = newVal;
	
	if (history.size() == BarChart.historySize) {
	    Double d = (Double)history.removeFirst();
	    //pt.history.add(history.getLast());
	    //history = pt.history;
	    history.add(pt.history.getLast());
	    
	    if (d.doubleValue() == maxValue) {
		maxValue = getMax(history);
	    }
	} else {
	    history.add(pt.history.getLast());
	}	
	
	return this;
    }

    private double getMax(Collection h) {
	Iterator i = h.iterator();
	double max = Double.MIN_VALUE;

	while (i.hasNext()) {
	    double v = ((Double)i.next()).doubleValue();
	    if (v > max) max = v;
	}
	return max;
    }


}

package HtmlGet;

import java.util.*;
import java.net.*;
import gnu.regexp.*;

/* most results that are presented in a piece at a time call the same script
   over and over again to produce the next/previous result pages.  We'll 
   take advantage of this to automatically walk such results 
   
   links are added to the result if:
   1. the displayed link text matches the linkTextContains parameter
   OR
   2. the link is from the same path as the template URL, has a query string, 
      and the query matches qcontains and does not match qnotcontains.
*/
public class ResultCrawler
{
	public ResultCrawler(URL template, String qcontains, String qnotcontains,String linkTextContains) throws Exception
	{
		String urlstr = template.toExternalForm();
		int subs = urlstr.indexOf("?");
		if(subs>0) urlstr = urlstr.substring(0, subs);
		mURLTemplate = new URL(urlstr);
		mLinksToVisit=new HashSet();
		mVisitedLinks = new HashSet();
		mVisitedLinks.add(template);
		mQueryContainsRE = (qcontains==null?null:new RE(qcontains));
		mQueryNotContainsRE = (qnotcontains==null?null:new RE(qnotcontains));
		mLinkRE = new RE("<a\\s+href=\"*([^\">]+)\"*");
		mLinkTextRE = new RE("<a\\s+href=\"*([^\">]+)\"*[^>]+>([^<]+)</a>");
		mLinkTextContainsRE = (linkTextContains==null?null:new RE(linkTextContains));
	}

	public void processPage(String p) throws Exception
	{
		StringBuffer page = new StringBuffer(p);
		int linksAdded=0;
		String linkstr="";
		URL current;
		REMatch linkMatch=null;
		REMatch linkTargetMatch=null;
		REMatch [] matches=null;
		int idx=0;
		System.out.println("finding all links");
		if(mLinkTextContainsRE == null)
		    matches = mLinkRE.getAllMatches(page);
		else 
		    matches = mLinkTextRE.getAllMatches(page);

		System.out.println("found " + matches.length + " matches ");
		for(int matchidx=0; 
		    matchidx < matches.length;
		    matchidx++)
			{
			    linkMatch = matches[matchidx];
			    if(linkMatch == null) continue;
				linkstr=linkMatch.toString(1);
				current=null;
				try
					{
						current = new URL(mURLTemplate, linkstr);
						
					}
				catch (Exception e) {}
			
				boolean rejected=true;
				if(mLinkTextContainsRE != null)
				{
					String text = linkMatch.toString(2);
					if(mLinkTextContainsRE.getMatch(text) != null)
					rejected =false;
				}
				if(rejected && current !=null &&
				   current.getHost().equals(mURLTemplate.getHost()) &&
				   current.getPort() ==  mURLTemplate.getPort() &&
				   current.getPath().equals(mURLTemplate.getPath()))
					
							{
								rejected=false;
								String query = current.getQuery();
								if(query==null || query.length() == 0) rejected=true;
								if(mQueryContainsRE != null &&query !=null)
									{
										if(mQueryContainsRE.getMatch(query) == null)
											{
												rejected=true;
											}
									}
								if(mQueryNotContainsRE != null && query !=null)
									{
										if(mQueryNotContainsRE.getMatch(query) != null)
											{
												rejected=true;
											}
									}
							
								
								
							}
				if(rejected==false &&
				   !mLinksToVisit.contains(current) && 
				   mVisitedLinks.contains(current) == false)
				    {
					System.out.println("found a new link:"+current.toExternalForm());
					
										mLinksToVisit.add(current);
										linksAdded++;
									}
			
								idx = linkMatch.getEndIndex();
			}

		System.out.println("added " + linksAdded + "links");
	}

	public URL getNext() throws Exception
	{
		Iterator i = mLinksToVisit.iterator();
		URL ret=null;
		if(i.hasNext()) 
			{
				ret = (URL)i.next();
				i.remove();
				mVisitedLinks.add(ret);
				
			}
		System.out.println("links to visit=" + mLinksToVisit.size() + ", already visisted=" + mVisitedLinks.size());
		return ret;
	}
				

	private HashSet mVisitedLinks;
	private HashSet mLinksToVisit;
	RE mQueryContainsRE;
	RE mQueryNotContainsRE;
	RE mLinkTextContainsRE;
	RE mLinkRE;
	RE mLinkTextRE;
	private URL mURLTemplate;
}
		
		


create function wrappers.web_rescan(integer) returns boolean 
as 'libweb.so','web_rescan' language 'C';


		
create wrapper websource (init=csv_init,
			  next=csv_next, 
			  done=csv_done,
			  rescan=wrappers.web_rescan)
 with wrapperinfo 'BATCHSIZE=10,WRAPPERTYPE=PULL,HOST=localhost,PORT=2000,QUERY=file ../../WebFiles/quotes.hget,ERROR_INDICATOR=ERROR:';

drop wrapper mscsvwrapper;

create wrapper mscsvwrapper 
	(init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 


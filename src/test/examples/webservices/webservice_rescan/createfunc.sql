create function webservices_rescan(integer) returns boolean
as 'libwebservices.so','webservices_rescan' language 'C';

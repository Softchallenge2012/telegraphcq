create function hifi.glue_rescan(integer) returns boolean
as 'libglue.so','glue_rescan' language 'C';



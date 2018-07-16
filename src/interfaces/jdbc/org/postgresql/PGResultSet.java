package org.postgresql;


import java.util.Vector;


/** PG base rep of a Res Set.
 */
public interface PGResultSet
{

	/** The init method is what is used to put the data into a ResultSet.
	 */
	public void init (Field[] fields, Vector tuples, String status,
			  int update_count, long insert_oid, boolean binaryCursor);
}

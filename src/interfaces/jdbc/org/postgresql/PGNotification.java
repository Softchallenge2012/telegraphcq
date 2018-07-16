package org.postgresql;


/* $Header: /project/eecs/db/cvsroot/postgres/src/interfaces/jdbc/org/postgresql/PGNotification.java,v 1.2 2004/03/24 08:11:31 chungwu Exp $
 * This interface defines PostgreSQL extention for Notifications
 */
public interface PGNotification
{
	/*
	 * Returns name of this notification
	 */
	public String getName();

	/*
	 * Returns the process id of the backend process making this notification
	 */
	public int getPID();

}


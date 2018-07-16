/*
 *
 * This code is written by Lakshminarayanan Subramanian from the University of
 * California at Berkeley. This software was created during his internship
 * in Microsoft Research, Redmond under the mentorship of Venkata
 * Padmanabhan. This software is copyrighted by the authors, Microsoft
 * Research and the Regents of University of California at Berkeley.
 *
 * The following terms apply to all files associated with the software unless
 * explicitly disclaimed in individual files.
 *
 * The authors hereby grant permission to use this software without
 * fee or royalty for any non-commercial purpose.  The authors also
 * grant permission to redistribute this software, provided this
 * copyright and a copy of this license (for reference) are retained
 * in all distributed copies.
 *
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.	THIS SOFTWARE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 */


#define NUMRECORDS 2000

typedef struct
{

	char		dest[STRINGSIZE];
	char		trace_label[NUMHOPS][STRINGSIZE];
	char		location[NUMHOPS][STRINGSIZE];
	int			delay[NUMHOPS];
	char		networkname[NUMHOPS][STRINGSIZE];
	float		lat[NUMHOPS];
	float		lon[NUMHOPS];
	int			sourdist[NUMHOPS];
	int			sourlindist[NUMHOPS];
	int			numhops;
}	Tracelog_Record;

Tracelog_Record trrecord;
Tracelog_Record rec[NUMRECORDS];

int			numrecords;
int			numhops_record;
int			errcount_record;

void		initialize_trrecord();
int			is_blank(char str[STRINGSIZE]);
int			get_next_record(FILE *fp);
void		process_trrecord(char loc[STRINGSIZE]);
void		process_log(char logfile[STRINGSIZE], char outputfile[STRINGSIZE]);
void		process_trrecord_lindist(char source[STRINGSIZE], int *finalsourdist, int *finalsourlindist);
void		process_log_lindist(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE]);
void		process_log_hotpotato(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE]);
void		process_log_cityadjgraph(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE]);
void		process_log_cityclosuregraph(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE]);
void		process_log_routerlabels(char logfile[STRINGSIZE], char outputfile[STRINGSIZE]);
void		process_commonpath(char logfile1[STRINGSIZE], char logfile2[STRINGSIZE], char source1[STRINGSIZE], char source2[STRINGSIZE]);

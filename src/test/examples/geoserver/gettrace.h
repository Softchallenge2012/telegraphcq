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

#define INFINITE 100000
#define NEGINFINITE -100000
#define USEUROPE_DELAYDIFF 30

typedef struct
{
	char		label[STRINGSIZE];
	char		location[STRINGSIZE];
	char		networkname[STRINGSIZE];
	float		lat;
	float		lon;
	int			distsour;
	int			lindistsour;
	int			delay;
}	Trace_Info;

Trace_Info	trinfo[NUMHOPS];
int			numhops;

int			extract_delay(char del[STRINGSIZE]);
int			is_in_usa(char location[STRINGSIZE]);
void		delay_verify();
void		trace(char ipaddr[STRINGSIZE], char outputfile[STRINGSIZE]);
void		trace_finalloc(char ipaddr[STRINGSIZE], char loc[STRINGSIZE]);
void		trace_lindist(char ipaddr[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE]);

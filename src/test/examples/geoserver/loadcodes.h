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

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define STRINGSIZE 100
#define MAXCODES 10000
#define MAXNETWORKS 100
#define NUMPIECES 20
#define NUMSUBPIECES 20
#define NUMHOPS 40
#define CODES_PATH "codes/"
#define FILE_EXTN ".codes"
#define INFINITY 1000000

typedef struct
{
	char		code[STRINGSIZE];
	char		location[STRINGSIZE];
}	Code;

typedef struct
{
	int			isempty;
	char		network[STRINGSIZE];
	int			numcodes;
	Code		rcodes[MAXCODES];
}	RouterCodes;

int			numroutercodes;

typedef struct
{
	char		network[STRINGSIZE];
	char		codelist[MAXNETWORKS][STRINGSIZE];
	int			numcodes;
	int			xpiece[NUMPIECES];
	int			numxpieces;
	int			is_star;
}	Interface;

Interface	interface[MAXNETWORKS];
int			numnetworks;

RouterCodes net[MAXNETWORKS];

char		networklist[MAXNETWORKS][STRINGSIZE];

char		pieces[NUMPIECES][STRINGSIZE];
char		subpieces[NUMPIECES][NUMSUBPIECES][STRINGSIZE];
int			numpieces;
int			numsubpieces[NUMPIECES];


void		initialize_routercodes();
int			get_index(char network[STRINGSIZE]);
void		get_codelocation(char str[STRINGSIZE], char code[STRINGSIZE], char location[STRINGSIZE]);
void		initialize_network_list();
int			list_networks(char str[STRINGSIZE]);
void		load_allcodes(char interfacefile[STRINGSIZE]);
void		initialize_interface();
int			get_interface_index(char network[STRINGSIZE]);
void		load_interface(char interfacefile[STRINGSIZE]);

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

#include<math.h>

#define NUMCITIES 10000
#define RADIUS 6378.0
#define _180_BY_PI 57.2958

typedef struct
{

	char		cityname[STRINGSIZE];
	float		lat;
	float		longt;
}	LatLong;

LatLong		latlong[NUMCITIES];

int			numcities;

void		load_cities(char filename[STRINGSIZE]);
void		get_latlong(char cityname[STRINGSIZE], float *lat, float *longt);
int			get_distance(float lat1, float lat2, float lon1, float lon2);

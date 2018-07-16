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

/*
 * This piece of code calculates the geographic distance between two cities
 * specified in a database. The database is preseently stored in
 * "latlong/latlong.new". The geographic distance is measured along the great
 * circle.
 *
 */

#include "loadcodes.h"
#include "caldist.h"

void
load_cities(char filename[STRINGSIZE])
{

	FILE	   *fp;
	char		str[STRINGSIZE],
				str1[STRINGSIZE];
	float		f1,
				f2;

	fp = fopen(filename, "r");
	numcities = 0;

	while (fgets(str, STRINGSIZE, fp) != NULL && numcities < NUMCITIES)
	{
		sscanf(str, "%s %f %f\n", str1, &f1, &f2);
		strcpy(latlong[numcities].cityname, str1);
		latlong[numcities].lat = f1;
		latlong[numcities].longt = f2;
		numcities++;
	}

	if (numcities == NUMCITIES)
	{
		printf("Too many cities entered\n");
		exit(1);
	}

	fclose(fp);
	return;
}

void
get_latlong(char cityname[STRINGSIZE], float *lat, float *longt)
{

	int			i;

	for (i = 0; i < numcities; i++)
	{
		if (strcmp(cityname, latlong[i].cityname) == 0)
		{
			*lat = latlong[i].lat;
			*longt = latlong[i].longt;
			return;
		}
	}

	*lat = -1.0;
	*longt = -181.0;

	return;

}


int
get_distance(float lat1, float lat2, float lon1, float lon2)
{

	double		l1,
				l2;
	double		h1,
				h2;
	double		x,
				y,
				z,
				theta,
				dist;

	l1 = (double) lat1 / (double) _180_BY_PI;
	l2 = (double) lat2 / (double) _180_BY_PI;
	h1 = (double) lon1 / (double) _180_BY_PI;
	h2 = (double) lon2 / (double) _180_BY_PI;

	x = sin(l1) * sin(l2);
	y = cos(l1) * cos(l2) * cos(h2 - h1);
	z = x + y;

	theta = acos(z);

	dist = RADIUS * theta;

	return (int) dist;
}

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
 * This is the main piece of code that performs the pattern matching between
 * the loaction codes in the directory "codes" and the router label.
 * It uses the parsing rules specified in the file "interface"
 */

#include "loadcodes.h"
#include "parselabel.h"


int
is_caps(char a)
{

	if (a > 64 && a < 92)
		return 1;
	return 0;
}

char
caps(char a)
{

	if (!isalpha(a))
	{
		printf("Not a correct input char %c\n", a);
		exit(1);
	}
	if (is_caps(a))
		return a;
	else
		return (char) a - 32;
}


char
lower_case(char a)
{

	if (a > 64 && a < 92)
		return a + 32;
	return a;
}

void
convert_small(char place[STRINGSIZE], char place1[STRINGSIZE])
{

	int			i;

	place1[0] = '\0';

	i = 0;
	while (place[i] != '\0')
	{
		if (!isalpha(place[i]))
			place1[i] = place[i];
		else if (is_caps(place[i]))
			place1[i] = lower_case(place[i]);
		else
			place1[i] = place[i];
		i++;
	}
	place1[i] = '\0';
	return;
}


void
convert_properloc(char loc1[STRINGSIZE], char loc2[STRINGSIZE])
{

	int			i,
				j;
	int			bool1;

	i = 0;
	j = 0;
	bool1 = 0;

	while (loc1[i] != 0)
	{
		if (loc1[i] == ',')
		{
			loc2[j] = '+';
			j++;
			bool1 = 1;
		}
		else if (is_caps(loc1[i]) && !bool1 && i != 0)
		{
			loc2[j] = '+';
			j++;
			loc2[j] = loc1[i];
			j++;
		}
		else
		{
			loc2[j] = loc1[i];
			j++;
		}
		i++;
	}
	loc2[j] = 0;

	if (j != 0)
		loc2[0] = caps(loc2[0]);

	return;
}




void
initialize_pieces()
{

	int			i,
				j;

	for (i = 0; i < NUMPIECES; i++)
	{
		pieces[i][0] = 0;
		numsubpieces[i] = 0;
		for (j = 0; j < NUMSUBPIECES; j++)
			subpieces[i][j][0] = 0;
	}
	numpieces = 0;

	return;
}


void
cut_label(char label[STRINGSIZE])
{

	int			i,
				j,
				k,
				r;

	i = 0;
	j = 0;
	k = 0;

	while (label[i] != '\0' && label[i] != '\n')
	{
		if (label[i] == '.')
		{
			pieces[j][k] = 0;
			j++;
			k = 0;
		}
		else
		{
			pieces[j][k] = label[i];
			k++;
		}
		i++;
	}
	pieces[j][k] = 0;
	numpieces = j + 1;

	for (r = 0; r < numpieces; r++)
	{
		i = 0;
		j = 0;
		k = 0;
		while (pieces[r][i] != '\0' && pieces[r][i] != '\n')
		{
			if (pieces[r][i] == '-')
			{
				subpieces[r][j][k] = 0;
				j++;
				k = 0;
			}
			else
			{
				subpieces[r][j][k] = pieces[r][i];
				k++;
			}
			i++;
		}
		subpieces[r][j][k] = 0;
		numsubpieces[r] = j + 1;
	}

	return;
}


void
find_location(char label[STRINGSIZE], char network[STRINGSIZE], char location[STRINGSIZE])
{

	int			index;
	int			i;
	char	   *s,
			   *t;
	int bool   ,
				bool1;

	index = get_index(network);

	if (index == -1)
	{
		printf("Network %s does not exist\n", network);
		exit(1);
	}

	for (i = 0; i < net[index].numcodes; i++)
	{
		s = strstr(label, net[index].rcodes[i].code);
		if (s != NULL)
		{
			bool = 0;

			bool1 = 1;
			t = s + strlen(net[index].rcodes[i].code);
			if (strlen(s) != strlen(label))
			{
				bool = 1;

				s--;
			}
			if (bool)
			{
				if (!isdigit(*s) && (*s != '-') && (*s != '.'))
					bool1 = 0;
			}
			if ((*t != 0) && !isdigit(*t) && (*t != '-') && (*t != '.'))
				bool1 = 0;

			if (bool1)
			{
				strcpy(location, net[index].rcodes[i].location);
				return;
			}
		}
	}

	location[0] = 0;

	return;
}


void
get_network(char network[STRINGSIZE])
{

	int			i;

	i = numpieces - 2;

	if (i <= 0 || i > NUMPIECES)
	{
		network[0] = 0;
		return;
	}

	strcpy(network, pieces[i]);
	return;
}



int
get_actual_pos(int pos)
{

	if (pos == 0)
		return 0;
	if (pos > 0 && pos <= numpieces)
		return pos;
	if (pos > numpieces)
		return -1;
	if (pos < 0 && (numpieces + pos) > 0)
		return (numpieces + pos + 1);
	if (pos < 0 && (numpieces + pos) <= 0)
		return -1;

	return -1;
}



void
get_location(char rlabel[STRINGSIZE], char location[STRINGSIZE])
{

	int			intf_index;
	int			i,
				j,
				pos,
				actual_pos,
				k;
	char		net[STRINGSIZE];
	char		label[STRINGSIZE];
	char		loc[STRINGSIZE];

	convert_small(rlabel, label);
	initialize_pieces();
	cut_label(label);
	get_network(net);

	intf_index = get_interface_index(net);

	if (intf_index == -1)
	{
		printf("Interface file does not have any rest element\n");
		exit(1);
	}

	if (interface[intf_index].is_star == 0)
	{
		for (i = 0; i < interface[intf_index].numxpieces; i++)
		{
			pos = interface[intf_index].xpiece[i];
			actual_pos = get_actual_pos(pos);

			if (actual_pos == 0)
			{
				find_location(label, "whole", loc);
				convert_properloc(loc, location);
				if (location[0] != 0)
					return;
			}

			if (actual_pos == 1)
			{
				for (k = 0; k < numsubpieces[0]; k++)
				{
					for (j = 0; j < interface[intf_index].numcodes; j++)
					{
						find_location(subpieces[0][k], interface[intf_index].codelist[j], loc);
						convert_properloc(loc, location);
						if (location[0] != 0)
							return;
					}
				}
			}

			if (actual_pos > 1)
			{
				k = actual_pos - 1;
				for (j = 0; j < interface[intf_index].numcodes; j++)
				{
					find_location(pieces[k], interface[intf_index].codelist[j], loc);
					convert_properloc(loc, location);
					if (location[0] != 0)
						return;
				}
			}
		}
	}
	else
	{

		find_location(label, "whole", loc);
		convert_properloc(loc, location);
		if (location[0] != 0)
			return;

		for (i = 0; i < numpieces - 2; i++)
		{
			if (i == 0)
			{
				for (k = 0; k < numsubpieces[0]; k++)
				{
					for (j = 0; j < interface[intf_index].numcodes; j++)
					{
						find_location(subpieces[0][k], interface[intf_index].codelist[j], loc);
						convert_properloc(loc, location);
						if (location[0] != 0)
							return;
					}
				}
			}
			else
			{
				for (j = 0; j < interface[intf_index].numcodes; j++)
				{
					find_location(pieces[i], interface[intf_index].codelist[j], loc);
					convert_properloc(loc, location);
					if (location[0] != 0)
						return;
				}
			}
		}
	}

	location[0] = 0;

	return;
}

void
print_interface()
{

	int			i,
				j;

	for (i = 0; i < numnetworks; i++)
	{
		printf("network=%s\n", interface[i].network);
		for (j = 0; j < interface[i].numcodes; j++)
			printf("%s ", interface[i].codelist[j]);
		printf("\n");
		printf("isstar=%d \n ", interface[i].is_star);
		for (j = 0; j < interface[i].numxpieces; j++)
			printf("%d ", interface[i].xpiece[j]);
		printf("\n\n");
	}

	return;
}

/*
int main() {

  load_allcodes("interface");
  load_interface("interface");
  print_interface();
  return 1;
}
*/

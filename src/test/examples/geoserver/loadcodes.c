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

#include "loadcodes.h"

void
initialize_routercodes()
{

	int			i;

	numroutercodes = 0;

	for (i = 0; i < MAXNETWORKS; i++)
	{
		net[i].isempty = 1;
		net[i].network[0] = 0;
		net[i].numcodes = 0;
	}

	return;
}



int
create_entry(char network[STRINGSIZE])
{

	int			i;

	for (i = 0; i < MAXNETWORKS; i++)
	{
		if (net[i].isempty)
		{
			net[i].isempty = 0;
			strcpy(net[i].network, network);
			net[i].numcodes = 0;
			return i;
		}
	}
	numroutercodes = i + 1;

	return -1;
}


int
get_index(char network[STRINGSIZE])
{

	int			i;

	for (i = 0; i < MAXNETWORKS; i++)
	{
		if (strcmp(net[i].network, network) == 0)
			return i;
	}

	return -1;
}


int
add_codes(int index, char code[STRINGSIZE], char location[STRINGSIZE])
{

	int			i;

	i = net[index].numcodes;
	if (i == MAXCODES)
		return 0;
	strcpy(net[index].rcodes[i].location, location);
	strcpy(net[index].rcodes[i].code, code);
	net[index].numcodes++;

	return 1;
}


void
get_codelocation(char str[STRINGSIZE], char code[STRINGSIZE], char location[STRINGSIZE])
{

	int			i,
				j,
	bool;

	i = 0;
	bool = 0;

	j = 0;

	while (str[i] != '\0' && str[i] != '\n')
	{

		if (str[i] == '|')
		{
			bool = 1;

			code[j] = 0;
			j = 0;
		}
		else
		{
			if (bool)
			{
				location[j] = str[i];
				j++;
			}
			else
			{
				code[j] = str[i];
				j++;
			}
		}
		i++;
	}
	location[j] = 0;

	return;
}


void
load_network_codes(char network[STRINGSIZE])
{

	char		str[STRINGSIZE],
				str1[STRINGSIZE];
	FILE	   *fp;
	char		code[STRINGSIZE],
				location[STRINGSIZE];
	int			index,
				k;

	strcpy(str, CODES_PATH);
	strcat(str, network);
	strcat(str, FILE_EXTN);
	index = get_index(network);

	if (index == -1)
	{
		printf("No entry has been created for the network %s \n", network);
		exit(1);
	}

	fp = fopen(str, "r");

	while (fgets(str1, STRINGSIZE, fp) != NULL)
	{
		get_codelocation(str1, code, location);
		k = add_codes(index, code, location);
		if (k == 0)
		{
			printf("Too many codes in %s.codes\n", network);
			exit(1);
		}
	}
	fclose(fp);

	return;
}



void
initialize_network_list()
{

	int			i;

	for (i = 0; i < MAXNETWORKS; i++)
		networklist[i][0] = 0;

	return;
}



int
list_networks(char str[STRINGSIZE])
{

	int			i,
				j,
				k;
	char		str1[STRINGSIZE],
				str2[STRINGSIZE],
				str3[STRINGSIZE];

	i = 0;
	j = 0;
	k = 0;
	sscanf(str, "%s %s %s\n", str1, str2, str3);

	while (str2[i] != '\0' && str2[i] != '\n')
	{
		if (str2[i] == ',')
		{
			networklist[j][k] = 0;
			j++;
			if (j == MAXNETWORKS)
			{
				printf("Too many networks entered %s\n", str2);
				exit(1);
			}
			k = 0;
		}
		else
		{
			networklist[j][k] = str2[i];
			k++;
		}
		i++;
	}

	networklist[j][k] = 0;

	return j + 1;
}


void
load_allcodes(char interfacefile[STRINGSIZE])
{

	FILE	   *fp;
	int			count,
				index,
				i,
				k;
	char		str[STRINGSIZE];

	initialize_routercodes();

	fp = fopen(interfacefile, "r");

	while (fgets(str, STRINGSIZE, fp) != NULL)
	{
		initialize_network_list();
		count = list_networks(str) + 1;
		for (i = 0; i < count; i++)
		{
			index = get_index(networklist[i]);
			if (index == -1)
			{
				k = create_entry(networklist[i]);
				load_network_codes(networklist[i]);
			}
		}
	}

	return;
}



void
initialize_interface()
{

	int			i,
				j;

	for (i = 0; i < MAXNETWORKS; i++)
	{
		interface[i].network[0] = 0;
		interface[i].numcodes = 0;
		interface[i].numxpieces = 0;
		for (j = 0; j < MAXNETWORKS; j++)
			interface[i].codelist[j][0] = 0;
		for (j = 0; j < NUMPIECES; j++)
			interface[i].xpiece[j] = 0;
	}

	return;
}

int
get_interface_index(char network[STRINGSIZE])
{

	int			i;

	for (i = 0; i < numnetworks; i++)
	{
		if (strcmp(interface[i].network, network) == 0)
			return i;
	}

	for (i = 0; i < numnetworks; i++)
	{
		if (strcmp(interface[i].network, "general") == 0)
			return i;
	}

	return -1;
}



void
load_interface(char interfacefile[STRINGSIZE])
{

	FILE	   *fp;
	char		str[STRINGSIZE];
	int			i,
				j,
				k,
				pos;
	char		str1[STRINGSIZE],
				str2[STRINGSIZE],
				str3[STRINGSIZE];
	char		dummystr[STRINGSIZE];

	fp = fopen(interfacefile, "r");

	pos = 0;
	while (fgets(str, STRINGSIZE, fp) != NULL)
	{
		sscanf(str, "%s %s %s\n", str1, str2, str3);
		strcpy(interface[pos].network, str1);
		i = 0;
		j = 0;
		k = 0;
		while (str2[i] != '\0' && str2[i] != '\n')
		{
			if (str2[i] == ',')
			{
				interface[pos].codelist[j][k] = 0;
				j++;
				if (j == MAXNETWORKS)
				{
					printf("Too many networks entered %s\n", str2);
					exit(1);
				}
				k = 0;
			}
			else
			{
				interface[pos].codelist[j][k] = str2[i];
				k++;
			}
			i++;
		}
		interface[pos].codelist[j][k] = 0;
		interface[pos].numcodes = j + 1;

		i = 0;
		j = 0;
		k = 0;

		if (str3[0] == '*')
		{
			interface[pos].is_star = 1;
			interface[pos].numxpieces = 0;
		}
		else
		{
			interface[pos].is_star = 0;
			while (str3[i] != '\0' && str3[i] != '\n')
			{
				if (str3[i] == ',')
				{
					dummystr[k] = 0;
					interface[pos].xpiece[j] = atoi(dummystr);
					j++;
					if (j == NUMPIECES)
					{
						printf("Too many pieces entered %s\n", str3);
						exit(1);
					}
					k = 0;
				}
				else
				{
					dummystr[k] = str3[i];
					k++;
				}
				i++;
			}
			dummystr[k] = 0;
			interface[pos].xpiece[j] = atoi(dummystr);
			interface[pos].numxpieces = j + 1;
		}
		pos++;
		if (pos == MAXNETWORKS)
		{
			printf("Too many entires in the Interfaec file\n");
			exit(1);
		}
	}

	numnetworks = pos;

	return;
}

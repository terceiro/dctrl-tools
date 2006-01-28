/*  dctrl-tools - Debian control file inspection tools
    Copyright (C) 1999, 2003 Antti-Juhani Kaijanaho

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <ctype.h>
#include "strutil.h"

const char * left_trimmed(const char * s)
{
	const char * p;

	for (p = s; *p != 0 && isspace (*p); p++);

	return p;
}

void trim_right (char * s) 
{
	char * p;
	char * herep;
	enum { BLANKS, NONBLANKS } state = NONBLANKS;
	
	for (herep = 0, p = s; *p != 0; p++) {
		switch (state) {
		case BLANKS:
			if (!isspace (*p)) {
				state = NONBLANKS;
			}
			break;
		case NONBLANKS:
			if (isspace (*p)) {
				herep = p;
				state = BLANKS;
			}
		}
	}
	if (state == BLANKS) *herep = 0;
}

#ifdef TESTMAIN

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
printout (const char * s)
{
  printf("\"%s\"\n", s);
}

int
main (void)
{
  static const char * teststrings [] = { 
    "autoksiko",
    " autoksiko",
    "autoksiko ",
    " autoksiko ",
    "autoksiko           teen",
    "         autoksiko  teen",
    "autoksiko teen          ",
    "      autoksiko teen    ",
    " miljoona miljoona ruusua ",
    0 };
  int i;

  for (i = 0; teststrings [i] != 0; i++)
    {
      char * ts;
      char * orig_ts;

      orig_ts = ts = strdup (teststrings [i]);
      if (ts == 0) {
	      fail();
      }

      printf ("Plain: ");
      printout (ts);

      printf ("Left-trimmed: ");
      printout (left_trimmed (ts));

      printf ("Right-trimmed: ");
      trim_right (ts);
      printout (ts);

      printf ("Completely trimmed: ");
      printout (left_trimmed (ts));

      puts ("");
      free (orig_ts);
    }
  return 0;
}
#endif /* TESTMAIN */
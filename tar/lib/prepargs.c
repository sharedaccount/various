/* Parse arguments from a string and prepend them to an argv.
   Copyright 1999-2001, 2007, 2013 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Paul Eggert <eggert@twinsun.com>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "prepargs.h"
#include <sys/types.h>
#include <xalloc.h>

#if HAVE_STRING_H
# include <string.h>
#endif

#include <ctype.h>

/* Find the white-space-separated options specified by OPTIONS, and
   using BUF to store copies of these options, set ARGV[0], ARGV[1],
   etc. to the option copies.  Return the number N of options found.
   Do not set ARGV[N].  If ARGV is null, do not store ARGV[0]
   etc.  Backslash can be used to escape whitespace (and backslashes).  */
static int
prepend_args (char const *options, char *buf, char **argv)
{
  char const *o = options;
  char *b = buf;
  int n = 0;

  for (;;)
    {
      while (isspace ((unsigned char) *o))
	o++;
      if (!*o)
	return n;
      if (argv)
	argv[n] = b;
      n++;

      do
	if ((*b++ = *o++) == '\\' && *o)
	  b[-1] = *o++;
      while (*o && ! isspace ((unsigned char) *o));

      *b++ = '\0';
    }
}

/* Prepend the whitespace-separated options in OPTIONS to the argument
   vector of a main program with argument count *PARGC and argument
   vector *PARGV.  */
void
prepend_default_options (char const *options, int *pargc, char ***pargv)
{
  if (options)
    {
      char *buf = xmalloc (strlen (options) + 1);
      int prepended = prepend_args (options, buf, (char **) 0);
      int argc = *pargc;
      char * const *argv = *pargv;
      char **pp = (char **) xmalloc ((prepended + argc + 1) * sizeof *pp);
      *pargc = prepended + argc;
      *pargv = pp;
      *pp++ = *argv++;
      pp += prepend_args (options, buf, pp);
      while ((*pp++ = *argv++))
	continue;
    }
}

.TH GREP-DCTRL 1 2004-01-03 "Debian Project" "Debian user's manual"
\" Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004
\"               Antti-Juhani Kaijanaho <gaia@iki.fi>
\" Permission is granted to make and distribute verbatim copies of
\" this manual provided the copyright notice and this permission notice
\" are preserved on all copies.
\" 
\" Permission is granted to copy and distribute modified versions of this
\" manual under the conditions for verbatim copying, provided that the
\" entire resulting derived work is distributed under the terms of a
\" permission notice identical to this one.
\" 
\" Permission is granted to copy and distribute translations of this
\" manual into another language, under the above conditions for modified
\" versions, except that this permission notice may be included in
\" translations approved by the authors of this manual instead of in
\" the original English.
\" 
.SH NAME
grep-dctrl, grep-status, grep-available \- grep Debian control files
.SH SYNOPSIS
.B grep-dctrl
[options] predicate
[
.IR file " ..."
]
.sp
.B grep-status
[options] predicate
[
.IR file " ..."
]
.sp
.B grep-available
[options] predicate
[
.IR file " ..."
]
.sp
.B grep-dctrl
--copying | --help | --version | -ChV
.sp
.B grep-status
--copying | --help | --version | -ChV
.sp
.B grep-available
--copying | --help | --version | -ChV
.SH DESCRIPTION
The grep-dctrl program can answer such questions as 
.IR "What is the Debian package foo?" , 
.IR "Which version of the Debian package bar is now current?" ,
.IR "Which Debian packages does John Doe maintain?" ,
.I  "Which Debian packages are somehow related to the Scheme"
.IR " programming language?" ,
and with some help,
.IR "Who maintain the essential packages of a Debian system?" ,
given a useful input file.
.PP
It is a specialised grep program that is meant for processing any file
which has the general format of a Debian package control file, as
described in the Debian Packaging Manual.  These include the dpkg
available file, the dpkg status file, and the Packages files on a
distribution medium (such as a Debian CD-ROM or an FTP site carrying
Debian).
.PP
You must give a search predicate on the command line.  The predicate
defines which kind of paragraphs (aka package records) are output.  An
atomic predicate is a search pattern along with any options that
modify it.  Possible modifiers are --eregex, --field, --ignore-case,
-P, --regex and --exact-match, along with their single-letter
equivalents.  An atomic predicate gives a pattern to search for.  By
default, the search is a case-sensitive fixed substring match on each
paragraph (in other words, package record) in the input.  With
suitable modifiers, this can be changed: the search can be
case-insensitive and the pattern can be seen as an extended POSIX
regular expression.  Predicates can be combined using the connectives
--and, --or and --not.  Predicates can be grouped using parentheses.
.PP
By default, the full matching paragraphs are printed on the standard
output; specific fields can be selected for output with the -s option.
.PP
After the predicate comes zero or more file names.  If no file names
are specified, the file name is searched in configuration files.  The
input file from the first program name \- input file association with
the correct program name is used.  The program names are matched with
the base form of the name of the current program (the 0'th command
line argument, if you will).  The file name "-" is taken to mean the
standard input stream.  The files are searched in order but
separately; they are
.B not
concatenated together.  In other words, the end of a file always
implies the end of the current paragraph.
.PP
There is one exception to the above: if the program name is
.BR grep-dctrl ,
the default input source is always standard input; this cannot be
overridden by the configuration file.
.PP
The programs
.B grep-available
and 
.B grep-status
are aliases of (actually, symbolic links to)
.BR grep-dctrl .
In the shipped configuration, these aliases use as their default input
file the
.BR dpkg (8)
available and status files, respectively.
.SH OPTIONS
.SS Atomic predicate modifiers
.IP "-F FIELD,FIELD,...; --field=FIELD,FIELD,..."
Restrict pattern matching to the FIELDs given.  Multiple field names
in one -F option and multiple -F options in one atom are allowed; the
search named by the atom will be performed (disjunctively) among all
fields named.
.IP -P
Shorthand for "-FPackage".
.IP "-e, --eregex"
Regard the pattern of the current atomic predicate as an extended
POSIX regular expression
.IP "-r, --regex"
The pattern of the current atomic predicate is a standard POSIX regular expression.
.IP "-i, --ignore-case"
Ignore case when looking for a match in the current atomic predicate.
.IP "-X, --exact-match"
Do an exact match (as opposed to a substring match) in the current
atomic predicate.
.SS Predicate connectives
.IP "-!, --not, !"
Negate the following predicate.
.IP "-o, --or"
Disjunct the preceding and the following predicate.
.IP "-a, --and"
Conjunct the preceding and the following predicate.
.IP "( ... )"
Parentheses can be used for grouping.  Note that they need to be
escaped for most shells.
.SS Output format modifiers
.IP "-s FIELD,FIELD,...; --show-field=FIELD,FIELD,..."
Show only the body of these fields from the matching paragraphs.  The
field names must not include any colons or commas.  Commas are used to
delimit field names in the argument to this option.  The fields are
shown in the order given here.
.IP "-d"
Show only the first line of the Description field from the matching
paragraphs.  If no "-s" option is specified, this option also effects
"-s Description"; if there is a "-s" option but it does not include
the Description field name, one is appended to the option.  Thus the
Description field's location in the output is determined by the "-s"
option, if any, the last field being the default.
.IP "-n, --no-field-names"
Suppress field names when showing specified fields, only their bodies
are shown.  Each field is printed in its original form without the
field name, the colon after it and any whitespace predecing the start
of the body.
.IP "-v, --invert-match"
Instead of showing all the paragraphs that match, show those paragraphs
that do
.I not
match.
.IP "-c, --count"
Instead of showing the paragraphs that match (or, with -v, that don't
match), show the count of those paragraphs.
.IP "-q, --quiet, --silent"
Output nothing to the standard output stream.  Instead, exit
immediately after finding the first match.
.SS Miscellaneous
.IP "--config-file=FNAME"
Use FNAME as the config file instead of the defaults.
.IP "--debug-optparse"
Show how the current command line has been parsed. 
.IP "-l LEVEL, --errorlevel=LEVEL"
Set debugging level to LEVEL.  LEVEL is one of "fatal", "important",
"informational" and "debug", but the last may not be available,
depending on the compile-time options.  These categories are given
here in order; every message that is emitted when "fatal" is in
effect, will be emitted in the "important" error level, and so on.
The default is "important".
.IP "-V, --version"
Print out version information.
.IP "-C, --copying"
Print out the copyright license.  This produces much output; be sure
to redirect or pipe it somewhere (such as your favourite pager).
.IP "-h, --help"
Print out a help summary.
.SH EXAMPLES
The following example queries assume that the default configuration is
in effect.
.PP
The almost simplest use of this program is to print out the status or
available record of a package.  In this respect,
.B grep-dctrl
is like
.B "dpkg -s"
or
.BR "dpkg --print-avail".
To print out the status record of the package "mixal", do
.nf
% grep-status -PX mixal
.fi
and to get its available record, use
.nf
% grep-available -PX mixal
.fi
In fact, you can ask for the record of the "mixal" package
from any Debian control file.  Say, you have the Debian 2.2
CD-ROM's Packages file in the current directory; now you
can do a
.nf
% grep-dctrl -PX mixal Packages
.fi
.PP
But
.B grep-dctrl
can do more than just emulate
.BR dpkg .
It can more-or-less emulate
.BR apt-cache !
That program has a search feature that searches package descriptions.
But we can do that too:
.nf
% grep-available -F Description foo
.fi
searches for the string "foo" case-sensitively in the descriptions of
all available packages.  If you want case-insensitivity, use
.nf
% grep-available -F Description -i foo
.fi
Truth to be told,
.B apt-cache
searches package names, too.  We can separately search in the names;
to do so, do
.nf
% grep-available -F Package foo
.fi
or
.nf
% grep-available -P foo
.fi
which is pretty much the same thing.  We can also search in both
descriptions and names; if match is found in either, the package
record is printed:
.nf
% grep-available -P -F Description foo
.fi
or
.nf
% grep-available -F Package -F Description foo
.fi
This kind of search is the exactly same that
.B apt-cache
does.
.PP
Here's one thing neither
.B dpkg
nor
.B apt-cache
do.  Search for a string in the whole status or available file (or
any Debian control file, for that matter) and print out all package
records where we have a match.  Try
.nf
% grep-available dpkg
.fi
sometime and watch how thoroughly
.B dpkg
has infiltrated Debian.
.PP
All the above queries were based on simple substring searches.
But
.B grep-dctrl
can handle regular expressions in the search pattern.  For example,
to see the status records of all packages with either "apt" or
"dpkg" in their names, use
.nf
% grep-status -P -e 'apt|dpkg'
.fi
.PP
Now that we have seen all these fine and dandy queries, you might
begin to wonder whether it is necessary to always see the whole
paragraph.  You may be, for example, interest only in the dependency
information of the packages involved.  Fine.  To show the depends
lines of all packages maintained by me, do a
.nf
% grep-available -F Maintainer -s Depends 'ajk@debian.org'
.fi
If you want to see the packages' names, too, use
.nf
% grep-available -F Maintainer -s Package,Depends \\
  'ajk@debian.org'
.fi
Note that there must be no spaces in the argument to the -s switch.
.PP
More complex queries are also possible.  For example, to see the list of packages
maintained by me and depending on libc6, do
.nf
% grep-available -F Maintainer 'ajk@debian.org' \\
   -a -F Depends libc6 -s Package,Depends
.fi
Remember that you can use other Unix filters to help you, too.  Ever
wondered, who's the most active Debian developer based on the number
of source packages being maintained?  Easy.  You just need to have a
copy of the most recent Sources file from any Debian mirror.
.nf
% grep-dctrl -n -s Maintainer '' Sources | sort |
  uniq -c | sort -nr
.fi
This example shows a neat trick: if you want to selectively
show only some field of
.I all
packages, just supply an empty pattern.
.PP
The term "bogopackage" means the count of the packages that a Debian
developer maintains.  To get the bogopackage count for the maintainer
of
.BR grep-dctrl ,
say
.nf
% grep-available -c -FMaintainer \\
  "`grep-available -sMaintainer -n -PX grep-dctrl`"
.fi
.PP
Sometimes it is useful to output the data of several fields on the
same line.  For example, the following command outputs the list of
installed packages, sorted by their Installed-Size.
.nf
% grep-status -FStatus -sInstalled-Size,Package -n \\
    "install ok installed" | paste -sd "  \\n" | sort -n             
.fi
Note that there should be exactly 2 spaces in the "  \n" string. 
.PP
These examples cover a lot of typical uses of this utility, but not
all possible uses.  Use your imagination!  The building blocks are
there, and if something's missing, let me know.
.SH DIAGNOSTICS
In the absence of errors, the exit code 0 is used if at least one
match was found, and the exit code 1 is used if no matches were found.
If there were errors, the exit code is 2, with one exception.  If the
-q, --quiet or --silent options are used, the exit code 0 is used when
a match is found regardless of whether there have been non-fatal
errors.
.PP
These messages are emitted in log levels "fatal" and "important".
Additional messages may be provided by the system libraries.
.IP "a pattern is mandatory"
You must specify a pattern to be searched for.
.IP "a predicate is required"
No predicate was specified, but one is required.
.IP "cannot find enough memory"
More memory was needed than was available.  This error may be
transient, that is, if you try again, all may go well.
.IP "cannot suppress field names when showing whole paragraphs"
When you do not use the -s switch,
.B grep-dctrl
just passes the matching paragraphs through, not touching them any
way.  This means, for example, that you can only use -n when you use
-s.
.IP "inconsistent atom modifiers"
Conflicting atom modifiers were used; for example, perhaps both -X and
-e were specified for the same atom.
.IP "missing ')' in command line"
There were more opening than closing parentheses in the given
predicate.
.IP "no such log level"
The argument to -l was invalid.
.IP "unexpected ')' in command line"
There was no opening parenthesis that would match some closing
parenthesis in the command line.
.IP "unexpected end of file"
The input file is broken: it ends before it should.
.IP "unexpected end of line"
The input file is broken: a line ends before it should.
.IP "predicate is too complex"
The predicate's complexity (the number of atoms and connectives)
exceed compile-time limits.
.IP "read failure or out of memory"
There was a problem reading the configuration file.  Maybe there was a
transput error; maybe memory was exhausted.  This error may be
transient, that is, if you try again, all may go well.
.IP "syntax error in command line"
There is a problem in the command line.  Look, and you shall find it.
.IP "syntax error: need a executable name"
There is a problem in the configuration file.  Look, and you shall find it.
.IP "syntax error: need an input file name"
There is a problem in the configuration file.  Look, and you shall find it.
.IP "too many file names"
The number of file names specified in the command line exceeded a
compile-time limit.
.IP "too many output fields"
The argument to -s had too many field names in it.  This number is
limited to 256.
.SH FILES
.IP SYSCONF/grep-dctrl.rc
See the next file.
.IP ~/.grep-dctrlrc
These files are the default configuration files for
.BR grep-dctrl .
The format is line-based, with `#' introducing a comment that lasts to
the end of the line.  Each line defines one association between a
program name and a default input file.  These two things are listed in
the line in order, separated by whitespace.  Empty lines are ignored.
.IP /var/lib/dpkg/available
The default input file of
.B grep-available
when the shipped configuration is in effect.
.IP /var/lib/dpkg/status
The default input file of
.B grep-status
when the shipped configuration is in effect.
.SH AUTHOR
The program and this manual page were written by Antti-Juhani
Kaijanaho <gaia@iki.fi>.  Bill Allombert <ballombe@debian.org>
provided one of the examples in the manual page.
.SH "SEE ALSO"
Ian Jackson et al.: Debian Packaging Manual.  Published as the Debian
package packaging-manual.  Also available in the Debian website.  The
Debian project, 2003.
.PP
.BR ara (1),
.BR apt-cache (1),
.BR dpkg (8),
.BR dpkg-awk (1),
.BR sgrep (1)
\" Local variables:
\" mode: nroff
\" End:

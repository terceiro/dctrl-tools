/*  dctrl-tools - Debian control file inspection tools
    Copyright (C) 2003 Antti-Juhani Kaijanaho

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

#include "msg.h"
#include "paragraph.h"

void para_init(para_t * para, FSAF * fp, fieldtrie_t * trie) 
{
	para->fp = fp;
	para->trie = trie;
	para->start = 0; 
	para->end = 0;
	para->eof = false;
	para_parse_next(para);
}

void para_parse_next(para_t * para)
{
	debug_message("para_parse_next", 0);
	para->start = para->end;
	for (size_t i = 0; i < fieldtrie_count(para->trie); i++) {
		para->fields[i].start = 0;
		para->fields[i].end = 0;
	}
	fsaf_invalidate(para->fp, para->start);
	register enum { START, FIELD_NAME, BODY, BODY_NEWLINE,
			BODY_SKIPBLANKS, END } state = START;
	register size_t pos = para->start;
	register FSAF * fp = para->fp;
	size_t field_start = 0;
	struct field_data * field_data = 0;
	while (state != END) {
#               ifndef TEST_NODEBUG
		static char * const stnm[] = { "START", "FIELD_NAME",
					       "BODY", "BODY_NEWLINE",
					       "BODY_SKIPBLANKS", "END" };
		if (do_msg(L_DEBUG)) fprintf(stderr, "State: %s\n", stnm[state]);
#               endif
		int c = fsaf_getc(fp, pos++);
		switch (state) {
		case START:
			switch (c) {
			case -1:
				para->eof = true;
				state = END;
				break;
			case '\n':
				para->start++;
				break;
			default:
				field_start = --pos;
				state = FIELD_NAME;
			}
			break;
		case FIELD_NAME:
			switch (c) {
			case -1:
				message(L_FATAL, _("unexpected end of file"), 0);
				fail();
			case ':': {
				size_t len = (pos-1) - field_start;
				struct fsaf_read_rv r = fsaf_read(fp, field_start, len);
				assert(r.len == len);
				size_t inx = fieldtrie_lookup(para->trie, r.b, len);
				if (inx == -1) {
					field_data = 0;
				} else {
					assert(inx < MAX_FIELDS);
					field_data = &para->fields[inx];
					field_data->start = pos;
				}
				state = BODY;
			}
				break;
			case '\n':
				message(L_FATAL, _("unexpected end of line"), 0);
				fail();
			}
			break;
		case BODY:
			switch (c) {
			case -1:
				message(L_FATAL, _("unexpected end of file"), 0);
				fail();
			case '\n':
				if (field_data != 0) {
					field_data->end = pos-1;
					while (field_data->start < field_data->end
					       && fsaf_getc(fp, field_data->start) == ' ') {
						++field_data->start;
					}
				}
				state = BODY_NEWLINE;
				break;
			}
			break;
		case BODY_NEWLINE:
			switch (c) {
			case -1: 
				//para->eof = true;
				/* pass through */
			case '\n':
				state = END;
				break;
			case ' ': case '\t':
				state = BODY_SKIPBLANKS;
				break;
			default:
				field_start = --pos;
				state = FIELD_NAME;
			}
			break;
		case BODY_SKIPBLANKS:
			switch (c) {
			case -1:
				/* pass through */
			case '\n':
				state = END;
				break;
			case ' ': case '\t':
				break;
			default:
				state = BODY;
			}
			break;
		case END: assert(0);
		}
	}
	para->end = pos-1;
}
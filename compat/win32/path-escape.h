#ifndef PATH_ESCAPE_H
#define PATH_ESCAPE_H
#include "git-compat-util.h"

/* Unescape a single UTF16 character used as Win32 "forbidden character"
 * escape to one of the characters "*:<>?|. The colon is not unescaped if
 * it appears after a leading drive letter to avoid round-tripping problems,
 * since a colon appearing after a drive letter is never escaped.
 *
 * Parameters:
 *   start: Start of string
 *   wchar: Position of character to unescape. Only this character is unescaped.
 */
inline static void path_unescape_char(const wchar_t *start, wchar_t *wchar)
{
	/* Avoid doing 7 comparisons (that are all likely to fail) in most
	 * cases by bailing out early if the character doesn't have the rather
	 * unusual common bit pattern of all alternatives.
	 */
	if ((*wchar & 0xffa0) == 0xf020 &&
	    (*wchar == 0xf000 + '?' || *wchar == 0xf000 + '*' ||
	     *wchar == 0xf000 + '<' || *wchar == 0xf000 + '>' ||
	     *wchar == 0xf000 + '"' || *wchar == 0xf000 + '|' ||
	     (*wchar == 0xf000 + ':' &&
	      (wchar != start + 1 || !isalpha(*start))))) {
		*wchar -= 0xf000;
	}
}

/* Escape a single UTF16 character that is allowed in POSIX paths but not Win32
 * paths ("*:<>?|). Note that the caller must make sure not to escape the first
 * colon appearing after a drive letter.
 *
 * Parameters:
 *   wchar: Position of character to escape. Only this character is escaped.
 */
inline static void path_escape_char(wchar_t *wchar)
{
	/* Avoid doing 7 comparisons (that are all likely to fail) in some
	 * cases by bailing out early if the character doesn't have the
	 * common bit pattern of all alternatives.
	 */
	if ((*wchar & 0x00a0) == 0x0020 &&
	    (*wchar == '?' || *wchar == '*' || *wchar == '<' || *wchar == '>' ||
	     *wchar == '"' || *wchar == '|' || *wchar == ':')) {
		*wchar += 0xf000;
	}
}

#endif

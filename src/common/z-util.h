/* File z-util.h */

#ifndef INCLUDED_Z_UTIL_H
#define INCLUDED_Z_UTIL_H

#include "h-basic.h"


/*
 * Extremely basic stuff, like global temp and constant variables.
 * Also, some very useful low level functions, such as "streq()".
 * All variables and functions in this file are "addressable".
 */


/**** Available variables ****/

/* Temporary Vars */
extern char char_tmp;
extern byte byte_tmp;
extern sint sint_tmp;
extern uint uint_tmp;
extern long long_tmp;
extern huge huge_tmp;
extern errr errr_tmp;

/* Temporary Pointers */
extern cptr cptr_tmp;
extern vptr vptr_tmp;


/* Constant pointers (NULL) */
extern cptr cptr_null;
extern vptr vptr_null;


/* A bizarre vptr that always points at itself */
extern vptr vptr_self;


/* A cptr to the name of the program */
extern cptr argv0;


/* Aux functions */
extern void (*plog_aux)(cptr);
extern void (*quit_aux)(cptr);
extern void (*core_aux)(cptr);


/**** Available Functions ****/

/* Function that does nothing */
extern void func_nothing(void);

/* Functions that return basic "errr" codes */
extern errr func_success(void);
extern errr func_problem(void);
extern errr func_failure(void);

/* Functions that return bools */
extern bool func_true(void);
extern bool func_false(void);

/* Function that work with hturn */
extern int ht_passed(hturn *new_ht, hturn *old_ht, huge passed);
extern char* ht_show(hturn *ht_ptr, int mode);

/* Case insensitive comparison between two strings */
extern int my_stricmp(const char *s1, const char *s2);
extern int my_strnicmp(cptr a, cptr b, int n);
extern const char *my_stristr(const char *haystack, const char *needle);

/* Copy a string */
extern size_t my_strcpy(char *buf, const char *src, size_t bufsize);

/* Concatenate two strings */
extern size_t my_strcat(char *buf, const char *src, size_t bufsize);

/* Test equality, prefix, suffix, and do "strdup" */
extern bool streq(cptr s, cptr t);
extern bool prefix(cptr s, cptr t);
extern bool suffix(cptr s, cptr t);

#ifndef HAVE_STRDUP
extern char *strdup(cptr s);
#endif

/* Determine the length of a fixed-size string */
#ifndef HAVE_STRNLEN
extern size_t strnlen(char *s, size_t maxlen);
#endif


/* Print an error message */
extern void plog(cptr str);

/* Exit, with optional message */
extern void quit(cptr str);

/* Dump core, with optional message */
extern void core(cptr str);



#endif

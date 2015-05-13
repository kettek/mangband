/*
 * Handle client-side files, such as the .mangrc configuration
 * file, and some various "pref files".
 */

#include "angband.h"


/*
 * Extract the first few "tokens" from a buffer
 *
 * This function uses "colon" and "slash" as the delimeter characters.
 *
 * We never extract more than "num" tokens.  The "last" token may include
 * "delimeter" characters, allowing the buffer to include a "string" token.
 *
 * We save pointers to the tokens in "tokens", and return the number found.
 *
 * Hack -- Attempt to handle the 'c' character formalism
 *
 * Hack -- An empty buffer, or a final delimeter, yields an "empty" token.
 *
 * Hack -- We will always extract at least one token
 */
s16b tokenize(char *buf, s16b num, char **tokens)
{
        int i = 0;

        char *s = buf;


        /* Process */
        while (i < num - 1)
        {
                char *t;

                /* Scan the string */
                for (t = s; *t; t++)
                {
                        /* Found a delimiter */
                        if ((*t == ':') || (*t == '/')) break;

                        /* Handle single quotes */
                        if (*t == '\'') //'
                        {
                                /* Advance */
                                t++;

                                /* Handle backslash */
                                if (*t == '\\') t++;

                                /* Require a character */
                                if (!*t) break;

                                /* Advance */
                                t++;

                                /* Hack -- Require a close quote */
                                if (*t != '\'') *t = '\''; //'
                        }

                        /* Handle back-slash */
                        if (*t == '\\') t++; 
                }

                /* Nothing left */
                if (!*t) break;

                /* Nuke and advance */
                *t++ = '\0';

                /* Save the token */
                tokens[i++] = s;

                /* Advance */
                s = t;
        }

        /* Save the token */
        tokens[i++] = s;

        /* Number found */
        return (i);
}



/*
 * Convert a octal-digit into a decimal
 */
static int deoct(char c)
{
        if (isdigit(c)) return (D2I(c));
        return (0);
}

/*
 * Convert a hexidecimal-digit into a decimal
 */
static int dehex(char c)
{
        if (isdigit(c)) return (D2I(c));
        if (islower(c)) return (A2I(c) + 10);
        if (isupper(c)) return (A2I(tolower(c)) + 10);
        return (0);
}


/*
 * Hack -- convert a printable string into real ascii
 *
 * I have no clue if this function correctly handles, for example,
 * parsing "\xFF" into a (signed) char.  Whoever thought of making
 * the "sign" of a "char" undefined is a complete moron.  Oh well.
 */
void text_to_ascii(char *buf, cptr str)
{
        char *s = buf;

        /* Analyze the "ascii" string */
        while (*str)
        {
                /* Backslash codes */
                if (*str == '\\')
                {
                        /* Skip the backslash */
                        str++;

                        /* Hex-mode XXX */
                        if (*str == 'x')
                        {
                                *s = 16 * dehex(*++str);
                                *s++ += dehex(*++str);
                        }

                        /* Hack -- simple way to specify "backslash" */
                        else if (*str == '\\')
                        {
                                *s++ = '\\';
                        }

                        /* Hack -- simple way to specify "caret" */
                        else if (*str == '^')
                        {
                                *s++ = '^';
                        }

                        /* Hack -- simple way to specify "space" */
                        else if (*str == 's')
                        {
                                *s++ = ' ';
                        }

                        /* Hack -- simple way to specify Escape */
                        else if (*str == 'e')
                        {
                                *s++ = ESCAPE;
                        }

                        /* Backspace */
                        else if (*str == 'b')
                        {
                                *s++ = '\b';
                        }

                        /* Newline */
                        else if (*str == 'n')
                        {
                                *s++ = '\n';
                        }

                        /* Return */
                        else if (*str == 'r')
                        {
                                *s++ = '\r';
                        }

                        /* Tab */
                        else if (*str == 't')
                        {
                                *s++ = '\t';
                        }

                        /* Octal-mode */
                        else if (*str == '0')
                        {
                                *s = 8 * deoct(*++str);
                                *s++ += deoct(*++str);
                        }

                        /* Octal-mode */
                        else if (*str == '1')
                        {
                                *s = 64 + 8 * deoct(*++str);
                                *s++ += deoct(*++str);
                        }

                        /* Octal-mode */
                        else if (*str == '2')
                        {
                                *s = 64 * 2 + 8 * deoct(*++str);
                                *s++ += deoct(*++str);
                        }

                        /* Octal-mode */
                        else if (*str == '3')
                        {
                                *s = 64 * 3 + 8 * deoct(*++str);
                                *s++ += deoct(*++str);
                        }

                        /* Skip the final char */
                        str++;
                }

                /* Normal Control codes */
                else if (*str == '^')
                {
                        str++;
                        *s++ = (*str++ & 037);
                }

                /* Normal chars */
                else
                {
                        *s++ = *str++;
                }
        }

        /* Terminate */
        *s = '\0';
}

/*
 * Extract a "parsed" path from an initial filename
 * Normally, we simply copy the filename into the buffer
 * But leading tilde symbols must be handled in a special way
 * Replace "~user/" by the home directory of the user named "user"
 * Replace "~/" by the home directory of the current user
 */
errr path_parse(char *buf, int max, cptr file)
{
#ifndef WIN32
        cptr            u, s;
        struct passwd   *pw;
        char            user[128];
#endif /* WIN32 */


        /* Assume no result */
        buf[0] = '\0';

        /* No file? */
        if (!file) return (-1);

        /* File needs no parsing */
        if (file[0] != '~')
        {
                strcpy(buf, file);
                return (0);
        }

	/* Windows should never have ~ in filename */
#ifndef WIN32

        /* Point at the user */
        u = file+1;

        /* Look for non-user portion of the file */
        s = strstr(u, PATH_SEP);

        /* Hack -- no long user names */
        if (s && (s >= u + sizeof(user))) return (1);

        /* Extract a user name */
        if (s)
        {
                int i;
                for (i = 0; u < s; ++i) user[i] = *u++;
                user[i] = '\0';
                u = user;
        }

        /* Look up the "current" user */
        if (u[0] == '\0') u = getlogin();

        /* Look up a user (or "current" user) */
        if (u) pw = getpwnam(u);
        else pw = getpwuid(getuid());

        /* Nothing found? */
        if (!pw) return (1);

        /* Make use of the info */
        (void)strcpy(buf, pw->pw_dir);

        /* Append the rest of the filename, if any */
        if (s) (void)strcat(buf, s);

        /* Success */
#endif /* WIN32 */
        return (0);
}



/*
 * Hack -- replacement for "fopen()"
 */
FILE *my_fopen(cptr file, cptr mode)
{
        char                buf[1024];

        /* Hack -- Try to parse the path */
        if (path_parse(buf, 1024, file)) return (NULL);

        /* Attempt to fopen the file anyway */
        return (fopen(buf, mode));
}


/*
 * Hack -- replacement for "fclose()"
 */
errr my_fclose(FILE *fff)
{
        /* Require a file */
        if (!fff) return (-1);

        /* Close, check for error */
        if (fclose(fff) == EOF) return (1);

        /* Success */
        return (0);
}

/*
 * Hack -- replacement for "fgets()"
 *
 * Read a string, without a newline, to a file
 *
 * Process tabs, strip internal non-printables
 */
errr my_fgets(FILE *fff, char *buf, huge n)
{
        huge i = 0;

        char *s;

        char tmp[1024];

        /* Read a line */
        if (fgets(tmp, 1024, fff))
        {
                /* Convert weirdness */
                for (s = tmp; *s; s++)
                {
                        /* Handle newline */
                        if (*s == '\n')
                        {
                                /* Terminate */
                                buf[i] = '\0';

                                /* Success */
                                return (0);
                        }

                        /* Handle tabs */
                        else if (*s == '\t')
                        {
                                /* Hack -- require room */
                                if (i + 8 >= n) break;

                                /* Append a space */
                                buf[i++] = ' ';

                                /* Append some more spaces */
                                while (!(i % 8)) buf[i++] = ' ';
                        }

                        /* Handle printables */
                        else if (isprint(*s))
                        {
                                /* Copy */
                                buf[i++] = *s;

                                /* Check length */
                                if (i >= n) break;
                        }
                }
        }

        /* Nothing */
        buf[0] = '\0';

        /* Failure */
        return (1);
}


/*
 * Find the default paths to all of our important sub-directories.
 *
 * The purpose of each sub-directory is described in "variable.c".
 *
 * All of the sub-directories should, by default, be located inside
 * the main "lib" directory, whose location is very system dependant.
 *
 * This function takes a writable buffer, initially containing the
 * "path" to the "lib" directory, for example, "/pkg/lib/angband/",
 * or a system dependant string, for example, ":lib:".  The buffer
 * must be large enough to contain at least 32 more characters.
 *
 * Various command line options may allow some of the important
 * directories to be changed to user-specified directories, most
 * importantly, the "info" and "user" and "save" directories,
 * but this is done after this function, see "main.c".
 *
 * In general, the initial path should end in the appropriate "PATH_SEP"
 * string.  All of the "sub-directory" paths (created below or supplied
 * by the user) will NOT end in the "PATH_SEP" string, see the special
 * "path_build()" function in "util.c" for more information.
 *
 * Mega-Hack -- support fat raw files under NEXTSTEP, using special
 * "suffixed" directories for the "ANGBAND_DIR_DATA" directory, but
 * requiring the directories to be created by hand by the user.
 *
 * Hack -- first we free all the strings, since this is known
 * to succeed even if the strings have not been allocated yet,
 * as long as the variables start out as "NULL".
 */
void init_file_paths(char *path)
{
        char *tail;


        /*** Free everything ***/

        /* Free the main path */
        string_free(ANGBAND_DIR);

        /* Free the sub-paths */
        string_free(ANGBAND_DIR_APEX);
        string_free(ANGBAND_DIR_BONE);
        string_free(ANGBAND_DIR_DATA);
        string_free(ANGBAND_DIR_EDIT);
        string_free(ANGBAND_DIR_FILE);
        string_free(ANGBAND_DIR_HELP);
        string_free(ANGBAND_DIR_INFO);
        string_free(ANGBAND_DIR_SAVE);
        string_free(ANGBAND_DIR_USER);
        string_free(ANGBAND_DIR_XTRA);


        /*** Prepare the "path" ***/

        /* Hack -- save the main directory */
        ANGBAND_DIR = string_make(path);

        /* Prepare to append to the Base Path */
        tail = path + strlen(path);


#ifdef VM


        /*** Use "flat" paths with VM/ESA ***/

        /* Use "blank" path names */
        ANGBAND_DIR_APEX = string_make("");
        ANGBAND_DIR_BONE = string_make("");
        ANGBAND_DIR_DATA = string_make("");
        ANGBAND_DIR_EDIT = string_make("");
        ANGBAND_DIR_FILE = string_make("");
        ANGBAND_DIR_HELP = string_make("");
        ANGBAND_DIR_INFO = string_make("");
        ANGBAND_DIR_SAVE = string_make("");
        ANGBAND_DIR_USER = string_make("");
        ANGBAND_DIR_XTRA = string_make("");


#else /* VM */


        /*** Build the sub-directory names ***/

        /* Build a path name */
        strcpy(tail, "apex");
        ANGBAND_DIR_APEX = string_make(path);

        /* Build a path name */
        strcpy(tail, "bone");
        ANGBAND_DIR_BONE = string_make(path);

        /* Build a path name */
        strcpy(tail, "data");
        ANGBAND_DIR_DATA = string_make(path);

        /* Build a path name */
        strcpy(tail, "edit");
        ANGBAND_DIR_EDIT = string_make(path);

        /* Build a path name */
        strcpy(tail, "file");
        ANGBAND_DIR_FILE = string_make(path);

        /* Build a path name */
        strcpy(tail, "help");
        ANGBAND_DIR_HELP = string_make(path);

        /* Build a path name */
        strcpy(tail, "info");
        ANGBAND_DIR_INFO = string_make(path);

        /* Build a path name */
        strcpy(tail, "save");
        ANGBAND_DIR_SAVE = string_make(path);

        /* Build a path name */
        strcpy(tail, "user");
        ANGBAND_DIR_USER = string_make(path);

        /* Build a path name */
        strcpy(tail, "xtra");
        ANGBAND_DIR_XTRA = string_make(path);

#endif /* VM */


#ifdef NeXT

        /* Allow "fat binary" usage with NeXT */
        if (TRUE)
        {
                cptr next = NULL;

# if defined(m68k)
                next = "m68k";
# endif

# if defined(i386)
                next = "i386";
# endif

# if defined(sparc)
                next = "sparc";
# endif

# if defined(hppa)
                next = "hppa";
# endif

                /* Use special directory */
                if (next)
                {
                        /* Forget the old path name */
                        string_free(ANGBAND_DIR_DATA);

                        /* Build a new path name */
                        sprintf(tail, "data-%s", next);
                        ANGBAND_DIR_DATA = string_make(path);
                }
        }

#endif /* NeXT */

}





/*
 * Parse a sub-file of the "extra info" (format shown below)
 *
 * Each "action" line has an "action symbol" in the first column,
 * followed by a colon, followed by some command specific info,
 * usually in the form of "tokens" separated by colons or slashes.
 *
 * Blank lines, lines starting with white space, and lines starting
 * with pound signs ("#") are ignored (as comments).
 *
 * Note the use of "tokenize()" to allow the use of both colons and
 * slashes as delimeters, while still allowing final tokens which
 * may contain any characters including "delimiters".
 *
 * Note the use of "strtol()" to allow all "integers" to be encoded
 * in decimal, hexidecimal, or octal form.
 *
 * Note that "monster zero" is used for the "player" attr/char, "object
 * zero" will be used for the "stack" attr/char, and "feature zero" is
 * used for the "nothing" attr/char.
 *
 * Parse another file recursively, see below for details
 *   %:<filename>
 *
 * Specify the attr/char values for "monsters" by race index
 *   R:<num>:<a>:<c>
 *
 * Specify the attr/char values for "objects" by kind index
 *   K:<num>:<a>:<c>
 *
 * Specify the attr/char values for "features" by feature index
 *   F:<num>:<a>:<c>
 *
 * Specify the attr/char values for unaware "objects" by kind tval
 *   U:<tv>:<a>:<c>
 *
 * Specify the attr/char values for inventory "objects" by kind tval
 *   E:<tv>:<a>:<c>
 *
 * Define a macro action, given an encoded macro action
 *   A:<str>
 *
 * Create a normal macro, given an encoded macro trigger
 *   P:<str>
 *
 * Create a command macro, given an encoded macro trigger
 *   C:<str>
 *
 * Create a keyset mapping
 *   S:<key>:<key>:<dir>
 *
 * Turn an option off, given its name
 *   X:<str>
 *
 * Turn an option on, given its name
 *   Y:<str>
 *
 * Specify visual information, given an index, and some data
 *   V:<num>:<kv>:<rv>:<gv>:<bv>
 *
 * Specify a use for a subwindow
 *   W:<num>:<use>
 */
errr process_pref_file_aux(char *buf)
{
        int i, j, k;
	int n1, n2;

        char *zz[16];


        /* Skip "empty" lines */
        if (!buf[0]) return (0);

        /* Skip "blank" lines */
        if (isspace(buf[0])) return (0);

        /* Skip comments */
        if (buf[0] == '#') return (0);


        /* Require "?:*" format */
        if (buf[1] != ':') return (1);


        /* Process "%:<fname>" */
        if (buf[0] == '%')
        {
                /* Attempt to Process the given file */
                return (process_pref_file(buf + 2));
        }


        /* Process "R:<num>:<a>/<c>" -- attr/char for monster races */
        if (buf[0] == 'R')
        {
                if (tokenize(buf+2, 3, zz) == 3)
                {
                        i = (huge)strtol(zz[0], NULL, 0);
                        n1 = strtol(zz[1], NULL, 0);
                        n2 = strtol(zz[2], NULL, 0);
                        if (i >= MAX_R_IDX) return (1);
                        if (n1) Client_setup.r_attr[i] = n1;
                        if (n2) Client_setup.r_char[i] = n2;
                        return (0);
                }
        }


        /* Process "K:<num>:<a>/<c>"  -- attr/char for object kinds */
        else if (buf[0] == 'K')
        {
                if (tokenize(buf+2, 3, zz) == 3)
                {
                        i = (huge)strtol(zz[0], NULL, 0);
                        n1 = strtol(zz[1], NULL, 0);
                        n2 = strtol(zz[2], NULL, 0);
                        if (i >= MAX_K_IDX) return (1);
                        if (n1) Client_setup.k_attr[i] = n1;
                        if (n2) Client_setup.k_char[i] = n2;
                        return (0);
                }
        }


        /* Process "F:<num>:<a>/<c>" -- attr/char for terrain features */
        else if (buf[0] == 'F')
        {
                if (tokenize(buf+2, 3, zz) == 3)
                {
                        i = (huge)strtol(zz[0], NULL, 0);
                        n1 = strtol(zz[1], NULL, 0);
                        n2 = strtol(zz[2], NULL, 0);
                        if (i >= MAX_F_IDX) return (1);
                        if (n1) Client_setup.f_attr[i] = n1;
                        if (n2) Client_setup.f_char[i] = n2;
                        return (0);
                }
        }


        /* Process "U:<tv>:<a>/<c>" -- attr/char for unaware items */
        else if (buf[0] == 'U')
        {
                if (tokenize(buf+2, 3, zz) == 3)
                {
                        j = (huge)strtol(zz[0], NULL, 0);
                        n1 = strtol(zz[1], NULL, 0);
                        n2 = strtol(zz[2], NULL, 0);
			if (j > 100) return 0;
			if (n1) Client_setup.u_attr[j] = n1;
			if (n2) Client_setup.u_char[j] = n2;
                        return (0);
                }
        }


        /* Process "E:<tv>:<a>/<c>" -- attr/char for equippy chars */
        else if (buf[0] == 'E')
        {
		/* Do nothing */
		return (0);

#if 0
                if (tokenize(buf+2, 3, zz) == 3)
                {
                        j = (byte)strtol(zz[0], NULL, 0) % 128;
                        n1 = strtol(zz[1], NULL, 0);
                        n2 = strtol(zz[2], NULL, 0);
                        if (n1) tval_to_attr[j] = n1;
                        if (n2) tval_to_char[j] = n2;
                        return (0);
                }
#endif
        }

        /* Process "A:<str>" -- save an "action" for later */
        else if (buf[0] == 'A')
        {
                text_to_ascii(macro__buf, buf+2);
                return (0);
        }

        /* Process "P:<str>" -- create normal macro */
        else if (buf[0] == 'P')
        {
                char tmp[1024];
                text_to_ascii(tmp, buf+2);
                macro_add(tmp, macro__buf, FALSE);
                return (0);
        }

        /* Process "C:<str>" -- create command macro */
        else if (buf[0] == 'C')
        {
                char tmp[1024];
                text_to_ascii(tmp, buf+2);
                macro_add(tmp, macro__buf, TRUE);
                return (0);
        }


        /* Process "S:<key>:<key>:<dir>" -- keymap */
        else if (buf[0] == 'S')
        {
                if (tokenize(buf+2, 3, zz) == 3)
                {
                        i = strtol(zz[0], NULL, 0) & 0x7F;
                        j = strtol(zz[0], NULL, 0) & 0x7F;
                        k = strtol(zz[0], NULL, 0) & 0x7F;
                        if ((k > 9) || (k == 5)) k = 0;
                        keymap_cmds[i] = j;
                        keymap_dirs[i] = k;
                        return (0);
                }
        }


        /* Process "V:<num>:<kv>:<rv>:<gv>:<bv>" -- visual info */
        else if (buf[0] == 'V')
        {
		/* Do nothing */
		//return (0); // @@@ Why was this here?

                if (tokenize(buf+2, 5, zz) == 5)
                {
                        i = (byte)strtol(zz[0], NULL, 0);
                        color_table[i][0] = (byte)strtol(zz[1], NULL, 0);
                        color_table[i][1] = (byte)strtol(zz[2], NULL, 0);
                        color_table[i][2] = (byte)strtol(zz[3], NULL, 0);
                        color_table[i][3] = (byte)strtol(zz[4], NULL, 0);
                        return (0);
                }
        }


        /* Process "X:<str>" -- turn option off */
        else if (buf[0] == 'X')
        {
                for (i = 0; option_info[i].o_desc; i++)
                {
                        if (option_info[i].o_var &&
                            option_info[i].o_text &&
                            streq(option_info[i].o_text, buf + 2))
                        {
                                (*option_info[i].o_var) = FALSE;
				Client_setup.options[i] = FALSE;
                                return (0);
                        }
                }
        }

        /* Process "Y:<str>" -- turn option on */
        else if (buf[0] == 'Y')
        {
                for (i = 0; option_info[i].o_desc; i++)
                {
                        if (option_info[i].o_var &&
                            option_info[i].o_text &&
                            streq(option_info[i].o_text, buf + 2))
                        {
                                (*option_info[i].o_var) = TRUE;
				Client_setup.options[i] = TRUE;
                                return (0);
                        }
                }
        }

	/* Process "W:<num>:<use>" -- specify window action */
	else if (buf[0] == 'W')
	{
		if (tokenize(buf+2, 2, zz) == 2)
		{
			i = (byte)strtol(zz[0], NULL, 0);
			window_flag[i] = 1L << ((byte)strtol(zz[1], NULL, 0));
			return (0);
		}
	}


        /* Failure */
        return (1);
}


errr Save_options(void)
{
	int i;

    FILE *fp;

    char buf[1024];

    /* Build the filename */
    path_build(buf, 1024, ANGBAND_DIR_USER, "options.prf");

    /* Open the file */
    fp = my_fopen(buf, "w");

    /* Catch errors */
    if (!fp) return (-1);

	/* Skip space */
	fprintf(fp, "# This file can be used to set or clear all of the options.\n");
	fprintf(fp, "# Note that all of the options are given.\n\n");
	fprintf(fp, "# Remember that \"X\" turns an option OFF, while \"Y\" turns an option ON.\n");
	fprintf(fp, "# Also remember that not all options are used.\n\n");

	/* Process "X:<str>" and "Y:<str>" */
    for (i = 0; option_info[i].o_desc; i++)
    {
		if ((*option_info[i].o_var) == FALSE)
		{
            if (option_info[i].o_text)
				fprintf(fp, "X:%s\n", option_info[i].o_text);
		}
		else if ((*option_info[i].o_var) == TRUE)
		{
            if (option_info[i].o_text)
				fprintf(fp, "Y:%s\n", option_info[i].o_text);
		}
		else
			fprintf(fp, "\n");
		if ((i == 15) || (i == 27) || (i == 43) || (i == 59))
			fprintf(fp, "\n");
    }

	/* Close the file */
	my_fclose(fp);

	return 0;
}

/*
 * Process the "user pref file" with the given name
 *
 * See the function above for a list of legal "commands".
 */
errr process_pref_file(cptr name)
{
        FILE *fp;

        char buf[1024];


        /* Build the filename */
        path_build(buf, 1024, ANGBAND_DIR_USER, name);

        /* Open the file */
        fp = my_fopen(buf, "r");

        /* Catch errors */
        if (!fp) return (-1);

        /* Process the file */
        while (0 == my_fgets(fp, buf, 1024))
        {
                /* Process the line */
                if (process_pref_file_aux(buf))
                {
                        /* Useful error message */
                        printf("Error in '%s' parsing '%s'.", buf, name);
                }
        }

        /* Close the file */
        my_fclose(fp);

        /* Success */
        return (0);
}


/*
 * Show the Message of the Day.
 *
 * It is given in the "Setup" info sent by the server.
 */
void show_motd(void)
{
	int i;
	char ch;

	/* Clear the screen */
	Term_clear();

	for (i = 0; i < 23; i++)
	{
		/* Show each line */
		Term_putstr(0, i, -1, TERM_WHITE, &Setup.motd[i * 80]);
	}

	/* Show it */
	Term_fresh();

	/* Wait for a keypress */
	Term_inkey(&ch, TRUE, TRUE);

	/* Clear the screen again */
	Term_clear();
}

/*
 * Peruse a file sent by the server.
 *
 * This is used by the artifact list, the unique list, the player
 * list, *Identify*, and Self Knowledge.
 *
 * It may eventually be used for help file perusal as well.
 */
void peruse_file(void)
{
	char k; 

	/* Initialize */
	cur_line = 0;
	max_line = 0;

	/* The screen is icky */
	screen_icky = TRUE;

	/* Save the old screen */
	Term_save();

	/* Show the stuff */
	while (TRUE)
	{
		/* Clear the screen */
		//Term_clear();

		/* Send the command */
		Send_special_line(special_line_type, cur_line);

		/* Show a general "title" */
      //          prt(format("[Mangband %d.%d.%d] <%d>",
		//	VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, max_line), 0, 0);

		/* Prompt */
		//prt("[Press Return, Space, -, or ESC to exit.]", 23, 0);

		/* Get a keypress */
		k = inkey();

		/* Hack -- make any key escape if we're in popup mode */
		if (max_line <= (SCREEN_HGT - 2)/2 && special_line_type == SPECIAL_FILE_OTHER) k = ESCAPE;
		
		/* Hack -- go to a specific line */
		if (k == '#')
		{
			char tmp[80];
			prt("Goto Line: ", 23, 0);
			strcpy(tmp, "0");
			if (askfor_aux(tmp, 80, 0))
			{
				cur_line = atoi(tmp);
			}
		}

		/* Back up half page */
		if (k == '_')
		{
			cur_line -= 10;
			if (cur_line < 0) cur_line = 0;
		}

		/* Back up one full page */
		if ((k == '-') || (k == '9'))
		{
			cur_line -= 20;
			if (cur_line < 0) cur_line = 0;
		}


		/* Advance to the bottom */
		if (k == '1' && max_line)
		{
			cur_line = max_line - 20;
			if (cur_line < 0) cur_line = 0;
		}

		/* Back up to the top */
		if (k == '7')
		{
			cur_line = 0;
		}

		/* Advance one line */
		if ((k == '\n') || (k == '\r') || (k == '2'))
		{
			cur_line++;
		}
		
		/* Back up one line */
		if ((k == '=') || (k == '8'))
		{
			cur_line--;
			if (cur_line < 0) cur_line = 0;
		}

		/* Advance one page */
		if (k == ' ' || k == '3')
		{
			cur_line += 20;
		}

		/* Exit on escape */
		if (k == ESCAPE) break;

		/* Check maximum line */
		if (cur_line > max_line || cur_line < 0)
			cur_line = 0;
			
	}

	/* Tell the server we're done looking */
	Send_special_line(SPECIAL_FILE_NONE, 0);

	/* No longer using file perusal */
	special_line_type = 0;

	/* Reload the old screen */
	Term_load();

	/* The screen isn't icky anymore */
	screen_icky = FALSE;

	/* Flush any events that came in */
	Flush_queue();
}

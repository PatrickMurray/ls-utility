/*	
 *	options.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


/*	
 *	This structure contains both default and user specified runtime
 *	options which modify the program's output accordingly.
 *	
 */
struct options {
	int IGNORE_DOTFILES;
	int INCLUDE_HIDDEN_DIRECTORIES;
	int FORCE_MULTICOLUMN_OUTPUT;
	int USE_LAST_FILE_STATUS_TIME;
	int PLAINFILE_DIRECTORIES;
	int APPEND_FILETYPE_DESCRIPTOR;
	int DISABLE_SORTING;
	int HUMAN_READABLE_SIZE;
	int DISPLAY_FILE_SERIAL_NUMBER;
	int DISPLAY_SIZE_KILOBYTES;
	int DISPLAY_LONG_FORMAT;
	int NUMERIC_UID_GID;
	int HUMAN_READABLE_NONPRINTABLE_CHARACTERS;
	int RECURSIVELY_LIST_SUBDIRECTORIES;
	int REVERSE_SORT_ORDER;
	int SORT_BY_SIZE_DESCENDING;
	int DISPLAY_BLOCKS_USED;
	int USE_LAST_MODIFIED_TIME;
	int USE_LAST_ACCESS_TIME;
	int FORCE_NONPRINTABLE_CHARACTERS;
	int SORT_MULTICOLUMN_OUTPUT_ACROSS;
	int SINGLE_ENTRY_PER_LINE;
};


struct options *parse_argv         (int*, char**);
struct options *get_default_options();
void            remove_options     (int*, char**);
int             count_options      (int*, char**);


/*	
 *	Parses argc and argv for user specified options, and returns a
 *	pointer to the generated options structure.
 *	
 *	@param		int*				argc pointer
 *	@param		char**				arguments
 *	
 *	@return		struct options*		generated options
 *	
 */
struct options *parse_argv(int *argc, char **argv) {
	char *arguments;
	struct options *options;
	char character;
	
	/*	
	 *	Arguments which are supported, but not necessarily implemented
	 *	(i.e. the extra credit)
	 *	
	 */
	arguments = "AaCcdFfhiklnqRrSstuwx1";
	
	options = get_default_options();
	
	/*	
	 *	Get each option from argv in LR order and modify the options
	 *	structure.
	 *	
	 *	NOTE:
	 *		Options which are not implemented are marked or override the
	 *		behavior of another option are clearly marked.
	 */
	while ((character = getopt(*argc, argv, arguments)) >= 0) {
		switch (character) {
			case 'A':
				options->IGNORE_DOTFILES = 1;
				break;
			case 'a':
				options->INCLUDE_HIDDEN_DIRECTORIES = 1;
				break;
			case 'C':
				/* NOT IMPLEMENTED */
				break;
			case 'c':
				options->USE_LAST_FILE_STATUS_TIME = 1;
				/* OVERRIDES */
				options->USE_LAST_ACCESS_TIME = 0;
				break;
			case 'd':
				options->PLAINFILE_DIRECTORIES = 1;
				/* OVERRIDES */
				options->RECURSIVELY_LIST_SUBDIRECTORIES = 0;
				break;
			case 'F':
				options->APPEND_FILETYPE_DESCRIPTOR = 1;
				break;
			case 'f':
				options->DISABLE_SORTING = 1;
				break;
			case 'h':
				options->HUMAN_READABLE_SIZE = 1;
				/* OVERRIDES */
				options->DISPLAY_SIZE_KILOBYTES = 0;
				break;
			case 'i':
				options->DISPLAY_FILE_SERIAL_NUMBER = 1;
				break;
			case 'k':
				options->DISPLAY_SIZE_KILOBYTES = 1;
				/* OVERRIDES */
				options->HUMAN_READABLE_SIZE = 0;
				break;
			case 'l':
				options->DISPLAY_LONG_FORMAT = 1;
				/* OVERRIDES */
				options->SINGLE_ENTRY_PER_LINE = 0;
				options->FORCE_MULTICOLUMN_OUTPUT = 0;
				options->SORT_MULTICOLUMN_OUTPUT_ACROSS = 0;
				break;
			case 'n':
				options->NUMERIC_UID_GID = 1;
				/* ENABLES */
				options->DISPLAY_LONG_FORMAT = 1;
				break;
			case 'q':
				options->HUMAN_READABLE_NONPRINTABLE_CHARACTERS = 1;
				/* OVERRIDES */
				options->FORCE_NONPRINTABLE_CHARACTERS = 0;
				break;
			case 'R':
				options->RECURSIVELY_LIST_SUBDIRECTORIES = 1;
				/* OVERRIDES */
				options->PLAINFILE_DIRECTORIES = 0;
				break;
			case 'r':
				options->REVERSE_SORT_ORDER = 1;
				break;
			case 'S':
				options->SORT_BY_SIZE_DESCENDING = 1;
				break;
			case 's':
				options->DISPLAY_BLOCKS_USED = 1;
				break;
			case 't':
				options->USE_LAST_MODIFIED_TIME = 1;
				break;
			case 'u':
				options->USE_LAST_ACCESS_TIME = 1;
				/* OVERRIDES */
				options->USE_LAST_FILE_STATUS_TIME = 0;
				break;
			case 'w':
				options->FORCE_NONPRINTABLE_CHARACTERS = 1;
				/* OVERRIDES */
				options->HUMAN_READABLE_NONPRINTABLE_CHARACTERS = 0;
				break;
			case 'x':
				/* NOT IMPLEMENTED */
				break;
			case '1':
				options->SINGLE_ENTRY_PER_LINE = 1;
				/* OVERRIDES */
				options->FORCE_MULTICOLUMN_OUTPUT = 0;
				options->DISPLAY_LONG_FORMAT = 0;
				options->SORT_MULTICOLUMN_OUTPUT_ACROSS = 0;
				break;
			default:
			case '?':
				/*	
				 *	If an unknown option is encountered, exit the
				 *	program in failure.
				 *	
				 *	NOTE:
				 *		getopts() will display an error
				 *	
				 */
				exit(EXIT_FAILURE);
		}
	}
	
	/*	
	 *	Now that options have been parsed and stored in the options
	 *	struct, they no longer need to be represented in argc and argv.
	 *	
	 */
	remove_options(argc, argv);
	
	return options;
}


/*	
 *	Analyzes the program's runtime environment to determine what the
 *	default options are.
 *	
 *	NOTE:
 *		Options are assigned in the order in which they appear in the
 *		options structure.
 *	
 *	@return		struct options*		default options
 *	
 */
struct options *get_default_options() {
	struct options *options;
	uid_t euid;
	int is_terminal;
	
	/*	
	 *	Allocate cleared memory on the heap for one options structure.
	 *	If an error is encountered, display an appropriate error and
	 *	exit in failure.
	 *	
	 */
	if ((options = calloc(1, sizeof(struct options))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory for options\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	/*	
	 *	Fetch the user's effective user identifier, this function is
	 *	always successful as per the manual.
	 *	
	 */
	euid = geteuid();
	
	/*	
	 *	Determine if STDOUT is routed to a terminal. If an error occurs,
	 *	display an appropriate error and exit in failure.
	 *	
	 */
	if ((is_terminal = isatty(STDOUT_FILENO)) < 0) {
		fprintf(stderr,
		        "%s: unable to determine if STDOUT is terminal: %s\n",
		        getprogname(), strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	/*	
	 *	If the EUID is zero, that tells us that the user is a superuser.
	 *	As per the ls manual, dot files are to be ignored by default,
	 *	unless otherwise specified.
	 *	
	 */
	if (euid == 0) {
		options->IGNORE_DOTFILES = 1;
	} else {
		options->IGNORE_DOTFILES = 0;
	}
	
	options->INCLUDE_HIDDEN_DIRECTORIES = 0;
	
	/*	
	 *	If output is routed to a terminal, than multicolumn output is
	 *	the default; however, this feature has yet to implemented.
	 *	
	 */
	if (is_terminal) {
		options->FORCE_MULTICOLUMN_OUTPUT = 1;
	} else {
		options->FORCE_MULTICOLUMN_OUTPUT = 0;
	}
	
	options->USE_LAST_FILE_STATUS_TIME  = 0;
	options->PLAINFILE_DIRECTORIES      = 0;
	options->APPEND_FILETYPE_DESCRIPTOR = 0;
	options->DISABLE_SORTING            = 0;
	options->HUMAN_READABLE_SIZE        = 0;
	options->DISPLAY_FILE_SERIAL_NUMBER = 0;
	options->DISPLAY_SIZE_KILOBYTES     = 0;
	options->DISPLAY_LONG_FORMAT        = 0;
	options->NUMERIC_UID_GID            = 0;
	
	/*	
	 *	If output is to a terminal, than all output must be converted to
	 *	human readable characters - any non printable characters are to
	 *	be replaced with '?'.
	 *	
	 */
	if (is_terminal) {
		options->HUMAN_READABLE_NONPRINTABLE_CHARACTERS = 1;
	} else {
		options->HUMAN_READABLE_NONPRINTABLE_CHARACTERS = 0;
	}

	options->RECURSIVELY_LIST_SUBDIRECTORIES = 0;
	options->REVERSE_SORT_ORDER              = 0;
	options->SORT_BY_SIZE_DESCENDING         = 0;
	options->DISPLAY_BLOCKS_USED             = 0;
	options->USE_LAST_MODIFIED_TIME          = 0;
	options->USE_LAST_ACCESS_TIME            = 0;
	
	/*	
	 *	If output is not to a terminal, than non-printable characters
	 *	are to be printed. (i.e. output is piped to a file or another
	 *	process)
	 *	
	 */
	if (!is_terminal) {
		options->FORCE_NONPRINTABLE_CHARACTERS = 1;
	} else {
		options->FORCE_NONPRINTABLE_CHARACTERS = 0;
	}
	
	options->SORT_MULTICOLUMN_OUTPUT_ACROSS = 0;
	
	/*	
	 *	Once again, if output is not to a terminal, than entries are to
	 *	be displayed as one per line, unless otherwise specified at
	 *	runtime.
	 *	
	 */
	if (!is_terminal) {
		options->SINGLE_ENTRY_PER_LINE = 1;
	} else {
		options->SINGLE_ENTRY_PER_LINE = 0;
	}

	return options;
}


/*	
 *	Removes all parsed options from argc and argv. Argc and argv should
 *	then only contain entries that are to be traversed.
 *	
 *	NOTE:
 *		Thankfully, getopt() modified argv such that all arguments
 *		immediately follow argv[0] (the executable name)
 *		
 *		e.g: Take argv,
 *			"./ls directory1 -lA -h directory2" - runtime
 *			"./ls -lA -h directory1 directory2" - getopt() modification
 *			"directory1 directory2"             - after removal
 *	
 *	@param			int*			argument count pointer
 *	@param			char**			argument pointer
 *	
 *	@return			void
 *	
 */
void remove_options(int *argc, char **argv) {
	int n;
	int count;
	int i;
	int j;
	
	n = *argc;
	count = count_options(argc, argv);
	
	for (i = 0; i < n; i++) {
		j = i + count + 1;
		/* Shift over indexes to remove ./ls and options */
		if (j < n) {
			argv[i] = argv[j];
		}
	}
	
	/*	
	 *	Remove from argc the number of options plus the executable name.
	 *	
	 */
	*argc -= count + 1;
}


/*	
 *	Counts the number of options that appear in argv.
 *	
 *	@param			int*		argument count pointer
 *	@param			char**		argument pointer
 *	
 *	@return			int			option count
 *	
 */
int count_options(int *argc, char **argv) {
	int n;
	int count;
	int i;

	n = *argc;
	count = 0;

	for (i = 0; i < n; i++) {
		/*	
		 *	If the argument begins with '-' it is considered an
		 *	argument, and the count is incremented.
		 *	
		 */
		if (strncmp(argv[i], "-", 1) == 0) {
			count++;
		}
	}
	
	return count;
}

/*	
 *	file.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


/*	
 *	This structure contains basic file information gathered during entry
 *	traversal. This information will be used to sort based on whichever
 *	granularity the user specifies.
 *	
 */
struct file {
	char *name;
	struct stat stat;
	struct stat lstat;
	off_t size;
	time_t time;
};


/*	
 *	This structure contains formatted information regarding files. This
 *	information is used during both the normal display format and long
 *	format.
 *	
 *	NOTE:
 *		Not all of this information will be displayed by default,
 *		options may be required at runtime.
 *	
 */
struct metadata {
	char *blocksize;
	char *filemode;
	char *links;
	char *owner_id;
	char *owner_name;
	char *group_id;
	char *group_name;
	char *size;
	char *time_string;
	char *serial_number;
	char *descriptor;
};


struct file *generate_files     (struct options*, int, char**);
int          files_length       (struct file*);
int          is_dotfile         (char*);
int          is_hidden_directory(char*);


/*	
 *	Generates a pointer array of file structures.
 *	
 *	NOTE:
 *		This pointer array's length may be less than argc. This may
 *		occur when entries specified in argv are inaccessible or an
 *		error occurs.
 *	
 *	@param			struct options*		options to be followed
 *	@param			int					argument count
 *	@param			char**				arguments
 *	
 *	@return			struct file*		generated structure
 *	
 */
struct file *generate_files(struct options *options, int argc, char **argv) {
	struct file *files;
	int i;
	int idx;
	size_t t;
	
	/*	
	 *	Allocate cleared memory for argc + 1 instances of the file
	 *	structure, the final structure is necessary to act as a null
	 *	terminator. If an error occurs, display an appropriate error and
	 *	exit in failure.
	 *	
	 */
	if ((files = calloc(argc + 1, sizeof(struct file))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory for files\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	/*	
	 *	Traverses through every argument specified. The files array
	 *	index is only incremented if all information gathering is
	 *	successful; if an error occurs, the array entry will be
	 *	overridden by the next file entry or the null terminator.
	 *	
	 */
	for (i = 0, idx = 0; i < argc; i++) {
		/*	
		 *	If the entry is a dot file, and dot files are to be ignored,
		 *	skip the entry.
		 *	
		 */
		if (options->IGNORE_DOTFILES && is_dotfile(argv[i])) {
			continue;
		}
		
		/*	
		 *	If the entry is contained in a relatively hidden directory,
		 *	and hidden directories are excluded, skip the entry.
		 *	
		 */
		if (!options->INCLUDE_HIDDEN_DIRECTORIES &&
		    is_hidden_directory(argv[i])) {
			continue;
		}
		
		struct stat stat_buffer;
		struct stat lstat_buffer;
		
		if (stat(argv[i], &stat_buffer) < 0) {
			fprintf(stderr,
			        "%s: an error occurred while accessing '%s': %s\n",
			        getprogname(), argv[i], strerror(errno)
			);
			continue;
		}
		
		if (lstat(argv[i], &lstat_buffer) < 0) {
			fprintf(stderr,
			        "%s: an error occurred while accessing '%s': %s\n",
			        getprogname(), argv[i], strerror(errno)
			);
			continue;
		}
		
		/*	
		 *	Use the appropriate reference time for files, as specified
		 *	by the options.
		 *	
		 */
		if (options->USE_LAST_FILE_STATUS_TIME) {
			t = stat_buffer.st_ctime;
		} else if (options->USE_LAST_MODIFIED_TIME) {
			t = stat_buffer.st_mtime;
		} else if (options->USE_LAST_ACCESS_TIME) {
			t = stat_buffer.st_atime;
		} else {
			t = stat_buffer.st_mtime;
		}
		
		files[idx].name  = argv[i];
		files[idx].stat  = stat_buffer;
		files[idx].lstat = lstat_buffer;
		files[idx].size  = stat_buffer.st_size;
		files[idx].time  = t;
		
		idx++;
	}
	
	/* Assign null terminator */
	files[idx].name = NULL;
	
	return files;
}


/*	
 *	Returns the number of file structs in the provided pointer array.
 *	
 *	@param		struct file*		pointer array
 *	
 *	@return		int					length
 *	
 */
int files_length(struct file *files) {
	int i;
	for (i = 0; files[i].name != NULL; i++);
	return i;
}


/*	
 *	Determines if the given filename is a dot file. Returns 0 if the
 *	file is not a dot file, otherwise if it is a dot file.
 *	
 *	@param		char*		filename
 *	
 *	@return		int
 *	
 */
int is_dotfile(char *filename) {
	char *base;
	base = basename(filename);
	return (strcmp(base, ".") == 0 || strcmp(base, "..") == 0);
}


/*	
 *	Determines if the provided filename belongs in a hidden directory.
 *	Returns 0 if it does not, 1 if it is in a hidden directory.
 *	
 *	NOTE:
 *		This function compares the file's relative path, not absolute
 *		path. This was a design decision made so the program could be
 *		run within hidden directories, had the realpath been used, no
 *		entries would have been displayed.
 *	
 *	@param		char*		filename
 *	
 *	@return		int
 *	
 */
int is_hidden_directory(char *filename) {
	int n;
	char *parent;
	char *base;
	
	/*	
	 *	Because hidden directories begin with a dot character, we must
	 *	first check that the file is not a dot file.
	 *	
	 */
	if (is_dotfile(filename)) {
		return 0;
	}
	
	n = strlen(filename);
	
	if ((base = calloc(n+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}

	if ((parent = calloc(n+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}

	strcpy(base, filename);
	strcpy(parent, filename);

	strcpy(base, basename(base));
	strcpy(parent, dirname(parent));
	
	/*	
	 *	Iterate through the relative path as long as the base is not '/'
	 *	and more than one character long (basename() can return '.')
	 *	
	 */
	while (strcmp(base, "/") != 0 && strlen(base) > 1) {
		if (strlen(base) > 1 && strncmp(base, ".", 1) == 0) {
			return 1;
		}

		strcpy(parent, dirname(parent));
		strcpy(base, basename(parent));
	}
	
	return 0;
}

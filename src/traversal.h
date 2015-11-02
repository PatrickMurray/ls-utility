/*	
 *	traversal.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


void traverse_argv             (struct options*, int, char**);
void traverse_working_directory(struct options*);
void traverse_directory        (struct options*, char*);
void traverse_directory_helper (struct options*, char*, int);
int  count_directories         (struct file*);
int  count_directory_entries   (char*);


/*	
 *	Traverses the provided arguments and display's their entries.
 *	
 *	@param		struct options*		options
 *	@param		int					argument count
 *	@param		char**				arguments
 *	
 *	@return		void
 *	
 */
void traverse_argv(struct options *options, int argc, char **argv) {
	struct file *files;
	int length;
	int i;
	int num_directories;
	
	if (argc == 0) {
		traverse_working_directory(options);
		return;
	}
	
	files = generate_files(options, argc, argv);
	
	sort_files(options, files);
	
	display_entries(options, files);
	
	length = files_length(files);
	if (length > 0) {
		/* Provide spacing between multiple entries */
		printf("\n");
	}
	
	for (i = 0; i < length; i++) {
		if (S_ISDIR(files[i].stat.st_mode)) {
			traverse_directory(options, files[i].name);
			
			if (i != length - 1) {
				/* Space out entries except the last */
				printf("\n");
			}
		}
	}
	
	free(files);
}


/*	
 *	Displays the contents of the working directory.
 *	
 *	@param		struct options*		options
 *	
 *	@return		void
 *	
 */
void traverse_working_directory(struct options *options) {
	traverse_directory_helper(options, ".", 0);
}


/*	
 *	Displays the contents of the provided directory.
 *	
 *	@param		struct options*		options
 *	@param		char*				entry
 *	
 *	@return		void
 *	
 */
void traverse_directory(struct options *options, char *path) {
	traverse_directory_helper(options, path, 1);
}


/*	
 *	A helper function to traverse_working_directory() and
 *	traverse_directory() that actually does all the work.
 *	
 *	@param		struct options*		options
 *	@param		char*				entries
 *	@param		int					boolean value, show directory name
 *	
 *	@return		void
 *	
 */
void traverse_directory_helper(struct options *options, char *path, int show_path) {
	char cwd_buffer[PATH_MAX+1];
	char *cwd;
	DIR *dp;
	struct dirent *dirp;
	int num_entries;
	char **entries;
	int i;
	struct file *files;
	int num_directories;
	
	/* store where we are, we'll need a way to get back later */
	if ((cwd = getcwd(cwd_buffer, PATH_MAX+1)) == NULL) {
		fprintf(stderr, "%s: unable to fetch working directory: %s\n",
		        getprogname(), strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	if (show_path) {
		printf("%s:\n", path);
	}
	
	num_entries = count_directory_entries(path);
	
	if ((entries = calloc(num_entries, sizeof(char *))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n", getprogname());
		exit(EXIT_FAILURE);
	}
	
	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "%s: an error occurred while opening '%s': %s\n",
		        getprogname(), path, strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	/* fetch entries */
	for (i = 0; (dirp = readdir(dp)) != NULL; i++) {
		entries[i] = dirp->d_name;
	}
	
	if ((files = calloc(num_entries, sizeof(struct file))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n", getprogname());
		exit(EXIT_FAILURE);
	}
	
	/* change to the entry directory */
	if (chdir(path) != 0) {
		fprintf(stderr, "%s: unable to change working directory to '%s': %s\n", 
		        getprogname(), path, strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	files = generate_files(options, num_entries, entries);
	
	sort_files(options, files);
	
	display_entries(options, files);
	
	num_directories = count_directories(files);
	
	if (options->RECURSIVELY_LIST_SUBDIRECTORIES && num_directories > 0) {
		/* provide spacing around multi-line entries */
		printf("\n");
	}
	
	if (options->RECURSIVELY_LIST_SUBDIRECTORIES) {
		for (i = 0; i < num_entries; i++) {
			if (!is_dotfile(files[i].name) && S_ISDIR(files[i].stat.st_mode)) {
				traverse_directory(options, files[i].name);
				if (--num_directories > 0) {
					/* provide more spacing */
					printf("\n");
				}
			}
		}
	}
	
	/* return to the previous directory */
	if (chdir(cwd) < 0) {
		fprintf(stderr, "%s: unable to change working directory to '%s': %s\n", 
		        getprogname(), cwd, strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	closedir(dp);
}


/*	
 *	Counts the number of directories in an array of file structs.
 *	
 *	@param		struct file*		file struct array
 *	
 *	@return		int					count
 *	
 */
int count_directories(struct file *files) {
	int i;
	int n;
	int count;
	
	n = files_length(files);
	count = 0;
	
	for (i = 0; i < n; i++) {
		/* dot files appear as directories (because they point to them) */
		if (S_ISDIR(files[i].stat.st_mode) && !is_dotfile(files[i].name)) {
			count++;
		}
	}
	
	return count;
}


/*	
 *	Count the number of entries that a directory contains.
 *	
 *	@param		char *path			directory path
 *	
 *	@return		int					count
 *	
 */
int count_directory_entries(char *path) {
	DIR *dp;
	struct dirent *dirp;
	int count;
	
	if ((dp = opendir(path)) == NULL) {
		fprintf(stderr, "%s: an error occurred while opening '%s': %s\n",
		        getprogname(), path, strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	for (count = 0; (dirp = readdir(dp)) != NULL; count++);
	
	closedir(dp);
	
	return count;
}

/*	
 *	sorting.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


void sort_files  (struct options*, struct file*);
int  compare_size(const void*, const void*);
int  compare_time(const void*, const void*);
int  compare_name(const void*, const void*);
void reverse_sort(struct file*);


/*	
 *	Sorts the provided array of file structures in the manner specified
 *	in the options structure.
 *	
 *	@param		struct options*		runtime options
 *	@param		struct file*		files array
 *	
 *	@return		void
 *	
 */
void sort_files(struct options *options, struct file *files) {
	int length;
	
	/* Do nothing if sorting is disabled. */
	if (options->DISABLE_SORTING) {
		return;
	}
	
	length = files_length(files);
	
	/*	
	 *	Sort files by whichever granularities is specified in the
	 *	options. Several sorts must be reversed because qsort() returns
	 *	results in ascending order.
	 *	
	 */
	if (options->SORT_BY_SIZE_DESCENDING) {
		qsort(files, length, sizeof(struct file), compare_size);
		reverse_sort(files);
	} else if (options->USE_LAST_FILE_STATUS_TIME ||
	           options->USE_LAST_MODIFIED_TIME    ||
	           options->USE_LAST_ACCESS_TIME) {
		qsort(files, length, sizeof(struct file), compare_time);
		reverse_sort(files);
	} else {
		qsort(files, length, sizeof(struct file), compare_name);
	}
	
	/* Reverse the sort again if requested in the options. */
	if (options->REVERSE_SORT_ORDER) {
		reverse_sort(files);
	}
}


/*	
 *	Compare two file structures by the size of their file.
 *	
 *	@param			const void*		file struct
 *	@param			const void*		file struct
 *	
 *	@return			int
 *	
 */
int compare_size(const void *a, const void *b) {
	off_t x;
	off_t y;
	
	x = ((struct file *) a)->size;
	y = ((struct file *) b)->size;
	
	return (int) (x - y);
}


/*	
 *	Compare two file structures by their time attribute.
 *	
 *	@param			const void*		file struct
 *	@param			const void*		file struct
 *	
 *	@return			int
 *	
 */
int compare_time(const void *a, const void *b) {
	time_t x;
	time_t y;
	
	x = ((struct file *) a)->time;
	y = ((struct file *) b)->time;
	
	return (int) (x - y);
}


/*	
 *	Compare two file structures by their name.
 *	
 *	@param			const void*		file struct
 *	@param			const void*		file struct
 *	
 *	@return			int
 *	
 */
int compare_name(const void *a, const void *b) {
	char *x;
	char *y;
	int ret;
	
	/*	
	 *	File names must be converted to their lowercase equivalent so
	 *	that they are sorted in a manner understandable to the user.
	 *	
	 */
	x = strlowercase(((struct file *) a)->name);
	y = strlowercase(((struct file *) b)->name);
	
	ret = strcmp(x, y);
	
	return ret;
}

/*	
 *	Reverses the provided array of file structs.
 *	
 *	@param			struct file*	file array
 *	
 *	@return			void
 *	
 */
void reverse_sort(struct file *files) {
	int i;
	int n;
	struct file temp;
	
	n = files_length(files);
	
	for (i = 0; i < n / 2; i++) {
		temp = files[i];
		files[i] = files[n-i-1];
		files[n-i-1] = temp;
	}
}

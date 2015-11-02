/*	
 *	readability.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


/*	
 *	A structure used to store the maximum length of strings that are
 *	stored in the metadata structure. This maximum length is used to pad
 *	output appropriately in the long format.
 *	
 */
struct max_metadata {
	int blocksize;
	int filemode;
	int links;
	int owner_id;
	int owner_name;
	int group_id;
	int group_name;
	int size;
	int time_string;
	int serial_number;
	int descriptor;
};


struct max_metadata *generate_max_metadata(struct metadata*, int);
char                *kilobytes_size       ();
char                *human_readable_size  ();
char                *padding_left         (char*, size_t);
char                *padding_right        (char*, size_t);
char                *readable             (struct options*, char*);


/*	
 *	Takes an array of metadata structs and its length, and generates a
 *	max_metadata struct which corresponds to the maximum length output
 *	for each attribute in the given metadata array.
 *	
 *	@param			struct metadata*		metadata array
 *	@param			int						array length
 *	
 *	@return			struct max_metadata*	the maximum length of each
 *											metadata attribute
 *	
 */
struct max_metadata *generate_max_metadata(struct metadata *metadata, int length) {
	struct max_metadata *max;
	int i;
	size_t tmp;
	
	/*	
	 *	Allocate cleared memory for the max_metadata struct. Display an
	 *	appropriate error message and exit in failure if an error
	 *	occurs.
	 *	
	 */
	if ((max = calloc(1, sizeof(struct max_metadata))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	/* Initialize their default max lengths */
	max->blocksize     = 0;
	max->filemode      = 0;
	max->links         = 0;
	max->owner_id      = 0;
	max->owner_name    = 0;
	max->group_id      = 0;
	max->group_name    = 0;
	max->size          = 0;
	max->time_string   = 0;
	max->serial_number = 0;
	max->descriptor    = 0;
	
	/*	
	 *	Traverse the array of metadata and adjust the maximum length of
	 *	attributes as longer strings are encountered.
	 *	
	 */
	for (i = 0; i < length; i++) {
		tmp = strlen(metadata[i].blocksize);
		if (tmp > max->blocksize) {
			max->blocksize = tmp;
		}
		
		tmp = strlen(metadata[i].filemode);
		if (tmp > max->filemode) {
			max->filemode = tmp;
		}
		
		tmp = strlen(metadata[i].links);
		if (tmp > max->links) {
			max->links = tmp;
		}
		
		tmp = strlen(metadata[i].owner_id);
		if (tmp > max->owner_id) {
			max->owner_id = tmp;
		}
		
		tmp = strlen(metadata[i].owner_name);
		if (tmp > max->owner_name) {
			max->owner_name = tmp;
		}
		
		tmp = strlen(metadata[i].group_id);
		if (tmp > max->group_id) {
			max->group_id = tmp;
		}
		
		tmp = strlen(metadata[i].group_name);
		if (tmp > max->group_name) {
			max->group_name = tmp;
		}
		
		tmp = strlen(metadata[i].size);
		if (tmp > max->size) {
			max->size = tmp;
		}
		
		tmp = strlen(metadata[i].time_string);
		if (tmp > max->time_string) {
			max->time_string = tmp;
		}
		
		tmp = strlen(metadata[i].serial_number);
		if (tmp > max->serial_number) {
			max->serial_number = tmp;
		}
		
		tmp = strlen(metadata[i].descriptor);
		if (tmp > max->descriptor) {
			max->descriptor = tmp;
		}
	}
	
	return max;
}


/*	
 *	Pads the given string by concatenating spaces to the beginning of
 *	the string until it's length is as specified.
 *	
 *	@param		char*		string
 *	@param		size_t		desired length
 *	
 *	@return		char*		padded string
 *	
 */
char *padding_left(char *string, size_t length) {
	char *modified;
	int n;
	int i;
	int j;
	
	n = strlen(string);
	
	/* Allocate memory for the padded string and null terminator */
	if ((modified = calloc(length+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	for (i = 0, j = 0; i < length; i++) {
		if (i < length - n) {
			/* add spaces to the beginning of the string */
			modified[i] = ' ';
		} else {
			/* place the string's contents */
			modified[i] = string[j];
			j++;
		}
	}
	
	return modified;
}


/*	
 *	Pads the given string by concatenating spaces to the end of the
 *	string until it's length is as specified.
 *	
 *	@param		char*		string
 *	@param		size_t		desired length
 *	
 *	@return		char*		padded string
 *	
 */
char *padding_right(char *string, size_t length) {
	char *modified;
	int n;
	int i;
	
	n = strlen(string);
	
	/* Allocate memory for the padded string and null terminator */
	if ((modified = calloc(length+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	strncpy(modified, string, length);
	
	/* Append spaces to the end */
	for (i = n; i < length; i++) {
		modified[i] = ' ';
	}
	
	return modified;
}


/*	
 *	If the options require human readable non-printable characters, then
 *	all non printable characters in the string are swapped with '?',
 *	otherwise, the unmodified string is returned.
 *	
 *	@param		char*		string
 *	@param		size_t		desired length
 *	
 *	@return		char*		padded string
 *	
 */
char *readable(struct options *options, char *string) {
	char *modified;
	int length;
	int i;
	
	if (options->FORCE_NONPRINTABLE_CHARACTERS) {
		return string;
	}
	
	length = strlen(string);
	
	/* Allocate memory for new readable string */
	if ((modified = calloc(length + 1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	strcpy(modified, string);
	
	for (i = 0; i < length; i++) {
		if (!isprint(string[i])) {
			modified[i] = '?';
		}
	}
	
	return modified;
}

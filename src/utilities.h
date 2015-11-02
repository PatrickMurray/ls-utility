/*	
 *	utilities.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


int   null_pointer_array_length(void**);
char *strlowercase             (char*);


/*	
 *	Returns the length of any null terminating array.
 *	
 *	@param		void**		array
 *	
 *	@return		int			length
 *	
 */
int null_pointer_array_length(void **array) {
	int i;
	for (i = 0; array[i] != NULL; i++);
	return i;
}


/*	
 *	Converts the provided string to its lowercase equivalent.
 *	
 *	@param		char*		string
 *	
 *	@return		char*		lowercase string
 *	
 */
char *strlowercase(char *string) {
	int i;
	int n;
	char *modified;
	
	n = strlen(string);
	
	/*	
	 *	Allocate some cleared memory for the modified string, of length
	 *	|string| + 1 - to account for the null terminator.
	 *	 
	 */
	if ((modified = calloc(1, sizeof(string)+1)) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\b", getprogname());
		exit(EXIT_FAILURE);
	}
	
	/*	
	 *	The modified string consists of the lowercase equivalent of
	 *	every character in the string.
	 *	
	 */
	for (i = 0; i < n; i++) {
		modified[i] = tolower(string[i]);
	}
	
	return modified;
}

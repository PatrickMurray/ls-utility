/*	
 *	ls.c
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


/* System & Library Includes */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <errno.h>
#include <libgen.h>
#include <bsd/stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


/* Package Includes */
#include "utilities.h"
#include "options.h"
#include "file.h"
#include "sorting.h"
#include "readability.h"
#include "display_helper_functions.h"
#include "display.h"
#include "traversal.h"


/*	
 *	Initializes the program's runtime by branching out option parsing
 *	and entry traversal.
 *	
 *	@param		int		argument count
 *	@param		char**	argument string array
 *	
 *	@return		int		status code
 *	
 */
int main(int argc, char **argv) {
	struct options *options;
	
	setprogname(argv[0]);
	options = parse_argv(&argc, argv);
	traverse_argv(options, argc, argv);
	
	free(options);

	return EXIT_SUCCESS;
}

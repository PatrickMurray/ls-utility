/*	
 *	display_helper_functions.h
 *	
 *	"I pledge my honor that I have abided by the Stevens Honor System"
 *	
 *	(c) 2015 Patrick Murray
 *	
 */


blkcnt_t blocksize        (struct options*, struct file*);
blkcnt_t total_blocksize  (struct options*, struct file*);
char    *blocksize_string (struct options*, struct file*);
char    *filemode         (struct options*, struct file*);
nlink_t  links            (struct options*, struct file*);
char    *links_string     (struct options*, struct file*);
uid_t    owner_id         (struct options*, struct file*);
char    *owner_id_string  (struct options*, struct file*);
char    *owner_name       (struct options*, struct file*);
gid_t    group_id         (struct options*, struct file*);
char    *group_id_string  (struct options*, struct file*);
char    *group_name       (struct options*, struct file*);
off_t    size             (struct options*, struct file*);
char    *size_string      (struct options*, struct file*);
char    *readable_size    (off_t);
char    *kilobytes_size   (off_t);
char    *time_string      (struct options*, struct file*);
char    *serial_number    (struct options*, struct file*);
char     descriptor       (struct options*, struct file*);
char    *descriptor_string(struct options*, struct file*);


/*	
 *	Returns the blocksize of the given file. If the BLOCKSIZE
 *	environment variable is set, then that byte size will be used.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			blkcnt_t			block size
 *	
 */
blkcnt_t blocksize(struct options *options, struct file *file) {
	char *env;
	char *ptr;
	double tmp;
	blkcnt_t bs;
	
	bs = file->lstat.st_blocks;
	
	if ((env = getenv("BLOCKSIZE")) != NULL) {
		/* default blocksize is 256, have to multiply by that */
		bs *= 256;
		tmp = strtol(env, &ptr, 10);
		/* convert to new blocksize by dividing by the new blocksize */
		bs = ceil(bs / tmp);
	}
	
	return bs;
}


/*	
 *	Given an array of files, returns the summation of the blocksize.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file array pointer
 *	
 *	@return			blkcnt_t			total block size
 *	
 */
blkcnt_t total_blocksize(struct options *options, struct file *files) {
	int i;
	int n;
	blkcnt_t total;
	
	n = files_length(files);
	
	for (i = 0, total = 0; i < n; i++) {
		total += blocksize(options, &files[i]);
	}
	
	return total;
}


/*	
 *	Loads the blocksize, and returns it in the form of a string.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *blocksize_string(struct options *options, struct file *file) {
	char *string;
	blkcnt_t bs;
	int digits;
	
	bs = blocksize(options, file);
	
	if (bs == 0) {
		digits = 1;
	} else {
		digits = ceil(log10(bs));
	}
	
	if ((string = calloc(digits+2, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%zu ", bs);
	
	return string;
}


/*	
 *	Returns the filemode of the given file object.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *filemode(struct options *options, struct file *file) {
	char *string;
	int length;
	
	length = 11;
	
	if ((string = calloc(length+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	strmode(file->stat.st_mode, string);
	
	return string;
}


/*	
 *	Returns the number of links the given file object has pointing to it
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			nlink_t				number of links
 *	
 */
nlink_t links(struct options *options, struct file *file) {
	nlink_t links;
	links = file->lstat.st_nlink;
	return links;
}


/*	
 *	Returns a string of the number of links.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *links_string(struct options *options, struct file *file) {
	char *string;
	nlink_t link;
	int digits;
	
	link = links(options, file);
	
	if (link == 0) {
		digits = 1;
	} else {
		digits = ceil(log10(link));
	}
	
	if ((string = calloc(digits+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%zu", link);
	
	return string;	
}


/*	
 *	Returns the owner identifier of the given file pointer.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			uid_t				owner identifier
 *	
 */
uid_t owner_id(struct options *options, struct file *file) {
	return file->stat.st_uid;
}


/*	
 *	Returns a string of the given file pointer's owner identifier.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *owner_id_string(struct options *options, struct file *file) {
	char *string;
	uid_t id;
	int digits;
	
	id = owner_id(options, file);
	
	if (id == 0) {
		digits = 1;
	} else {
		digits = ceil(log10(id));
	}
	
	if ((string = calloc(digits+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%d", id);
	
	return string;
}


/*	
 *	Returns a string of the owner username.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *owner_name(struct options *options, struct file *file) {
	uid_t id;
	struct passwd *owner_pw;
	
	id = owner_id(options, file);
	owner_pw = getpwuid(id);
	
	return owner_pw->pw_name;
}


/*	
 *	Returns the group identifier of the given file struct pointer.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			gid_t				group identifier
 *	
 */
gid_t group_id(struct options *options, struct file *file) {
	return file->stat.st_gid;
}


/*	
 *	Returns a string of the group id number.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *group_id_string(struct options *options, struct file *file) {
	char *string;
	gid_t id;
	int digits;
	
	id = group_id(options, file);
	
	if (id == 0) {
		digits = 1;
	} else {
		digits = ceil(log10(id));
	}
	
	if ((string = calloc(digits+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%d", id);
	
	return string;
}


/*	
 *	Returns a string of the group name.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *group_name(struct options *options, struct file *file) {
	gid_t id;
	struct group *group_gr;
	
	id = group_id(options, file);
	group_gr = getgrgid(id);
	
	return group_gr->gr_name;
}


/*	
 *	Returns the size of the given file struct.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			off_t				file size
 *	
 */
off_t size(struct options *options, struct file *file) {
	return file->size;
}


/*	
 *	Returns the size of a file as a string.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *size_string(struct options *options, struct file *file) {
	char *string;
	off_t s;
	int digits;
	
	s = size(options, file);
	
	if (options->HUMAN_READABLE_SIZE) {
		return readable_size(s);
	} else if (options->DISPLAY_SIZE_KILOBYTES) {
		return kilobytes_size(s);
	}
	
	if (s == 0) {
		digits = 1;
	} else {
		digits = ceil(log10(s));
	}
	
	if ((string = calloc(digits+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%zu", s);
	
	return string;
}


/*	
 *	Returns the given size in a more human readable format.
 *	
 *	@param			off_t		file size
 *	
 *	@return			char*		string
 *	
 */
char *readable_size(off_t size) {
	char  *string;
	double n;
	int    exponent;
	int    precision;
	
	n         = size;
	exponent  = 0;
	precision = 1;
	
	//                XXX   .      X       G
	// 5 + precision = 3 + 1 + precision + 1
	if ((string = calloc(5 + precision, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	char abbreviations[9] = {
		'B',
		'K',
		'M',
		'G',
		'T',
		'P',
		'E',
		'Z',
		'Y'
	};
	
	
	while (n >= 1000 && exponent < (sizeof(abbreviations)/sizeof(char))) {
		n /= 1000;
		exponent += 1;
	}
	
	if (fmod(n, 1.0) >= (1.0 / pow(10, precision))) {
		sprintf(string, "%.*lf%c", precision, n, abbreviations[exponent]);
	} else {
		sprintf(string, "%i%c", (int) n, abbreviations[exponent]);
	}
	
	return string;
}


/*	
 *	Returns the given size in kilobytes.
 *	
 *	@param			off_t		file size
 *	
 *	@return			char*		string
 *	
 */
char *kilobytes_size(off_t size) {
	char *string;
	double n;
	int precision;
	
	n = ((float) size) / 1000;
	precision = 1;
	
	if ((string = calloc((int) n + 3, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	if (fmod(n, 1.0) >= (1.0 / pow(10, precision)))
	{
		sprintf(string, "%.*lf%c", precision, n, 'K');
	} else {
		sprintf(string, "%i%c", (int) n, 'K');
	}
	
	return string;
}


/*	
 *	Returns the pertinent filetime as a human readable string. If the TZ
 *	environment variable is set, that timezone will be used instead of
 *	the system's local timezone.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *time_string(struct options *options, struct file *file) {
	char *timezone;
	struct tm *local;
	int length;
	char *string;
	
	if ((local = calloc(1, sizeof(struct tm *))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	if ((timezone = getenv("TZ")) != NULL) {
		setenv("TZ", timezone, 1);
		tzset();
	}
	
	local = localtime(&file->time);
	length = 13;
	
	if ((string = calloc(length+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	strftime(string, length, "%b %d %H:%M", local);
	
	return string;
}


/*	
 *	Returns a string of the given file's serial number (inode number).
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *serial_number(struct options *options, struct file *file) {
	char *string;
	ino_t serial_number;
	int digits;
	
	serial_number = file->stat.st_ino;
	
	if (serial_number == 0) {
		digits = 1;
	} else {
		digits = ceil(log10(serial_number));
	}
	
	if ((string = calloc(digits+2, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%zu ", serial_number);
	
	return string;
}


/*	
 *	Returns the given file's file descriptor character.
 *	
 *	@param			struct options*			options
 *	@param			struct file*			file pointer
 *	
 *	@return			char					descriptor
 *	
 */
char descriptor(struct options *options, struct file *file) {
	char *mode;
	
	mode = filemode(options, file);
	
	switch (mode[0]) {
		case 'd':
			return '/';
		case 'l':
			return '@';
		case 'p':
			return '|';
		case 's':
			return '=';
		case 'w':
			return '%';
		case '?':
		default:
			if (file->stat.st_mode & S_IXUSR ||
	 		    file->stat.st_mode & S_IXGRP ||
	 		    file->stat.st_mode & S_IXOTH) {
				return '*';
			}
			return '\0';
	}
}


/*	
 *	Returns the given file's descriptor as a string.
 *	
 *	@param			struct options*		options
 *	@param			struct file*		file pointer
 *	
 *	@return			char*				string
 *	
 */
char *descriptor_string(struct options *options, struct file *file) {
	char *string;
	char desc;
	int digits;
	
	desc = descriptor(options, file);
	digits = 1;
	
	if ((string = calloc(digits+1, sizeof(char))) == NULL) {
		fprintf(stderr, "%s: unable to allocate memory\n",
		        getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	sprintf(string, "%c", desc);
	
	return string;
}

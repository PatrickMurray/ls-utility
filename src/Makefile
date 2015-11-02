NAME = ls
TARBALL = pmurray1-midterm.tar
SRC = ls.c
HEADERS = utilities.h options.h file.h sorting.h readability.h display_helper_functions.h display.h traversal.h
OBJ = $(SRC:.c=.o)
LINKS = -lbsd -lm
RM = rm -f

all: $(OBJ)
	gcc -Wall -Werror $(OBJ) -o $(NAME) $(LINKS)

clean:
	-$(RM) *~
	-$(RM) \#*
	-$(RM) *.o
	-$(RM) *.core

fclean:	clean
	-$(RM) $(NAME)

re: fclean all

submit: fclean
	tar cvzf $(TARBALL) Makefile $(SRC) $(HEADERS)

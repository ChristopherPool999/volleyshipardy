CC = g++ # compiler
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -O3 -lncurses # compiler options

OBJS = main.o volleyball.o battleship.o

a.out: ${OBJS}  # dependancies
	${CC} ${CFLAGS} ${OBJS}

main.o: main.cc battleship.h battleship.cc volleyball.h volleyball.cc #dependancies
	${CC} ${CFLAGS} -c main.cc 

volleyball.o: volleyball.cc volleyball.h #dependancies
	${CC} ${CFLAGS} -c volleyball.cc

battleship.o: battleship.cc battleship.h #dependancies
	${CC} ${CFLAGS} -c battleship.cc

clean: # rm removes files, -rf forcible removes them
	rm -rf ${OBJS}
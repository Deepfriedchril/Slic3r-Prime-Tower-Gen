CXX = gcc
SRCS = main.c
HEDR =

sptg: ${SRCS} ${HEDR}
	${CXX} ${SRCS} -o sptg

clean:
	rm -f *.o sptg
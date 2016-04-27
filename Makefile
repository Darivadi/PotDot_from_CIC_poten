CC = gcc

CFLAGS = -O3 -I/home/$(USER)/local/include -I/usr/include/ -DBINARYDATA 
CFLAGSDEBUG = -g -Wall -c -I/home/$(USER)/local/include/ -I/usr/include/ -DCIC -DBINARYDATA
LFLAGS = -L$(HOME)/local/lib

PROGRAM = main_PotDot_from_CIC_poten


$(PROGRAM):
	$(CC) -c $@.c $(CFLAGS)
	$(CC) $@.o -lm $(LFLAGS) -o $@.x


debug:
	echo Compiling for debug $(PROGRAM).c
	$(CC) $(CFLAGSDEBUG) $(PROGRAM).c -o $(PROGRAM).o
	$(CC) $(PROGRAM).o $(LFLAGS) -lm -lfftw3 -o $(PROGRAM).x


clean:
	rm -rf $(PROGRAM)
	rm -rf *~
	rm -rf *.out
	rm -rf *#
	rm -rf *.o
	rm -rf *.a
	rm -rf *.i
	rm -rf *.so
	rm -rf *.x

par-shell: par-shell.o commandlinereader.o proclist.o
	gcc -o par-shell par-shell.o commandlinereader.o proclist.o -pthread

proclist.o: proclist.c proclist.h
	gcc -Wall -g -c proclist.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc -Wall -g -c commandlinereader.c

par-shell.o: par-shell.c commandlinereader.h proclist.h
	gcc -Wall -g -pthread -c par-shell.c
	
clean:
	rm -f *.o par-shell
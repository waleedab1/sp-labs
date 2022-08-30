 #format is target-name: target dependencies 

#{-tab-}actions 

 # All Targets 

all: mypipe 

 # Tool invocations 

# Executable "hello" depends on the files main.o numbers.o add.o 

mypipe: mypipe.o
	gcc -m32 -g -Wall -o mypipe mypipe.o

 # Depends on the source and header files 

mypipe.o: mypipe.c
	gcc -g -Wall -m32 -c -o mypipe.o mypipe.c
	
 #tell make that "clean" is not a file name! 

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o mypipe

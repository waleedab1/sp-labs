 #format is target-name: target dependencies 

#{-tab-}actions 

 # All Targets 

all: myshell 

 # Tool invocations 

# Executable "hello" depends on the files main.o numbers.o add.o 

myshell: LineParser.o task3.o
	gcc -m32 -g -Wall -o myshell LineParser.o task3.o

 # Depends on the source and header files 

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c
	
task3.o: task3.c
	gcc -g -Wall -m32 -c -o task3.o task3.c
	
 #tell make that "clean" is not a file name! 

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o myshell

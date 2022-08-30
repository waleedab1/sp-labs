 #format is target-name: target dependencies 

#{-tab-}actions 

 # All Targets 

all: mypipeline 

 # Tool invocations 

# Executable "hello" depends on the files main.o numbers.o add.o 

mypipeline: mypipeline.o LineParser.o
	gcc -m32 -g -Wall -o mypipeline mypipeline.o LineParser.o

 # Depends on the source and header files 

mypipeline.o: mypipeline.c
	gcc -g -Wall -m32 -c -o mypipeline.o mypipeline.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c
 #tell make that "clean" is not a file name! 

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o mypipeline

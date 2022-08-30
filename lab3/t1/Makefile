 #format is target-name: target dependencies 

#{-tab-}actions 

 # All Targets 

all: virusDetector 

 # Tool invocations 

# Executable "hello" depends on the files main.o numbers.o add.o 

virusDetector: virusDetector.o
	gcc -m32 -g -Wall -o virusDetector virusDetector.o

 # Depends on the source and header files 

virusDetector.o: task1c.c
	gcc -g -Wall -m32 -c -o virusDetector.o task1c.c


 #tell make that "clean" is not a file name! 

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o virusDetector

 #format is target-name: target dependencies 

#{-tab-}actions 

 # All Targets 

all: myELF 

 # Tool invocations 

# Executable "hello" depends on the files main.o numbers.o add.o 

myELF: task1.o
	gcc -m32 -g -Wall -o myELF task1.o

 # Depends on the source and header files 

task1.o: task1.c
	gcc -g -Wall -elf32 -m32 -c -o task1.o task1.c


 #tell make that "clean" is not a file name! 

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o myELF

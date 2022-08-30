 #format is target-name: target dependencies 

#{-tab-}actions 

 # All Targets 

all: myELF 

 # Tool invocations 

# Executable "hello" depends on the files main.o numbers.o add.o 

myELF: task2.o
	gcc -m32 -g -Wall -o myELF task2.o

 # Depends on the source and header files 

task2.o: task2.c
	gcc -g -Wall -elf32 -m32 -c -o task2.o task2.c


 #tell make that "clean" is not a file name! 

.PHONY: clean 

 #Clean the build directory 

clean:
	rm -f *.o myELF

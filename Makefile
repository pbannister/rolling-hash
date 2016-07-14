#
#	Build and measure rolling hash implmentation.
#

all: # default rule

bin			: ; mkdir bin
o 			: ; mkdir o

clean 		: clean.o o
clean.o 	: ; rm -rf o 
 
CC			= cc -O3
#CC			= g++ -O3

o/%.o: sources/%.c
	$(CC) -c -o $@ $<

o/%.o: sources/%.cpp
	$(CC) -c -o $@ $<

O_rate=o/main.o o/ZRollingHash.o	
X_rate=bin/rate

$(X_rate) : $(O_rate)
	$(CC) -lc++ -o $@ $(O_rate)
	
rate : $(X_rate)
	$(X_rate)
	
all : o bin rate

o/%.s: sources/%.cpp
	$(CC) -S -c -o $@ $<

asm :  o/ZRollingHash.s

.PHONY: all clean clean.o test

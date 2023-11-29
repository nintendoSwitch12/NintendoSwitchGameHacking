#Makefile

CC := aarch64-linux-gnu-gcc
SRC := ./src

all: main.o random.o sdk.o
	$(CC) -o main main.o random.o sdk.o

main.o: $(SRC)/main.c $(SRC)/random.h $(SRC)/sdk.h
	$(CC) -c $(SRC)/main.c

sdk.o : $(SRC)/sdk.c $(SRC)/sdk.h 
	$(CC) -c $(SRC)/sdk.c

random.o : $(SRC)/random.c $(SRC)/random.h
	$(CC) -c $(SRC)/random.c

.PHONY: claen
clean :
	rm *.o -rf

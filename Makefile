CC = g++
CFLAGS = -g

all: final

final: mvote.o Prompt.o linear_hash_table_1.o InputFile.o reverse_index.o ElectorClass.o
	$(CC) $(CFLAGS) -o final mvote.o Prompt.o linear_hash_table_1.o InputFile.o reverse_index.o ElectorClass.o

mvote.o: mvote.cpp
	$(CC) $(CFLAGS) -c mvote.cpp

ElectorClass.o: ElectorClass.cpp Elector.h
	$(CC) $(CFLAGS) -c ElectorClass.cpp

Prompt.o: Prompt.cpp Prompt.h
	$(CC) $(CFLAGS) -c Prompt.cpp

linear_hash_table_1.o: linear_hash_table_1.cpp linear_hash_table.h
	$(CC) $(CFLAGS) -c linear_hash_table_1.cpp

InputFile.o: InputFile.cpp InputFile.h
	$(CC) $(CFLAGS) -c InputFile.cpp

reverse_index.o: reverse_index.cpp reverse_index.h
	$(CC) $(CFLAGS) -c reverse_index.cpp

clean:
	rm -f *.o final

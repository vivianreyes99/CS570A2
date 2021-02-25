wordcount: wordcount.o
	gcc -g wordcount.o -lpthread -o wordcount
wordcount.o: wordcount.c
	gcc -c wordcount.c -lpthread -o wordcount.o
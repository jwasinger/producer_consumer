TARGET="producer_consumer"

all:
	gcc -g -std=c99 -lpthread producer_consumer.c -o $(TARGET)

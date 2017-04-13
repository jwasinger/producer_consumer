TARGET="producer_consumer"

all:
	gcc -g -std=c99 -lpthread producer_consumer.h producer_consumer.c main.c -o $(TARGET)

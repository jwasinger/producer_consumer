#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#include "producer_consumer.h"

void *consumer_main(void *sync_buf) {
	while(1) {
		consume_item((SyncBuffer *)sync_buf);
	}
}

void *producer_main(void *sync_buf) {
	while(1) {
		produce_item((SyncBuffer *)sync_buf);
	}
}

int main(int argc, char **argv) {
	SyncBuffer *sync_buf = NULL;
	capture_error(init_sync_buffer(&sync_buf));

	pthread_t consumer, producer;

	pthread_create(&consumer,
								 NULL,
								 consumer_main,
								 sync_buf);

	pthread_create(&producer,
								 NULL,
								 producer_main,
								 sync_buf);
	
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	return 0;
}

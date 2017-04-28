#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<getopt.h>

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
	int num_consumers, num_producers, arg;
	num_consumers = num_producers = arg = -1;
	pthread_t *consumers = NULL;
	pthread_t *producers = NULL;

	while((arg = getopt(argc, argv, "c:p:")) != -1) {
		switch(arg) {
			case 'c':
				num_producers = atoi(optarg);
				break;
			case 'p':
				num_consumers = atoi(optarg);
				break;
		}
	}

	if(num_consumers == -1 || num_producers == -1 || num_producers <= 0 || num_consumers <= 0) {
		printf("Argument error...\n");
		exit(-1);
	}

	if (get_rdrand_support() == 1) {
		printf("rdrand supported\n");
	} else {
		printf("rdrand supported\n");
	}

	capture_error(init_sync_buffer(&sync_buf));

	consumers = malloc(sizeof(pthread_t) * num_consumers);
	producers = malloc(sizeof(pthread_t) * num_producers);

	for(int i = 0; i < num_producers; i++) {
		capture_error(pthread_create(producers + i,
						NULL,
						producer_main,
						sync_buf));
	}
	
	for(int i = 0; i < num_producers; i++) {
		capture_error(pthread_create(consumers + i,
						NULL,
						consumer_main,
						sync_buf));
	}

	for(int i = 0; i < num_producers; i++) {
		pthread_join(producers[i], NULL);
	}

	for(int i = 0; i < num_consumers; i++) {
		pthread_join(consumers[i], NULL);
	}

	return 0;
}

#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

#define BUF_MAX_LEN 20

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<stdint.h>
#include<x86intrin.h>

typedef struct {
	int Id;
	int WaitTime;
} Item;

typedef struct {
	Item *buffer;
	int	buffer_len;
	pthread_mutex_t *mutex;
} SyncBuffer;

//void pop_buffer(struct *SyncBuffer buf);

//void push_buffer(struct *SyncBuffer buf);

void produce_item();

void consume_item();

int init_sync_buffer(SyncBuffer** rw_buf);

int32_t gen_rdrand();

Item gen_item();

void capture_error(int ret);

#endif

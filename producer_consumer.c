#include "producer_consumer.h"

void fatal_error(char *error_str) {
	printf(error_str);
	exit(-1);
}


int get_rdrand_support() {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	char vendor[13];
	
	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	
	if(ecx & 0x40000000){
		return 1;
	}
	else{
		return 0;
	}
}

/*
 * TODO: make threadsafe
 */
void capture_error(int ret) {
  int errno_saved = errno;
  fprintf(stderr, "An error occurred!");
  fprintf(stderr, "The error value is %d\n", errno_saved);
	exit(-1);
}

void fail_if_error(int ret) {
	if (ret != 0) {
		exit(-1);
	}
}

int init_sync_buffer(SyncBuffer** rw_buf) {
	int ret = 0;

	SyncBuffer *new_buf = malloc(sizeof(SyncBuffer));
	if (new_buf == NULL) {
		fatal_error("...");
	}

	new_buf->buffer = malloc(sizeof(BUF_MAX_LEN));
	if (new_buf->buffer == NULL) {
		fatal_error("...");
	}

	new_buf->buffer_len = 0;

	ret = pthread_mutex_init(new_buf->mutex, NULL);
	if(ret != 0) {
		fatal_error("...");
	}

	*rw_buf = new_buf;
	return 0;
}

void pop_buffer(SyncBuffer* buf) {
	int ret = 0;

	if (buf->buffer_len == 0) {
		return;
	}

	ret = pthread_mutex_lock(buf->mutex);
	
	buf->buffer_len--;

	ret = pthread_mutex_unlock(buf->mutex);
}

void push_buffer(SyncBuffer* buf, Item *item) {
	if (buf->buffer_len == BUF_MAX_LEN) {
		return;
	}

	while(1) {
		if (buf->buffer_len == BUF_MAX_LEN) {
			continue;
		}

		capture_error(pthread_mutex_lock(buf->mutex));
		
		buf->buffer[buffer->buffer_len] = *item;
		buf->buffer_len++;

		capture_error(pthread_mutex_unlock(buf->mutex));
	}
}

void produce_item(SyncBuffer *buf) {
	while(1) {
		if (buffer_len != BUF_MAX_LEN) {
			//wait 3-7 seconds before producing an item
			int32_t rand_sleep_value = ...;

			push_buffer()
		}
	}
}

void consume_item() {
	while(1) {
		if (buffer_len != 0) {
			//remove an item from the buffer
			pop_buffer()

			//decrement buffer length
			unlock_mutex(buffer_mutex);
			break;
		}
	}
}

Item gen_item() {
	Item item;
	item.Id = next_id;
	next_id++;
	item.wait = gen_rand();
}

int32_t gen_rdrand() {
	if(get_rdrand_support() != 0) {
		int result;
		_rdrand32_step(&result);
		return result;
	} else {
		printf("rdrand not supported\n");
		exit(-1);
	}
}

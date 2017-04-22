#include "producer_consumer.h"

int next_id = 0;

void fatal_error(char *error_str) {
	printf(error_str);
	exit(-1);
}

/*
 * Wrapper around inline x86 asm to check hardware support for RDRAND
 */
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
 *
 * Fail if provided return value is not 0 and print a message about the error.
 *
 */
void capture_error(int ret) {
	if(ret != 0) {
		int errno_saved = errno;
		fprintf(stderr, "An error occurred!");
		fprintf(stderr, "The error value is %d\n", errno_saved);
		exit(-1);
	}
}

void fail_if_error(int ret) {
	if (ret != 0) {
		exit(-1);
	}
}

/*
 * SyncBuffer Init function
 */
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

	ret = pthread_mutex_init(&(new_buf->mutex), NULL);
	if(ret != 0) {
		fatal_error("...");
	}

	*rw_buf = new_buf;
	return 0;
}

int pop_buffer(SyncBuffer* buf) {
	int ret = 0;

	if (buf->buffer_len == 0) {
		return 0;
	}

	ret = pthread_mutex_lock(&(buf->mutex));

	//buffer has been modified by another thread while waiting for mutex to unlock
	if (buf->buffer_len == 0) {
		return 0;
	}

	
	buf->buffer_len--;

	capture_error(pthread_mutex_unlock(&(buf->mutex)));

	return 1;
}

void push_buffer(SyncBuffer* buf, Item *item) {
	if (buf->buffer_len == BUF_MAX_LEN) {
		return;
	}

	while(1) {
		if (buf->buffer_len == BUF_MAX_LEN) {
			continue;
		}

		capture_error(pthread_mutex_lock(&(buf->mutex)));
		
		buf->buffer[buf->buffer_len] = *item;
		buf->buffer_len++;

		printf("item produced:%d\n", buf->buffer_len);

		capture_error(pthread_mutex_unlock(&(buf->mutex)));
		break;
	}
}

/*
 * Generate an item and push it onto the buffer.  If the buffer is full, block until an item
 * is removed.  Then, add the new item
 */
void produce_item(SyncBuffer *buf) {
	while(1) {
		if (buf->buffer_len != BUF_MAX_LEN) {
			//wait 3-7 seconds before producing an item
			int32_t rand_sleep_value = (gen_rdrand() % 4)+3;
			sleep(rand_sleep_value);

			Item item = gen_item();
			push_buffer(buf, &item);


		}
	}
}

/*
 * Consume the next item.  If the stack is empty, block until another item is added
 * and then remove it
 */
void consume_item(SyncBuffer *buf) {
	int ret = -1;
	while(1) {
		if (buf->buffer_len != 0) {
			//remove an item from the buffer
			ret = pop_buffer(buf);
			if(ret == 1) {
				printf("item consumed:%d\n", buf->buffer_len);
			}

			//decrement buffer length
			pthread_mutex_unlock(&(buf->mutex));
			break;
		}
	}
}

Item gen_item() {
	Item item;
	item.Id = next_id;
	next_id++;
	item.WaitTime = gen_rdrand();
}

/*
 * generate a random 32bit integer using hardware-based rdrand
 *
 */
int32_t gen_rdrand() {
	if(get_rdrand_support() != 0) {
		int result;
		_rdrand32_step(&result);
		return result;
	} else {
		return (int32_t)genrand_int32();
	}
}

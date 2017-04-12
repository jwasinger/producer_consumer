#define BUF_MAX_LEN 20

struct Item {
	
}


struct SyncBuffer {
	Item *buffer;
	int	buffer_len;
	pthread_mutex_t *mutex;
}

void pop_buffer(struct *SyncBuffer buf);

void push_buffer(struct *SyncBuffer buf);

void produce_item();

void consume_item();

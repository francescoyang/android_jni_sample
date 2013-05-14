//#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern pthread_cond_t tntcond;
extern pthread_mutex_t lntmutex;

class TestPort{
public:
	explicit TestPort();
	~TestPort();

	static void *thr_fn(void *arg);
	void InitPort();

private:
	pthread_t ntid;
};

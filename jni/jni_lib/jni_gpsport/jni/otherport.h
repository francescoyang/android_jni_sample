//#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern pthread_cond_t ntcond;
extern pthread_mutex_t ntmutex;

class Otherport{
public:
	explicit Otherport();
	~Otherport();

	static void *thr_fn(void *arg);
	void InitPort();

private:
	pthread_t ntid;
};

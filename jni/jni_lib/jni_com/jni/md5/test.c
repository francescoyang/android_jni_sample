#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	md5_state_t ctx;
	unsigned char* data = "12345";
	unsigned char ret[16];
	memset(ret, 0, sizeof(ret));

	md5_init(&ctx);
	md5_append(&ctx, data, strlen(data));
	md5_finish(&ctx, ret);

	int i;
	printf("Md5 result: ");
	for (i = 0;i < 16;++i){
		printf("%02X", ret[i]);
	}
	printf("\n");

	return 0;
}

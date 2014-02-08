#include<cstring>
#include<stdio.h>

#include"utils.h"


int main() {
	const char* testData = "token TOken token. TTTK KFas, askd. blaa";
// Test readToken
	char buf[1024];
	int offset = 0;
	bool hasToken = 1;
	printf("Testing tokenization\n");
	while(hasToken) {
		hasToken = readToken(testData, offset, buf, 1024);
		if(hasToken)
			printf("Token: %s\n", buf);
	}

// Test lowercase
	printf("Testing lowercase\n");
	offset = 0;
	hasToken = 1;
	while(hasToken) {
		hasToken = readToken(testData, offset, buf, 1024);
		lowercaseToken(buf);
		if(hasToken)
			printf("Token: %s\n", buf);
	}
	
}


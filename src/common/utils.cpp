#include "utils.h"

#include<stdio.h>
#include<cstring>

#include<sys/stat.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

void writeToken(const char* buffer, int fd) {
	int len = strlen(buffer);
	int bytesWritten = write(fd, buffer, len);
	if(len != bytesWritten) {
		printf("Error writing token\n");
	}
}

const int DIFF = 'a' - 'A';
void lowercaseToken(char* token) {
	char* str = token;
	while(*str != '\0') {
		char c = *str;
		if(c >= 'A' && c <= 'Z') {
			*str = c + DIFF;
		}		
		str++;
	}
}

bool isAlphanumeric(char c) {
	return (c >= 'a' && c <= 'z') ||
				 (c >= 'A' && c <= 'Z')	||
				 (c >= '0' && c <= '9');
}

bool shouldRead(char c) {
	return isAlphanumeric(c);
}

bool readToken(const char* data, int& offset, char* buffer, int buffLength) {
	const char* str = data + offset;
	if(*str == '\0')
		return false;
	int leading = 0;
	while(!isAlphanumeric(*str) && *str != '\0') {
		str++;
		leading++;
	}

	int i = 0;
	while(shouldRead(*str) && i < buffLength) {
		*(buffer + i) = *str;
		i++;
		str++;
	}

	offset = offset + leading + i;
	buffer[i] = 0;
	return i > 0;
}

int readFullData(int fd, char** data) {
	const int BUFF_SIZE = 4096 * 1024;
	char* buffer = new char[BUFF_SIZE];
	int bytes = 1;
	bytes = read(fd, buffer, BUFF_SIZE);
	if(bytes >= 0)
		buffer[bytes] = 0;
	*data = buffer;
	return bytes;
}


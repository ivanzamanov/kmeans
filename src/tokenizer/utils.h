#ifndef __TOK_UTILS_H__
#define __TOK_UTILS_H__

bool readToken(const char* data, int& offset, char* buffer, int buffLength);
void lowercaseToken(char* token);
void writeToken(const char* buffer, int outputFD);

#endif

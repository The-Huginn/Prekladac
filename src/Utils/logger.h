#ifndef __LOGGER__
#define __LOGGER__

#include<stdio.h>
#include<stdlib.h>

#ifdef _DEBUG

#define INFO(s, ...) \
	do { \
	char buffer[255]; \
	sprintf(buffer, "INFO [%s]: %s\n", __func__, s); \
	fprintf(stderr, buffer, __VA_ARGS__); \
	} while (0) 

#define WARNING(s, ...) \
	do { \
	char buffer[255]; \
	sprintf(buffer, "WARNING [%s]: %s\n", __func__, s); \
	fprintf(stderr, buffer, __VA_ARGS__); \
	} while (0) 

#define ERROR(s, ...) \
	do { \
	char buffer[255]; \
	sprintf(buffer, "ERROR [%s]: %s\n", __func__, s); \
	fprintf(stderr, buffer, __VA_ARGS__); \
	exit(1); \
	} while (0)

#else

#define WARING(s, ...)

#define ERROR(s, ...)

#endif // DEBUG


#endif // !__LOGGER__

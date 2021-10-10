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
	return 0; \
	} while (0)

#define ERROR_VOID(s, ...) \
	do { \
	char buffer[255]; \
	sprintf(buffer, "ERROR [%s]: %s\n", __func__, s); \
	fprintf(stderr, buffer, __VA_ARGS__); \
	return; \
	} while (0)

#else

#define INFO(s, ...)

#define WARNING(s, ...)

#define ERROR(s, ...) \
	do { \
	return 0; \
	} while (0)

#define ERROR_VOID(s, ...) \
	do { \
	return; \
	} while (0)


#endif // DEBUG


#endif // !__LOGGER__

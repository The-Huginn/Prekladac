#ifndef __LOGGER__
#define __LOGGER__

#include<stdio.h>
#include<stdlib.h>

#define DEBUG

#ifdef DEBUG

#define INFO(s, ...) \
	do { \
	fprintf(stderr, "INFO [%s]: " s "\n", __func__, ##__VA_ARGS__); \
	} while (0) 

#define WARNING(s, ...) \
	do { \
	fprintf(stderr, "WARNING [%s]: " s "\n", __func__, ##__VA_ARGS__); \
	} while (0) 

#define ERROR(s, ...) \
	do { \
	fprintf(stderr, "ERROR [%s]: " s "\n", __func__, ##__VA_ARGS__); \
	return 0; \
	} while (0)

#define ERROR_VOID(s, ...) \
	do { \
	fprintf(stderr, "ERROR [%s]: " s "\n", __func__, ##__VA_ARGS__); \
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

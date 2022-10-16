#ifndef OTHER_H_
#define OTHER_H_

#define EXIT_CODE 1

/* a macro used in order to check memory allocations #defensiveprogramming */

#define CHECK(ptr)                                                          \
do {                                                                        \
    if (!ptr) {                                                             \
        fprintf(stderr, "malloc() failed at %s:%d\n", __FILE__, __LINE__);  \
        exit(EXIT_CODE);                                                    \
    }                                                                       \
} while(0);

#endif /* OTHER_H_ */

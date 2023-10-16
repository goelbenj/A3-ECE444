#include <stdio.h>

#define FREE 0
#define BLOCKED 1

struct h_Node { 	
    int       STATUS;
    size_t SIZE;
    void   *c_blk;
    void   *n_blk;
    struct h_Node * NEXT;
};

extern struct h_Node h_list;

int m_init(void);

void *m_malloc(size_t size);

void m_free(void *ptr);

void *m_realloc(void *ptr, size_t size);

void h_layout(struct h_Node *ptr);

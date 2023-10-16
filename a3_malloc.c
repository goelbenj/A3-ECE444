#include "a3_malloc.h"
#include <unistd.h>

#define HEAP_SIZE 10000
char h_mem[HEAP_SIZE] = {0};
void *h_mem_start;
void *h_mem_end;
void *c_break;

struct h_Node h_list;

int m_init(void) {
    /*
    This function initializes a virtual heap in the stack memory of this program.
    The heap has a size of HEAP_SIZE bytes.
    */

    void *c_break = sbrk(HEAP_SIZE);
    if (c_break == (void *)-1) {
        // virtual heap cannot be created
        return -1;
    } else {
        // initialize h_list
        h_list.STATUS = FREE;
        h_list.SIZE = HEAP_SIZE;
        h_list.c_blk = c_break;
        h_list.n_blk = NULL;
        h_list.NEXT = NULL;
        return 0;
    }
}

void *m_malloc(size_t size) {
    /*
    This function allocates a block of size size in the virtual heap.
    */

    // traverse h_list and search for first free block of size size
    struct h_Node *curr = &h_list;
    while (curr != NULL) {
        if (curr->STATUS == FREE && size < curr->SIZE) {
            // fragment this block into two FREE and BLOCKED blocks
            struct h_Node *new_h_node = sbrk(sizeof(struct h_Node));
            new_h_node->STATUS = BLOCKED;
            new_h_node->SIZE = size;
            new_h_node->c_blk = curr->c_blk;
            new_h_node->n_blk = NULL;

            curr->SIZE -= size;
            curr->c_blk += size;
            curr->n_blk = new_h_node->c_blk;
            curr->NEXT = new_h_node;
            return new_h_node->c_blk;
        } else if (curr->STATUS == FREE && size == curr->SIZE) {
            curr->STATUS = BLOCKED;
            return curr->c_blk;
        }
        curr = curr->NEXT;
    }
    return (void *) NULL;
}

void h_layout(struct h_Node *ptr) {
    /*
    This function displays the layout of the h_list.
    */

    struct h_Node *curr = ptr;
    int i = 0;
    while (curr != NULL) {
        printf("NODE %i\n", i);
        printf("STATUS: %i\n", curr->STATUS);
        printf("SIZE: %zu\n", curr->SIZE);
        printf("c_blk: %p\n", curr->c_blk);
        printf("n_blk: %p\n", curr->n_blk);
        printf("NEXT: %p\n", curr->NEXT);
        printf("\n");
        i++;
        curr = curr->NEXT;
    }
}

int main(int argc, char *argv[])
{
    int status = m_init();
    h_layout(&h_list);
    printf("---------\n");
    void *return_status;
    return_status = m_malloc(9999);
    printf("RETURN STATUS: %p\n", return_status);
    h_layout(&h_list);
    return_status = m_malloc(2);
    printf("RETURN STATUS: %p\n", return_status);
}

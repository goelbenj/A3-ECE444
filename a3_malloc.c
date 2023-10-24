#include "a3_malloc.h"
#include <unistd.h>

#define HEAP_SIZE 20

struct h_Node h_list;

void consolidate_blocks(struct h_Node *block_1, struct h_Node *block_2);

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
            new_h_node->n_blk = (curr->NEXT) ? curr->NEXT->c_blk : NULL;
            new_h_node->NEXT = curr->NEXT;

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

void m_free(void *ptr) {
    /*
    This function frees the allocated block pointed to by ptr
    and returns it. Memory can only be freed if ptr points to
    a valid block in the heap. This function will also
    consolidate neighbouring blocks.
    */

    // traverse h_list and search for h_node whose c_blk == ptr
    struct h_Node *prev = NULL;
    struct h_Node *curr = &h_list;
    while (curr != NULL) {
        if (curr->c_blk == ptr) {
            curr->STATUS = FREE;

            // perform consolidation with neighbouring blocks
            consolidate_blocks(curr, curr->NEXT);
            consolidate_blocks(prev, curr);
            break;
        }
        prev = curr;
        curr = curr->NEXT;
    }
}

void *m_realloc(void *ptr, size_t size) {
    /*
    This function will find the h_Node associated with ptr and resize it
    to size. The address of the new ptr to the resized block could be the
    same as ptr or a new, different pointer. Note that the content of
    the common parts between the old and the new blocks should be the same.
    */

    // find associated h_Node
    struct h_Node *curr = &h_list;
    while (curr != NULL) {
        if (curr->c_blk == ptr) {
            break;
        }
        curr = curr->NEXT;
    }

    if (curr == NULL) {
        // case 1: ptr does not point to an allocated block
        return m_malloc(size);
    } else if (size == curr->SIZE) {
        // case 2: size is equal to h_Node->SIZE
        return ptr;
    } else if (size < curr->SIZE) {
        // case 3: size is smaller than h_Node->SIZE
        size_t delta = size - 1;
        curr->SIZE -= delta;
        struct h_Node *new_h_node = sbrk(sizeof(struct h_Node));
        new_h_node->STATUS = FREE;
        new_h_node->SIZE = delta;
        new_h_node->c_blk = curr->c_blk + curr->SIZE;
        new_h_node->n_blk = (curr->NEXT) ? curr->NEXT->c_blk : NULL;
        new_h_node->NEXT = curr->NEXT;

        curr->NEXT = new_h_node;
        curr->n_blk = new_h_node->c_blk;
        return curr->c_blk;

    } else {
        // case 4: size is larger than h_Node->SIZE

        // 1. copy data into temp
        size_t copy_size = curr->SIZE;
        char temp[copy_size];
        for (int i = 0; i < copy_size; i++) {
            temp[i] = ((char *)curr->c_blk)[i];
        }
        // 2. free block (and consolidate)
        m_free(ptr);
        // 3. malloc new block with increased size
        void *new_ptr = m_malloc(size);
        // 4. copy temp into data block
        for (int i = 0; i < copy_size; i++) {
            ((char *)new_ptr)[i] = temp[i];
        }
        return new_ptr;
    }

}

void consolidate_blocks(struct h_Node *block_1, struct h_Node *block_2) {
    /*
    This function will consolidate two blocks if they are free and non-NULL.
    */

    if (block_1 == NULL || block_2 == NULL) {
        // cannot consolidate NULL blocks
        return;
    }

    if (block_1->STATUS == FREE && block_2->STATUS == FREE) {
        block_1->SIZE += block_2->SIZE;
        block_1->n_blk = (block_2->NEXT) ? block_2->NEXT->c_blk : NULL;
        block_1->NEXT = block_2->NEXT;
        sbrk(-1 * sizeof(struct h_Node));
    }
}

void print_block(void *ptr, size_t size) {
    printf("BLOCK CONTENTS:\n");
    for (int i = 0; i < size; i++) {
        char c = ((char *)ptr)[i];
        printf("%c", c);
    }
    printf("\n");
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
        print_block(curr->c_blk, curr->SIZE);
        printf("\n");
        i++;
        curr = curr->NEXT;
    }
}

int main(int argc, char *argv[])
{
    int status = m_init();
    char *return_status = (char *)m_malloc(4);
    return_status[0] = 'D';
    return_status[1] = 'E';
    return_status[2] = 'A';
    return_status[3] = 'D';
    void *return_status_1 = m_malloc(1);
    char *return_status_2 = (char *)m_malloc(3);
    return_status_2[0] = 'C';
    return_status_2[1] = 'A';
    return_status_2[2] = 'T';
    void *new_realloc = m_realloc(return_status, 5);
    h_layout(&h_list);
    printf("---------\n");
    // m_free(return_status_2);
    // h_layout(&h_list);
    // m_free(return_status_1);
    // printf("---------\n");
    // h_layout(&h_list);
    // m_free(return_status);
    // printf("---------\n");
    // h_layout(&h_list);
}

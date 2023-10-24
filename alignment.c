#include <stdio.h>

// Define struct a_test1
struct a_test1 {
    short a;
    int x;
    short b;
} table1[10];

struct a_test1 *p1, *q1;

// Define struct a_test2
struct a_test2 {
    short a;
    short b;
    int x;
} table2[10];

struct a_test2 *p2, *q2;

int main(int argc, char *argv[]) {
    // STRUCT 1
    // a - Print the size of each member type in struct a_test1 (here short and int)
    printf("Sizeof a: %zu bytes\n", sizeof(p1->a));
    printf("Sizeof x: %zu bytes\n", sizeof(p1->x));
    printf("Sizeof b: %zu bytes\n", sizeof(p1->b));

    // b - Print the size of table1[0] and then the sizeof the whole table1
    printf("Sizeof table1[0]: %zu bytes\n", sizeof(table1[0]));
    printf("Sizeof table1: %zu bytes\n", sizeof(table1));

    // c - Print the alignment and address of table1[0]
    // 4-byte alignment = log2(4)= 2 lower bits of address are 0
    // get alignment of struct by taking
    printf("Address of table1[0]: %p bytes\n", table1);
    printf("Alignment of table1[0]: %lu bytes\n", sizeof(table1[0].x));

    // d-In a loop, print the aligned addresses of the previous and
    // the current elements in table1 (starting with table1[1]),
    // and then the difference between their addresses showing the
    // length of the previous element in the array
    for (int i = 1; i < 10; i++) {
        p1 = &table1[i];
        q1 = &table1[i-1];
        long diff = (long)p1 - (long)q1;
        printf("Different between addresses of table1[%i] and table1[%i]:"
               "%p - %p = %ld\n", i, i-1, p1, q1, diff);
    }


    // STRUCT 2
    // a - Print the size of each member type in struct a_test1 (here short and int)
    printf("Sizeof a: %zu bytes\n", sizeof(p2->a));
    printf("Sizeof b: %zu bytes\n", sizeof(p2->b));
    printf("Sizeof x: %zu bytes\n", sizeof(p2->x));

    // b - Print the size of table1[0] and then the sizeof the whole table1
    printf("Sizeof table2[0]: %zu bytes\n", sizeof(table2[0]));
    printf("Sizeof table2: %zu bytes\n", sizeof(table2));

    // c - Print the alignment and address of table2[0]
    // 4-byte alignment = log2(4)= 2 lower bits of address are 0
    // get alignment of struct by taking
    printf("Address of table2[0]: %p bytes\n", table2);
    printf("Alignment of table2[0]: %lu bytes\n", sizeof(table2[0].x));

    // d-In a loop, print the aligned addresses of the previous and
    // the current elements in table2 (starting with table2[1]),
    // and then the difference between their addresses showing the
    // length of the previous element in the array
    for (int i = 1; i < 10; i++) {
        p2 = &table2[i];
        q2 = &table2[i-1];
        long diff = (long)p2 - (long)q2;
        printf("Different between addresses of table2[%i] and table2[%i]:"
               "%p - %p = %ld\n", i, i-1, p2, q2, diff);
    }
}

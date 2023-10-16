#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    void *c_break = sbrk(0);
    printf("1- %p \n", c_break);
    c_break = sbrk(+5);
    printf("2- %p \n", c_break);
    c_break = sbrk(+5);
    printf("3- %p \n", c_break);
}

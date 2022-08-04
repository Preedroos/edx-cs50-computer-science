#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Hello, world!\n\n");
    string name = get_string("What's your name? ");
    printf("Hello, %s\n", name);
}
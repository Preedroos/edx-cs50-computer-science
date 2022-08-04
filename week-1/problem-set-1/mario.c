#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Enter the Height: ");
    }
    while (height < 1 || height > 8);

    for (int i = 1; i <= height; i++)
    {

        // Left space
        for (int j = height; j > i; j--)
        {
            printf(" ");
        }

        // Left side tower
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }

        // Middle space
        printf("  ");

        // Right side tower
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
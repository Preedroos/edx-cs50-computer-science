#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int card_num_arr[16];
    int remainder, length = 0;
    bool check = false;
    long card_num = get_long("Please, enter the card number: ");
    long card_num_cp = card_num;

    // Adding card_number to an array AND counting its length
    while (card_num_cp > 0)
    {
        remainder = card_num_cp % 10;
        card_num_arr[length] = remainder;
        card_num_cp /= 10;
        length += 1;
    }

    if (length != 16 && length != 13 && length != 15)
    {
      printf("INVALID\n");
      return 0;
    }

    // multiplying each of the underlined digits by 2, starting with the number’s second-to-last digit
    int counter = 1, sum_2nd_to_last = 0, something;
    while (counter < length)
    {
        something = (card_num_arr[counter] * 2) % 10;
        sum_2nd_to_last += ((card_num_arr[counter] * 2) / 10) + something;
        counter += 2;
    }

    int sum_1st_to_last = 0;
    counter = 0;
    while (counter < length)
    {
        sum_1st_to_last += card_num_arr[counter];
        counter += 2;
    }

    // Adding the sum above to the sum of the digits that weren’t multiplied by 2
    int sum = sum_2nd_to_last + sum_1st_to_last;
    if (sum % 10 == 0)
    {
        check = true;
    }

    if (check == true)
    {
        if (card_num_arr[length - 1] == 4)
        {
            printf("VISA\n");
        }
        else if (card_num_arr[length - 1] == 5)
        {
            if (card_num_arr[length - 2] == 1 || card_num_arr[length - 2] == 2 || card_num_arr[length - 2] == 3
                || card_num_arr[length - 2] == 4 || card_num_arr[length - 2] == 5)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (card_num_arr[length - 1] == 3)
        {
            if (card_num_arr[length - 2] == 4 || card_num_arr[length - 2] == 7)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
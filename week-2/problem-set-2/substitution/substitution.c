#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Declaration of functions
bool check_args(int args);
bool check_key(string key);
bool is_alpha(string key);
bool is_repeated(string key);
string get_cipher_text(string plain_text, string key);

int main(int argc, string argv[])
{
    if (check_args(argc) && check_key(argv[1]))
    {
        string plain_text = get_string("plaintext: ");
        string cipher_text = get_cipher_text(plain_text, argv[1]);
        printf("ciphertext: %s\n", cipher_text);
        return 0;
    }
    else
    {
        return 1;
    }
}

bool check_args(int args)
{
    if (args == 2)
    {
        return true;
    }
    else
    {
        printf("Usage: ./substitution KEY\n");
        return false;
    }
}

bool check_key(string key)
{
    if (is_alpha(key))
    {
        if (strlen(key) == 26)
        {
            if (is_repeated(key))
            {
                return true;
            }
            else
            {
                printf("Key must not contain repeated characters.\n");
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n");
        }
    }
    else
    {
        printf("Key must only contain alphabetic characters.\n");
    }
    return false;
}

bool is_alpha(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }
    }
    return true;
}

bool is_repeated(string key)
{
    char letter = '\0';
    for (int i = 0; i < strlen(key); i++)
    {
        letter = key[i];
        for (int j = i + 1; j < strlen(key); j++)
        {
            if (toupper(letter) == toupper(key[j]))
            {
                return false;
            }
        }
    }
    return true;
}

string get_cipher_text(string plain_text, string key)
{
    int index;
    string cipher_text = plain_text;
    for (int i = 0; i < strlen(plain_text); i++)
    {
        if (isalpha(plain_text[i]))
        {
            index = (int) plain_text[i];
            if (index >= 65 && index <= 90)
            {
                index -= 65;
                cipher_text[i] = toupper(key[index]);
            }
            else
            {
                index -= 97;
                cipher_text[i] = tolower(key[index]);
            }
        }
    }
    return cipher_text;
}
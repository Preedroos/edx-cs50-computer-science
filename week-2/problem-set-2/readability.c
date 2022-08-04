#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Text input
    string text = get_string("Text: ");

    // Calling the functions to receive sext's letters, words and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculates the approximate grade level needed to comprehend the text
    double L = (double)letters / (double)words * 100;
    double S = (double)sentences / (double)words * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Outputting an exception message if the index is less than 1 or greater than 16
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    // Outputting the grade level only
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int letters = 0, i = 0;
    char letter;
    do
    {
        letter = text[i];
        if (isalpha(letter))
        {
            letters++;
        }
        i++;
    }
    while (i < strlen(text));
    return letters;
}

int count_words(string text)
{
    int words = 1, i = 0;
    char letter;
    do
    {
        letter = text[i];
        if (letter == ' ')
        {
            words++;
        }
        i++;
    }
    while (i < strlen(text));
    return words;
}

int count_sentences(string text)
{
    int sentences = 0, i = 0;
    char letter;
    do
    {
        letter = text[i];
        if (letter == '!' || letter == '.' || letter == '?')
        {
            sentences++;
        }
        i++;
    }
    while (i < strlen(text));
    return sentences;
}
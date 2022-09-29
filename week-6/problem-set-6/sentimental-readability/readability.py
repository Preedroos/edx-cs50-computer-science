def main():

    # Text input
    text = input('Text: ')

    # Calling the functions to receive sext's letters, words and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculates the approximate grade level needed to comprehend the text
    L = letters / words * 100
    S = sentences / words * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Outputting an exception message if the index is less than 1 or greater than 16
    if index < 1:
        print('Before Grade 1\n')
    elif index > 16:
        print('Grade 16+\n')

    # Outputting the grade level only
    else:
        print(f'Grade {index}\n')


def count_letters(text: str):
    return sum(1 for i in range(len(text)) if text[i].isalpha())


def count_words(text: str):
    return sum(1 for i in range(len(text)) if text[i] == ' ') + 1


def count_sentences(text: str):
    end = ['?', '.', '!']
    return sum(1 for i in range(len(text)) if text[i] in end)


main()

def main():
    # Prompts the user for a credit card number
    card_number = input('Number: ')

    # Determines if a credit card number is not (syntactically) valid
    if not luhns_algorithm(card_number):
        print('INVALID\n')
        exit(1)

    # Determines which company the credit card belongs to
    if not valid_company(card_number):
        print('INVALID\n')
        exit(1)

    exit(0)


def luhns_algorithm(card_num):
    sum = 0

    # Reverse str to simplify the for loop
    card_num = card_num[::-1]

    for i in range(len(card_num)):

        # Multiply every other digit by 2, starting with the number’s second-to-last digit
        if i % 2 != 0:
            product = int(card_num[i]) * 2
            sum += (product // 10) + (product % 10)

        # Add the sum to the sum of the digits that weren’t multiplied by 2
        else:
            sum += int(card_num[i])

    # If the total’s last digit is 0, the number is valid
    return sum % 10 == 0


def valid_company(card_num):
    # AMEX
    if len(card_num) == 15:
        if card_num[:2] == '34' or card_num[:2] == '37':
            print('AMEX\n')
            return True

    # MASTERCARD OR VISA
    elif len(card_num) == 16:

        # MASTERCARD
        if int(card_num[:2]) in range(51, 56):
            print('MASTERCARD\n')
            return True

        # VISA
        elif card_num[0] == '4':
            print('VISA\n')
            return True

    # VISA
    elif len(card_num) == 13:
        if card_num[0] == '4':
            print('VISA\n')
            return True

    return False


main()

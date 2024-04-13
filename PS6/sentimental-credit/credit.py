# Function to check if a credit card number is valid
def is_valid(ccn):
    length = len(str(ccn))
    return (length in [13, 15, 16]) and checksum(ccn)


# Function to find the length of a number (number of digits)
def find_length(n):
    return len(str(n))


# Function to perform Luhn's algorithm and validate the checksum of a credit card number
def checksum(ccn):
    ccn_str = str(ccn)
    sum = 0
    for i in range(len(ccn_str) - 1, -1, -1):
        digit = int(ccn_str[i])
        if (len(ccn_str) - i) % 2 == 0:
            digit *= 2
            if digit > 9:
                digit = digit // 10 + digit % 10
        sum += digit
    return sum % 10 == 0


# Function to determine the type of credit card based on the starting digits
def print_type(ccn):
    ccn_str = str(ccn)
    if ccn_str.startswith(('34', '37')) and len(ccn_str) == 15:
        print("AMEX")
    elif ccn_str.startswith(('51', '52', '53', '54', '55')) and len(ccn_str) == 16:
        print("MASTERCARD")
    elif ccn_str.startswith('4') and (len(ccn_str) in [13, 16]):
        print("VISA")
    else:
        print("INVALID", end="\n")


# Main function
def main():
    # Prompt user for credit card number, ensuring it's non-negative
    credit_number = int(input("Number: "))
    # Check if the credit card number is valid
    if is_valid(credit_number):
        # Print the type of credit card
        print_type(credit_number)
    else:
        # If the credit card number is invalid, print "INVALID"
        print("INVALID")


if __name__ == "__main__":
    main()

#include <cs50.h>
#include <stdio.h>

// Function prototypes
void print_type(long long ccn);
bool is_valid(long long ccn);
int find_length(long long ccn);
bool checksum(long long ccn);

int main(void)
{
    long long credit_number;

    // Prompt user for credit card number, ensuring it's non-negative
    do
    {
        credit_number = get_long_long("Number : ");
    }
    while (credit_number < 0);

    // Check if the credit card number is valid
    if (is_valid(credit_number))
    {
        // Print the type of credit card
        print_type(credit_number);
    }
    else
    {
        // If the credit card number is invalid, print "INVALID"
        printf("INVALID\n");
    }

    return 0;
}

// Function to check if a credit card number is valid
bool is_valid(long long ccn)
{
    int len = find_length(ccn);

    // Check if the length of the credit card number is valid (13, 15, or 16 digits)
    // and if the checksum is valid
    return (len == 13 || len == 15 || len == 16) && checksum(ccn);
}

// Function to find the length of a number (number of digits)
int find_length(long long n)
{
    int digitCount = 0;

    // Count the number of digits by dividing by 10 until n becomes 0
    while (n != 0)
    {
        n /= 10;
        digitCount++;
    }

    return digitCount;
}

// Function to perform Luhn's algorithm and validate the checksum of a credit card number
bool checksum(long long ccn)
{
    int sum = 0;

    // Iterate through each digit of the credit card number
    for (int i = 0; ccn != 0; ccn /= 10, i++)
    {
        // If the current digit index is even, add it to the sum
        if (i % 2 == 0)
        {
            sum += ccn % 10;
        }
        // If the current digit index is odd, double the digit and add its digits to the sum
        else
        {
            int digit = 2 * (ccn % 10);
            sum += digit / 10 + digit % 10;
        }
    }

    // If the sum modulo 10 is 0, the checksum is valid
    return (sum % 10) == 0;
}

// Function to determine the type of credit card based on the starting digits
void print_type(long long ccn)
{
    if ((ccn >= 34e13 && ccn < 35e13) || (ccn >= 37e13 && ccn < 38e13))
    {
        printf("AMEX\n");
    }
    else if (ccn >= 51e14 && ccn < 56e14)
    {
        printf("MASTERCARD\n");
    }
    else if ((ccn >= 4e12 && ccn < 5e12) || (ccn >= 4e15 && ccn < 5e15))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    return;
}

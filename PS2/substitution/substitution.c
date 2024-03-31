#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int check_key(int ac, string key);

int main(int argc, string argv[])
{
    if (check_key(argc, argv[1]) != 0)
    {
        return 1;
    }
    string p = get_string("plaintext:  ");
    string c = p;
    for (int i = 0; p[i] != '\0'; i++)
    {
        if (isalpha(p[i]))
        {
            int index = toupper(p[i]) - 'A';
            if (islower(p[i]))
            {
                c[i] = tolower(argv[1][index]);
            }
            else
            {
                c[i] = toupper(argv[1][index]);
            }
        }
    }
    printf("ciphertext: %s\n", c);
    return 0;
}

int check_key(int ac, string key)
{
    int i = 0, c = 0;
    int seen[26] = {0};
    if (ac != 2)
    {
        printf("Usage: %s key\nERROR : 1\n", "./substitution ");
        return 1;
    }
    while (key[i] != '\0')
    {
        if (isalpha(key[i]))
        {
            int index = toupper(key[i]) - 'A';
            if (seen[index] == 1)
            {
                printf("Key should not contain duplicate characters !\n");
                return 2;
            }
            seen[index] = 1;
            c++;
        }
        else
        {
            printf("Key should contains only alphabetic characters ! \nERROR : 2\n");
            return 3;
        }
        i++;
    }
    if (c == 26)
    {
        return 0;
    }
    else
    {
        printf("Key must contain 26 characters ! \nERROR : 3\n");
        return 4;
    }
}

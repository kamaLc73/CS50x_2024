// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Choose number of buckets in hash table
const unsigned int N = 101;

// Hash table
node *table[N];

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL) // Check if file opened successfully
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Buffer to store each word read from the dictionary
    char word[LENGTH + 1];

    // Read each word in the file
    while (fscanf(source, "%s", word) != EOF)
    {

        // Check if the word is already in the dictionary
        if (check(word))
        {
            continue; // Skip duplicate words
        }

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(source);
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;

        // Hash the word to determine the bucket
        unsigned int index = hash(word);
        n->next = table[index];
        table[index] = n;
    }
    // Close the dictionary file
    fclose(source);
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_value = 31 * hash_value + word[i];
    }

    return hash_value % N;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    node *ptr = NULL;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        while (ptr != NULL)
        {
            count++;
            ptr = ptr->next;
        }
    }
    return count;
}

// Returns true if word is in dictionary, else false (case-insensitive)
bool check(const char *word)
{
    // Convert word to lowercase
    char lowercase_word[LENGTH + 1];
    for (int i = 0; word[i] != '\0'; i++)
    {
        lowercase_word[i] = tolower(word[i]);
    }
    lowercase_word[strlen(word)] = '\0';

    // Iterate over hash table
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        // Traverse linked list
        while (ptr != NULL)
        {
            // Convert dictionary word to lowercase
            char lowercase_dict_word[LENGTH + 1];
            for (int j = 0; ptr->word[j] != '\0'; j++)
            {
                lowercase_dict_word[j] = tolower(ptr->word[j]);
            }
            lowercase_dict_word[strlen(ptr->word)] = '\0';

            // Compare lowercase versions of words
            if (strcmp(lowercase_dict_word, lowercase_word) == 0)
            {
                return true; // If found, return true
            }
            ptr = ptr->next;
        }
    }
    return false; // If not found, return false
}

// Unloads recursively
void unload_recursive(node *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    unload_recursive(ptr->next);
    free(ptr);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        unload_recursive(table[i]);
        table[i] = NULL;
    }
    return true;
}

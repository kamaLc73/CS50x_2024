#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Function prototype
int calculate_score(string word);

int main(int argc, string argv[])
{
    // Get words from players
    string word1 = get_string("Player 1 : ");
    string word2 = get_string("Player 2 : ");

    // Calculate scores for both words
    int score1 = calculate_score(word1);
    int score2 = calculate_score(word2);

    // Determine the winner or if it's a tie
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
    return 0;
}

// Function to calculate the score of a word
int calculate_score(string word)
{
    int score = 0;
    // Array to store the score for each letter
    int array_score[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                           1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
                          };
    // Iterate through each character in the word
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i])) // Check if the character is a letter
        {
            // Calculate the score for the letter and add it to the total score
            int index = toupper(word[i]) -
                        'A'; // Convert the letter to uppercase and find its index in the array
            score += array_score[index]; // Add the score for the letter to the total score
        }
    }
    return score;
}

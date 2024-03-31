#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float calculate_index(string text);
void grade_level(float index);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: \n");

    printf("letters : %d\n", count_letters(text));

    printf("words : %d\n", count_words(text));

    printf("sentences : %d\n", count_sentences(text));

    // Compute the Coleman-Liau index
    float index = calculate_index(text);

    // Print the grade level
    grade_level(index);
    return 0;
}

int count_letters(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int count = 0;
    bool in_word = false;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]) || text[i] == '\'' ||
            text[i] == '-') // Consider single quote as part of word
        {
            if (!in_word)
            {
                count++;
                in_word = true;
            }
        }
        else
        {
            in_word = false;
        }
    }
    return count;
}

int count_sentences(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}

float calculate_index(string text)
{
    int w = count_words(text);
    int l = count_letters(text);
    int s = count_sentences(text);

    float L = ((float) l / (float) w) * 100;
    float S = ((float) s / (float) w) * 100;
    // index = 0.0588 * L - 0.296 * S - 15.8
    return 0.0588 * L - 0.296 * S - 15.8;
}

void grade_level(float index)
{
    // Print grade level of the text
    int grade = round(index);
    if (grade < 1)
    {
        printf("\nBefore Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("\nGrade 16+\n");
    }
    else
    {
        printf("\nGrade %d\n", grade);
    }
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int MAGIC_NUMBER = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Create a buffer for a block of data
    BYTE buffer[MAGIC_NUMBER];

    // Variables to keep track of current JPEG file and counter
    FILE *current_jpeg = NULL;
    int jpeg_counter = 0;
    char filename[20]; // Adjusted buffer size to accommodate the filename

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, MAGIC_NUMBER, card) == MAGIC_NUMBER)
    {
        // Check if this block starts a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            ((buffer[3] & 0xf0) == 0xe0))
        {
            // If we're already writing to a JPEG file, close it
            if (current_jpeg != NULL)
            {
                fclose(current_jpeg);
            }

            // Create a new JPEG file
            sprintf(filename, "./photos/%03i.jpg", jpeg_counter++);
            current_jpeg = fopen(filename, "w");

            // Check if file creation was successful
            if (current_jpeg == NULL)
            {
                fclose(card);
                fprintf(stderr, "Could not create output JPEG file\n");
                return 1;
            }

            // Write the block to the new JPEG file
            fwrite(buffer, 1, MAGIC_NUMBER, current_jpeg);
        }
        else
        {
            // Continue writing to the current JPEG file
            if (current_jpeg != NULL)
            {
                fwrite(buffer, 1, MAGIC_NUMBER, current_jpeg);
            }
        }
    }

    // Close any remaining open files
    if (current_jpeg != NULL)
    {
        fclose(current_jpeg);
    }
    fclose(card);

    return 0;
}

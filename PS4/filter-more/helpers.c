#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average of RGB values
            int avg =
                round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);

            // Set each color channel to the average
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary image to store blurred pixels
    RGBTRIPLE blurred[height][width];

    // Iterate through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Iterate through pixels in 3x3 grid centered at (i, j)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int new_i = i + di;
                    int new_j = j + dj;

                    // Check if pixel is within bounds
                    if (new_i >= 0 && new_i < height && new_j >= 0 && new_j < width)
                    {
                        // Accumulate color values
                        sumRed += image[new_i][new_j].rgbtRed;
                        sumGreen += image[new_i][new_j].rgbtGreen;
                        sumBlue += image[new_i][new_j].rgbtBlue;
                        count++;
                    }
                }
            }
            // Set blurred pixel to average color value
            blurred[i][j].rgbtRed = round((float) sumRed / count);
            blurred[i][j].rgbtGreen = round((float) sumGreen / count);
            blurred[i][j].rgbtBlue = round((float) sumBlue / count);
        }
    }
    // Copy blurred image back to original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Sobel operators for edge detection
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Temporary image to store edge-detected pixels
    RGBTRIPLE edged[height][width];

    // Iterate through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxRed = 0, GxGreen = 0, GxBlue = 0;
            int GyRed = 0, GyGreen = 0, GyBlue = 0;

            // Iterate through pixels in 3x3 grid centered at (i, j)
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int new_i = i + di;
                    int new_j = j + dj;

                    // Check if pixel is within bounds
                    if (new_i >= 0 && new_i < height && new_j >= 0 && new_j < width)
                    {
                        // Apply Sobel operators
                        GxRed += Gx[di + 1][dj + 1] * image[new_i][new_j].rgbtRed;
                        GxGreen += Gx[di + 1][dj + 1] * image[new_i][new_j].rgbtGreen;
                        GxBlue += Gx[di + 1][dj + 1] * image[new_i][new_j].rgbtBlue;

                        GyRed += Gy[di + 1][dj + 1] * image[new_i][new_j].rgbtRed;
                        GyGreen += Gy[di + 1][dj + 1] * image[new_i][new_j].rgbtGreen;
                        GyBlue += Gy[di + 1][dj + 1] * image[new_i][new_j].rgbtBlue;
                    }
                }
            }

            // Calculate gradient magnitude
            int magnitudeRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int magnitudeGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int magnitudeBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            // Cap magnitude values at 255
            magnitudeRed = magnitudeRed > 255 ? 255 : magnitudeRed;
            magnitudeGreen = magnitudeGreen > 255 ? 255 : magnitudeGreen;
            magnitudeBlue = magnitudeBlue > 255 ? 255 : magnitudeBlue;

            // Set edge-detected pixel color
            edged[i][j].rgbtRed = magnitudeRed;
            edged[i][j].rgbtGreen = magnitudeGreen;
            edged[i][j].rgbtBlue = magnitudeBlue;
        }
    }

    // Copy edge-detected image back to original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged[i][j];
        }
    }
}

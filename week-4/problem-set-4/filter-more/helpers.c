#include "helpers.h"
#include <math.h>

void reset_grid(int gxRed[3][3], int gyRed[3][3], int gxGreen[3][3], int gyGreen[3][3], int gxBlue[3][3], int gyBlue[3][3]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float sum;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum = image[i][j].rgbtRed;
            sum += image[i][j].rgbtGreen;
            sum += image[i][j].rgbtBlue;
            sum = round(sum / 3);
            image[i][j].rgbtRed = sum;
            image[i][j].rgbtGreen = sum;
            image[i][j].rgbtBlue = sum;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            tmp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp_image[height][width];
    float red, green, blue, count;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            count = 1;
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;

            if (i > 0)
            {
                count++;
                red += image[i - 1][j].rgbtRed;
                green += image[i - 1][j].rgbtGreen;
                blue += image[i - 1][j].rgbtBlue;
            }

            if (i < (height - 1))
            {
                count++;
                red += image[i + 1][j].rgbtRed;
                green += image[i + 1][j].rgbtGreen;
                blue += image[i + 1][j].rgbtBlue;
            }
            if (j > 0)
            {
                count++;
                red += image[i][j - 1].rgbtRed;
                green += image[i][j - 1].rgbtGreen;
                blue += image[i][j - 1].rgbtBlue;

                if (i > 0)
                {
                    count++;
                    red += image[i - 1][j - 1].rgbtRed;
                    green += image[i - 1][j - 1].rgbtGreen;
                    blue += image[i - 1][j - 1].rgbtBlue;
                }
                if (i < (height - 1))
                {
                    count++;
                    red += image[i + 1][j - 1].rgbtRed;
                    green += image[i + 1][j - 1].rgbtGreen;
                    blue += image[i + 1][j - 1].rgbtBlue;
                }
            }

            if (j < (width - 1))
            {
                count++;
                red += image[i][j + 1].rgbtRed;
                green += image[i][j + 1].rgbtGreen;
                blue += image[i][j + 1].rgbtBlue;

                if (i > 0)
                {
                    count++;
                    red += image[i - 1][j + 1].rgbtRed;
                    green += image[i - 1][j + 1].rgbtGreen;
                    blue += image[i - 1][j + 1].rgbtBlue;
                }

                if (i < (height - 1))
                {
                    count++;
                    red += image[i + 1][j + 1].rgbtRed;
                    green += image[i + 1][j + 1].rgbtGreen;
                    blue += image[i + 1][j + 1].rgbtBlue;
                }
            }

            tmp_image[i][j].rgbtRed = round(red / count);
            tmp_image[i][j].rgbtGreen = round(green / count);
            tmp_image[i][j].rgbtBlue = round(blue / count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp_image[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp_image[height][width];
    int gxRed[3][3], gyRed[3][3], gxGreen[3][3], gyGreen[3][3], gxBlue[3][3], gyBlue[3][3];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            reset_grid(gxRed, gyRed, gxGreen, gyGreen, gxBlue, gyBlue);

            if (i > 0)
            {
                gyRed[0][1] = image[i - 1][j].rgbtRed * -2;
                gyGreen[0][1] = image[i - 1][j].rgbtGreen * -2;
                gyBlue[0][1] = image[i - 1][j].rgbtBlue * -2;
            }

            if (i < (height - 1))
            {
                gyRed[2][1] = image[i + 1][j].rgbtRed * 2;
                gyGreen[2][1] = image[i + 1][j].rgbtGreen * 2;
                gyBlue[2][1] = image[i + 1][j].rgbtBlue * 2;
            }
            if (j > 0)
            {
                gxRed[1][0] = image[i][j - 1].rgbtRed * -2;
                gxGreen[1][0] = image[i][j - 1].rgbtGreen * -2;
                gxBlue[1][0] = image[i][j - 1].rgbtBlue * -2;

                if (i > 0)
                {
                    gxRed[0][0] = image[i - 1][j - 1].rgbtRed * -1;
                    gyRed[0][0] = image[i - 1][j - 1].rgbtRed * -1;
                    gxGreen[0][0] = image[i - 1][j - 1].rgbtGreen * -1;
                    gyGreen[0][0] = image[i - 1][j - 1].rgbtGreen * -1;
                    gxBlue[0][0] = image[i - 1][j - 1].rgbtBlue * -1;
                    gyBlue[0][0] = image[i - 1][j - 1].rgbtBlue * -1;
                }
                if (i < (height - 1))
                {
                    gxRed[2][0] = image[i + 1][j - 1].rgbtRed * -1;
                    gyRed[2][0] = image[i + 1][j - 1].rgbtRed * 1;
                    gxGreen[2][0] = image[i + 1][j - 1].rgbtGreen * -1;
                    gyGreen[2][0] = image[i + 1][j - 1].rgbtGreen * 1;
                    gxBlue[2][0] = image[i + 1][j - 1].rgbtBlue * -1;
                    gyBlue[2][0] = image[i + 1][j - 1].rgbtBlue * 1;
                }
            }

            if (j < (width - 1))
            {
                gxRed[1][2] = image[i][j + 1].rgbtRed * 2;
                gxGreen[1][2] = image[i][j + 1].rgbtGreen * 2;
                gxBlue[1][2] = image[i][j + 1].rgbtBlue * 2;

                if (i > 0)
                {
                    gxRed[0][2] = image[i - 1][j + 1].rgbtRed * 1;
                    gyRed[0][2] = image[i - 1][j + 1].rgbtRed * -1;
                    gxGreen[0][2] = image[i - 1][j + 1].rgbtGreen * 1;
                    gyGreen[0][2] = image[i - 1][j + 1].rgbtGreen * -1;
                    gxBlue[0][2] = image[i - 1][j + 1].rgbtBlue * 1;
                    gyBlue[0][2] = image[i - 1][j + 1].rgbtBlue * -1;
                }

                if (i < (height - 1))
                {
                    gxRed[2][2] = image[i + 1][j + 1].rgbtRed * 1;
                    gyRed[2][2] = image[i + 1][j + 1].rgbtRed * 1;
                    gxGreen[2][2] = image[i + 1][j + 1].rgbtGreen * 1;
                    gyGreen[2][2] = image[i + 1][j + 1].rgbtGreen * 1;
                    gxBlue[2][2] = image[i + 1][j + 1].rgbtBlue * 1;
                    gyBlue[2][2] = image[i + 1][j + 1].rgbtBlue * 1;
                }
            }

            int sumXRed = 0, sumYRed = 0, sumXGreen = 0, sumYGreen = 0, sumXBlue = 0, sumYBlue = 0;
            for (int l = 0; l < 3; l++)
            {
                for (int k = 0; k < 3; k++)
                {
                    sumXRed += gxRed[l][k];
                    sumYRed += gyRed[l][k];
                    sumXGreen += gxGreen[l][k];
                    sumYGreen += gyGreen[l][k];
                    sumXBlue += gxBlue[l][k];
                    sumYBlue += gyBlue[l][k];
                }
            }

            int rgbtRed = round(sqrt(sumXRed * sumXRed + sumYRed * sumYRed));
            int rgbtGreen = round(sqrt(sumXGreen * sumXGreen + sumYGreen * sumYGreen));
            int rgbtBlue = round(sqrt(sumXBlue * sumXBlue + sumYBlue * sumYBlue));

            if (rgbtRed > 255)
            {
                rgbtRed = 255;
            }
            if (rgbtGreen > 255)
            {
                rgbtGreen = 255;
            }
            if (rgbtBlue > 255)
            {
                rgbtBlue = 255;
            }

            tmp_image[i][j].rgbtRed = rgbtRed;
            tmp_image[i][j].rgbtGreen = rgbtGreen;
            tmp_image[i][j].rgbtBlue = rgbtBlue;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp_image[i][j];
        }
    }
    return;
}

void reset_grid(int gxRed[3][3], int gyRed[3][3], int gxGreen[3][3], int gyGreen[3][3], int gxBlue[3][3], int gyBlue[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gxRed[i][j] = 0;
            gyRed[i][j] = 0;
            gxGreen[i][j] = 0;
            gyGreen[i][j] = 0;
            gxBlue[i][j] = 0;
            gyBlue[i][j] = 0;
        }
    }
    return;
}
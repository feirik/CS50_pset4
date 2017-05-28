/**
 * Resizes a BMP by a floating factor.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    if(atof(argv[1]) <= 0 || atof(argv[1]) > 100)
    {
        fprintf(stderr, "Resize factor 'n' needs to be between 0 and 100\n");
        return 0;
    }
    
    // Halving START
    if(atof(argv[1]) >= 0.26 && atof(argv[1]) < 0.74)
    {
        float h = 0.5;
        
        // remember filenames
        char *infile = argv[2];
        char *outfile = argv[3];
    
        // open input file 
        FILE *inptr = fopen(infile, "r");
        if (inptr == NULL)
        {
            fprintf(stderr, "Could not open %s.\n", infile);
            return 2;
        }
    
        // open output file
        FILE *outptr = fopen(outfile, "w");
        if (outptr == NULL)
        {
            fclose(inptr);
            fprintf(stderr, "Could not create %s.\n", outfile);
            return 3;
        }
    
        // read infile's BITMAPFILEHEADER
        BITMAPFILEHEADER bf, bf2;
        fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
        
        bf2 = bf;
    
        // read infile's BITMAPINFOHEADER
        BITMAPINFOHEADER bi, bi2;
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
        
        bi2 = bi;
    
        // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
        if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
            bi.biBitCount != 24 || bi.biCompression != 0)
        {
            fclose(outptr);
            fclose(inptr);
            fprintf(stderr, "Unsupported file format.\n");
            return 4;
        }
        
        // New dimensions
        bi2.biWidth = bi.biWidth * h;
        bi2.biHeight = bi.biHeight * h;
        
        // determine padding for infile
        int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        
        // padding, outfile
        int newPadding = (4 - (bi2.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        
        bi2.biSizeImage = ((sizeof(RGBTRIPLE) * bi2.biWidth) + newPadding) * abs(bi2.biHeight);
        
        bf2.bfSize = bi2.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        
        // write outfile's BITMAPFILEHEADER
        fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);

        // write outfile's BITMAPINFOHEADER
        fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);
    
        
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // iterate over width of infile
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
        
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                // writes every second RGB triple to outfile
                if(j == 0 || j%2 == 0)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // adds outfile padding
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // skips infile padding
            fseek(inptr, padding, SEEK_CUR);
            
            // skips every second infile scanline as bmp is halved
            fseek(inptr, (3*bi.biWidth+padding), SEEK_CUR);
        }
    
        // close infile
        fclose(inptr);
    
        // close outfile
        fclose(outptr);
    
        // success
        return 0;
        
    }
    
    // HALVING END
    
    //Resize factor
    int f = round(atof(argv[1]));
    
    //  printf("Float factor is: %i\n", f);
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf2;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    bf2 = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi2;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    bi2 = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // New dimensions
    bi2.biWidth = bi.biWidth * f;
    bi2.biHeight = bi.biHeight * f;
    
    // determine padding for scanlines, in bmp
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // padding, out bmp
    int newPadding = (4 - (bi2.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    
    bi2.biSizeImage = ((sizeof(RGBTRIPLE) * bi2.biWidth) + newPadding) * abs(bi2.biHeight);
    
    bf2.bfSize = bi2.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for(int newLines = 0; newLines < f; newLines++)
        {
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile
                for(int k = 0; k < f; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            for (int k = 0; k < newPadding; k++)
            {
            fputc(0x00, outptr);
            }
            
            fseek(inptr, padding, SEEK_CUR);
            if(newLines < f-1)
            {
            fseek(inptr, -(3*bi.biWidth+padding), SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

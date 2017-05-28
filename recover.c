/*
Extract jpeg pictures from a .raw file
*/

#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Checking for 1 input argument
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
    
    // Storing input file name
    char *infile = argv[1];
    
    // Declaring variables
    FILE *outptr = NULL;
    BYTE buffer[512];
    char newFile[10];
    sprintf(newFile,"000.jpg");
    
    // Openings first .jpeg file
    outptr = fopen(newFile,"w");
    
    // Start count for .jpeg numbering and boolean flag for initialization
    int jpegCount = 1;
    int firstFlag = 1;
    
    // Opening .raw, error if not possible to open
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // Looping until end of file break condition
    while(1)
    {
        // Reading from infile to buffer, ending loop if reading is unsuccessful
        if(fread(&buffer, 1, 512, inptr) != 512)
        {
            break;
        }
        
        // Checking for start of .jpeg
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If new jpeg start, close outfile, rename filename, open outfile again
            if(!firstFlag)
            {
                fclose(outptr);
                sprintf(newFile,"%03i.jpg",jpegCount++);
                outptr = fopen(newFile,"w");
            }
            firstFlag = 0;
        }
        // Writes from buffer to outfile, only after .jpeg start has been identified
        if(!firstFlag)
        {
            fwrite(&buffer, 1, 512, outptr);
        }
    }
    // Closing files
    fclose(inptr);
    fclose(outptr);
    
    return 0;
}

/*=============================================================================
| Assignment: pa02 - Calculate the checksum of an input file given:
| -> the name of the input file,
| -> the checksum size of either 8, 16, or 32 bits
| Author: Hailey Simpson
| Language: c
| To Compile: gcc -o pa02 pa02.c
| To Execute: ./pa02 inputFilename.txt checksumSize
| where inputFilename.txt is the input file
| and checksumSize is either 8, 16, or 32
| Note:
| All input files are simple 8 bit ASCII input
| All execute commands above have been tested on Eustis
| Class: CIS3360 - Security in Computing - Fall 2026
| Instructor: McAlpin
| Due Date: 10/25/26
+===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) 
{
    FILE *file;                 // File pointer for the input file
    int ch;                     // Stores one character read from the file at a time
    int char_cnt = 0;           // Counts the characters proccessed 
    int line_cnt = 0;           // Counts the characters printed on the currrent output line 
    int byte_cnt = 0;           // Counts the bytes stored in a checksum group
    unsigned long checksum = 0; // Stores the checksum total
    unsigned long group = 0;    // Builds a group of bytes for a 16 bit or 32 bit checksum
    unsigned long mask;         // Stores a bit mask to keep only the required checksum bits

    // Checks command line arguments
    if (argc != 3) 
    {
        // If invalid, print the correct command format to the error stream and end the program
        fprintf(stderr, "Usage: %s filename checksumSize\n", argv[0]);
        return 1;
    }
    
    // Converts the checksum size argument from text to an integer and store it in checksumSize
    int checksumSize = atoi(argv[2]);
    // Checks whether the requested checksum size is valid
    if (checksumSize != 8 && checksumSize != 16 && checksumSize != 32) 
    {
        // If invalid, tell the user which checksum sizes are allowed and end the program
        fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
        return 1;
    }

    // Opens file for reading in binary mode
    file = fopen(argv[1], "rb");
    // Checks whether the file failed to open 
    if (file == NULL)
    {
        // If failed to open, print an error message and end the program
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    // Chooses a mask based on the requested checksum size
    if (checksumSize == 8) 
    {
        mask = 0xFF;       // Keeps the lowest 8 bits
    }
    else if (checksumSize == 16) 
    {
        mask = 0xFFFF;     // Keeps the lowest 16 bits
    }
    else
    {
        mask = 0xFFFFFFFF; // Keeps the lowest 32 bits
    }
    // Prints a blank line before echoing the input
    putchar('\n');

    // Reads and processes the file one character at a time until the end
    while ((ch = fgetc(file)) != EOF) 
    {
        // Echoes the character and tracks the current output line length
        if (ch != '\n') 
        {
            putchar(ch); // Prints the character read from the file
            line_cnt++;  // Increases line's character count
        }
        else 
        {
            putchar(ch);  // Prints the newline character from the file
            line_cnt = 0; // Resets the line length after a newline
        }
        // Increases the character count
        char_cnt++;

        // Starts a new output line after 80 printed non-newline characters
        if (line_cnt == 80)
        {
             
            putchar('\n'); // Prints the newline after 80 characters
            line_cnt = 0;  // Resets the line length 
        }

        if (checksumSize == 8) 
        {
            // Adds the character's numeric byte value
            checksum += ch; 
        }
        else 
        { 
            // Shifts existing bytes to the left and adds the next byte on the right
            group = (group << 8) | ch;
            // Increases the bytes count
            byte_cnt++;                 

            // Checks whether the group contains enough bytes 
            if (byte_cnt == checksumSize / 8)
            {
                checksum += group; // Adds the completed group to the checksum
                group = 0;         // Clears the group for the next bytes
                byte_cnt = 0;      // Resets the number of bytes in the group
            }
        }
    }
    // Closes the input file after reading it
    fclose(file);

    // Handles a final group does not contain enough bytes
    if (byte_cnt > 0)
    {
        // Adds ASCII 'X' bytes until the group is full
        while (byte_cnt < checksumSize / 8)
        {
            group = (group << 8) | 'X'; // Adds an 'X' byte to the group
            putchar('X');               // Prints the padding character
            char_cnt++;                 // Increases the character count
            byte_cnt++;                 // Increases the bytes count
        }
        // Adds the completed, padded group to the checksum
        checksum += group;
    }
    // Keeps only the lowest 8, 16, or 32 bits
    checksum &= mask;

    // Prints a blank line before the checksum result
    putchar('\n');
    // Prints the checksum size, hexadecimal checksum, and character count
    printf("%2d bit checksum is %8lx for all %4d chars\n", checksumSize, checksum, char_cnt);

    // Ends the program
    return 0;
}

/*=============================================================================
| I Hailey Simpson (ha078991) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
| I also affirm that I built, developed, and tested this code without using AI
| to build functions, scripts, or other elements of the submitted code.
+=============================================================================*/
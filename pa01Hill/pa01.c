/*============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Hill cipher
|
| Author: Hailey Simpson
| Language: c
| To Compile: gcc -o pa01 pa01.c
| To Execute: ./pa01 kX.txt pX.txt
| where kX.txt is the keytext file input
| and pX.txt is plaintext file input
| Note:
| All input files are simple 8 bit ASCII input
| All execute commands above have been tested on Eustis
|
| Class: CIS3360 - Security in Computing - Fall 2026
| Instructor: McAlpin
| Due Date: 09/20/26
+===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TEXT 10000  // Max size of plaintext
#define MAX_SIZE 9      // Max size of key matrix

int main(int argc, char *argv[])
{
    // File pointers for the key and plaintext files
    FILE *keyFile, *plaintextFile;  
    // Arrays for the key matrix, plaintext and ciphertext                
    int key[MAX_SIZE][MAX_SIZE];
    char plaintext[MAX_TEXT], ciphertext[MAX_TEXT]; 

    // Check command line arguments
    if (argc != 3) 
    {
        return 1;
    }

    // Open key file and plaintext file
    keyFile = fopen(argv[1], "r");
    plaintextFile = fopen(argv[2], "r");
    // Stop the program if either file can not be opened
    if (keyFile == NULL || plaintextFile == NULL) 
    {
        return 1;
    }

    // Read the size of the key matrix
    int n;
    fscanf(keyFile, "%d", &n);

    // Make sure the key size is between 2x2 and 9x9
    if (n < 2 || n > 9) 
    {
        fclose(keyFile);
        fclose(plaintextFile);
        return 1;
    }

    // Read the key matrix from the key file
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(keyFile, "%d", &key[i][j]);
        }
    }

    // Read plaintext and keep only alphabetic characters
    int ch = 0;
    int count = 0;
    while ((ch = fgetc(plaintextFile)) != EOF)
    {
        if (isalpha(ch))
        {
            // Uppercase letters are converted to lowercase
            plaintext[count] = tolower(ch);
            count++;
        }
    }

    // The plaintext length must be a multiple of the key size
    // Add x padding when necessary 
    while (count % n != 0)
    {
        plaintext[count] = 'x';
        count++;
    }

    // Encrypt the plaintext using the Hill cipher
    // Process plaintext n letters at a time
    for (int i = 0; i < count; i += n) 
    {
        // Calculate each letter in the ciphertext block
        for (int j = 0; j < n; j++)
        {
            int result = 0;
            
            // Multiply key matrix row by current plaintext block
            for (int k = 0; k < n; k++) 
            {
                result += key[j][k] * (plaintext[i + k] - 'a');
            }

            // Modulo 26 keeps result within the range of the alphabet
            result %= 26;

            // Convert number back into a lowercase letter
            ciphertext[i + j] = result + 'a';
        }
    }

    // Add null terminator to ciphertext array
    ciphertext[count] = '\0';

    // Print key matrix
    printf("\nKey matrix:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) 
        {
            // %4d aligns the matrix values
            printf("%4d", key[i][j]);
        }

        printf("\n");
    }

    // Print plaintext
    printf("\nPlaintext:\n");

    for (int i = 0; i < count; i++) 
    {
        printf("%c", plaintext[i]);

        if ((i + 1) % 80 == 0)
        {
            // Start newline after every 80 characters
            printf("\n");
        }
    }

    // Add newline if the last plaintext line has fewer than 80 characters
    if (count % 80 != 0)
    {
        printf("\n");
    }

    // Print ciphertext
    printf("\nCiphertext:\n");
    
    for (int i = 0; i < count; i++) 
    {
        printf("%c", ciphertext[i]);

        // Start newline after every 80 characters
        if ((i + 1) % 80 == 0)
        {
            printf("\n");
        }
    }

    // Add newline if the last ciphertext line has fewer than 80 characters
    if (count % 80 != 0)
    {
        printf("\n");
    }

    // Close both input files
    fclose(keyFile);
    fclose(plaintextFile);

    return 0;
}

/*=============================================================================
| I Hailey Simpson (5674627) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
| I also affirm that I built, developed, and tested this code without using AI
| to build functions, scripts, or other elements of the submitted code.
+=============================================================================*/
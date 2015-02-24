#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// function prototype
int copyfile1(char* infilename, char* outfilename);
int copyfile2(char* infilename, char* outfilename);

/** cptest.cpp
 * A file copying program.
 * Derived partially from caesar.cpp by Horstmann and Budd from big C++
 */

/**
   Prints usage instructions.
   @param program_name the name of this program
*/
void usage(char* program_name)
{
    printf("Usage: %s infile outfile\n", program_name);
}

/**
   Prints file opening error message
   @param filename the name of the file that could not be opened
*/
void open_file_error(char* filename)
{
    printf("Error opening file %s\n", filename);
}

/** Main program: copies a file.
    @param argc Number of command-line arguments (including program name).
    @param argv Array of pointers to character arays holding arguments.
    @return 0 if successful, 1 if fail.
*/
int main(int argc, char* argv[])
{
    char* infilename; // Names of files.
    char* outfilename;

    if (argc != 3)
    {
        usage(argv[0]); // Must have exactly 2 arguments.
        return 1;
    }

    infilename = argv[1];
    outfilename = argv[2];

    // Perform the copying
    int returnstatus = copyfile2(infilename, outfilename);

    return returnstatus;
}

/** Copies one file to another using formatted I/O, one character at a time.
 @param infilename Name of input file
 @param outfilename Name of output file
 @return 0 if successful, 1 if error.
*/
int copyfile1(char* infilename, char* outfilename)
{
    FILE* infile; //File handles for source and destination.
    FILE* outfile;

    infile = fopen(infilename, "r"); // Open the input and output files.
    if (infile == NULL)
    {
        open_file_error(infilename);
        return 1;
    }

    outfile = fopen(outfilename, "w");
    if (outfile == NULL)
    {
        open_file_error(outfilename);
        return 1;
    }

    int intch;  // Character read from input file. must be an int to catch EOF.
    unsigned char ch; // Character stripped down to a byte.

    // Read each character from the file, checking for EOF.
    while ((intch = fgetc(infile)) != EOF)
    {
        ch = (unsigned char) intch; // Convert to one-byte char.
        fputc(ch, outfile); // Write out.
    }

    // All done--close the files and return success code.
    fclose(infile);
    fclose(outfile);

    return 0; // Success!
}

/**
 * @brief Copies a file from one place to another using the binary file
 * read and write commands.
 *
 * @param infilename The name of the input file to copy.
 * @param outfilename The name of the output file to copy to.
 *
 * @return 0 if success, 1 if error.
 */
int copyfile2(char* infilename, char* outfilename)
{
    int infile = open(infilename, O_RDONLY);
    if (infile < 0)
    {
        printf("Error in cp: %d (%s)\n", errno, strerror(errno));
        return 1;
    }
    int outfile = open(outfilename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR |
                       S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if (outfile < 0)
    {
        printf("Error in cp: %d (%s)\n", errno, strerror(errno));
        return 1;
    }

    //Process file data here.
    char buffer[2048];
    int  read_bytes;
    while ((read_bytes = read(infile, buffer, 2048)) > 0)
    {
        write(outfile, buffer, read_bytes);
    }

    close(infile);
    close(outfile);

    return 1;
}


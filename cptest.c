#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <libgen.h>

// function prototype
int copyfile1(char* infilename, char* outfilename);
int copyfile2(char* infilename, char* outfilename);
int copyfile3(char* infilename, char* outfilename, int bufsize);

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
    printf("Usage: %s infile outfile [copymode = 1, 2, 3] [buffer size]\nCopy mode and buffer size are optional.\n", program_name);
}

/**
   Prints file opening error message
   @param filename the name of the file that could not be opened
*/
void open_file_error(char* filename)
{
    printf("Error opening file %s\n", filename);
}

/**
 * @brief Print the value of a timeval struct.
 *
 * @param tm_ptr Pointer to timeval struct to print.
 */
void printtimeofday(struct timeval * tm_ptr)
{
    printf("%ld seconds, %d microseconds\n", tm_ptr->tv_sec, tm_ptr->tv_usec);
}

/**
 * @brief Get the difference between two timeval structs.
 *
 * @param end The ending time, must be later than the start.
 * @param start The earlier time, must be earlier than the end.
 *
 * @return A pointer to a newly allocated timeval struct containing
 * the time difference.
 */
struct timeval * gettimediff(struct timeval * end, struct timeval * start)
{
    struct timeval * ret = (struct timeval *)malloc(sizeof(struct timeval));
    ret->tv_sec = end->tv_sec - start->tv_sec;
    ret->tv_usec = end->tv_usec - start->tv_usec;
    //Handle case of borrowing.
    if (ret->tv_usec < 0)
    {
        ret->tv_sec -= 1;
        ret->tv_usec += 1000000;
    }
    return ret;
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
    int mode = 1;
    int buffSize;
    struct timeval * tm_ptr_start = (struct timeval*)malloc(sizeof(struct timeval));
    struct timeval * tm_ptr_end = (struct timeval*)malloc(sizeof(struct timeval));
    struct timeval * tm_dt;
    if (argc < 4)
    {
        usage(argv[0]); // Must have exactly 2 arguments.
        return 1;
    }
    infilename = argv[1];

    char* dir;
    char* filename;
    struct stat statbuf;
    stat(argv[2], &statbuf);
    if (S_ISDIR(statbuf.st_mode))
    {
        dir = argv[2];
        filename = basename(infilename);
        outfilename = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(filename) + 2));
        strcat(outfilename, dir);
        strcat(outfilename, "/");
        strcat(outfilename, filename);
        strcat(outfilename, "\0");
    }
    else
    {
        outfilename = argv[2];
    }
    
    mode = atoi(argv[3]);
    if (mode == 3 && argc == 4)
    {
        buffSize = 2048;
    }
    else if (mode == 3 && argc == 5)
    {
        buffSize = atoi(argv[4]);
    }
    gettimeofday(tm_ptr_start, NULL);
    // Perform the copying
    int returnstatus;
    switch (mode)
    {
    case 1:
        returnstatus = copyfile1(infilename, outfilename);
        break;
    case 2:
        returnstatus = copyfile2(infilename, outfilename);
        break;
    case 3:
        returnstatus = copyfile3(infilename, outfilename, buffSize);
        break;
    }
    gettimeofday(tm_ptr_end, NULL);
    printtimeofday(tm_ptr_start);
    printtimeofday(tm_ptr_end);
    tm_dt = gettimediff(tm_ptr_end, tm_ptr_start);
    printf("Time difference: %ld seconds, %d microseconds\n", tm_dt->tv_sec, tm_dt->tv_usec);
    free(tm_ptr_start);
    free(tm_ptr_end);
    free(tm_dt);
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

/**
 * @brief Copies a file from one place to another using the binary file
 * read and write commands.
 *
 * @param infilename The name of the input file to copy.
 * @param outfilename The name of the output file to copy to.
 * @param bufsize The size of the file buffer in memory to use.
 *
 * @return 0 if success, 1 if error.
 */
int copyfile3(char* infilename, char* outfilename, int bufsize)
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
    char buffer[bufsize];
    int  read_bytes;
    while ((read_bytes = read(infile, buffer, bufsize)) > 0)
    {
        write(outfile, buffer, read_bytes);
    }

    close(infile);
    close(outfile);

    return 1;
}

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include "errno.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include "Timer.h"
#include <fcntl.h>

using namespace std;

void slow_copy(char* in, char* out)
{

    ifstream is(in);
    ofstream outfile(out);

    char c;
    while(is.get(c)) {
        outfile.put(c);
    }
    is.close();
}

void med_copy(char* in, char* out)
{
    int size = sizeof(char);
    int input_fd, output_fd;
    ssize_t ret_in, ret_out;
    char buffer[size];

    input_fd = open(in, O_RDONLY);
    if(input_fd == -1) {
        perror("open");
    }

    output_fd = open(out, O_WRONLY | O_CREAT, 0644);
    if(output_fd == -1) {
        perror("open");
    }

    while((ret_in = read(input_fd, &buffer, size)) > 0) {
        ret_out = write(output_fd, &buffer, (ssize_t) ret_in);
        if(ret_out != ret_in) {
            perror("write");
        }
    }

    close (input_fd);
    close (output_fd);

}

void fast_copy(char* in, char* out)
{
    int input_fd, output_fd;
    ssize_t ret_in, ret_out;
    char buffer[BUFSIZ];

    input_fd = open(in, O_RDONLY);
    if(input_fd == -1) {
        perror("open");
    }

    output_fd = open(out, O_WRONLY | O_CREAT, 0644);
    if(output_fd == -1) {
        perror("open");
    }

    while((ret_in = read(input_fd, &buffer, BUFSIZ)) > 0) {
        ret_out = write(output_fd, &buffer, (ssize_t) ret_in);
        if(ret_out != ret_in) {
            perror("write");
        }
    }

    close (input_fd);
    close (output_fd);
}

bool file_exists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}


int main(int argc, char **argv)
{
    if(argc < 2) {
       cout << "ERROR: Please enter a file to copy" << endl;
       return 1;
    }

    if(argc < 3) {
       cout << "ERROR: Please enter the destination file" << endl;
       return 1;
    }

    if(!file_exists(argv[1])) {
        cout << "ERROR: Source file does not exist" << endl;
        return 1;
    }

    if(file_exists(argv[2])) {
       cout << "ERROR: Destination file already exists" << endl;
       return 1;
    }

    if(argc == 4) {
        fast_copy(argv[1], argv[2]);
    }
    else {

        Timer t;
        double eTime;
        t.start();
        slow_copy(argv[1], argv[2]);
        t.elapsedUserTime(eTime);
        cout << "First method took: " << eTime << " seconds" << endl;

        Timer t2;
        double eTime2;
        t2.start();
        med_copy(argv[1], argv[2]);
        t2.elapsedUserTime(eTime2);
        cout << "Second method took: " << eTime2 << " seconds" << endl;

        Timer t3;
        double eTime3;
        t3.start();
        fast_copy(argv[1], argv[2]);
        t3.elapsedUserTime(eTime3);
        cout << "Third method took: " << eTime3 << " seconds" << endl;
    }

    return 0;
}


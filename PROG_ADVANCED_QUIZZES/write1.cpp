#include <iostream>

#include <sys/types.h>

#include <errno.h>

#include <fcntl.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#include <unistd.h>


#include "util_lib_1.h"


using namespace std;

// extern int		randint(int incl_low, int incl_high);
// extern void *		rand_bytes(unsigned int count, unsigned short min_val, unsigned short max_val, char * opt_buf);

class WRITE_PLUS_A
{


    public:

        bool write_own_new(const char * file_path)
        {

            struct stat		sb;

            errno = 0;
            int rval = stat(file_path, &sb);
            
            if (rval == 0 || errno == 0)
            {
                cerr << "File " << file_path << "should not exist yet for this test." << endl;
                return false;
            }


            errno = 0;
            int fd = open(file_path, O_RDWR | O_CREAT);
            if (fd < 0 || errno != 0) {
                cerr << "Error " << strerror(errno) << " trying to creat file " << file_path << " for read|write." << endl;
                return false;
            }

            char write_buf[1024];
            int write_count =	-1;

            unsigned int num_writes = randint(10, 100);

            for (int write_serial = 0; write_serial < num_writes; write_serial++ )
            {
                unsigned int rand_count = randint(1, sizeof(write_buf)-1);

                unsigned int min_val = 70;
                unsigned int max_val = 124;
                char * write_buf_ptr = (char *)rand_bytes(rand_count, min_val, max_val, write_buf);

                if (write_buf_ptr == (char *)NULL)
                {
                    cerr << "Error occured in rand_bytes() utility function." << endl;
                    (void)close(fd);
                    return false;
                }
                else
                {
                    *( write_buf_ptr + rand_count ) = '\n'; // Line break for easier check
                    *( write_buf_ptr + rand_count +1 ) = '\0'; // Line break for easier check
                }

                errno = 0;
                int bytes_written = write(fd, write_buf_ptr, rand_count+1);

                if (bytes_written < (rand_count + 1))
                {
                    cerr << "Error " << strerror(errno) << " or fewer bytes " << bytes_written << " were written than requested " << (rand_count+1) << endl;
                    (void)close(fd);
                    return false;
                }

            }

            errno =0;
            rval = close(fd);

            return true;
        }
};

int main()
{
    WRITE_PLUS_A wpa = WRITE_PLUS_A();

    char test_file[1024];
    sprintf(test_file, "%s%d", "/tmp/write_own_new_", randint(1, 1000));

    errno = 0;
    if (wpa.write_own_new((char *)test_file))
    {
        cout << "Wrote to test file:: " << test_file << endl;
    }
    else {
        cerr << "Error writing to test file:: " << test_file << endl;
        cerr << "Possible custom errno::" << errno << endl;
    }

}

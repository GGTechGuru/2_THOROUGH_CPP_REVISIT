// #include <sys/types.h>
// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

extern int		randint(int incl_low, int incl_high);
extern void *		rand_bytes(unsigned int count, unsigned short min_val, unsigned short max_val, char * opt_buf);

class LSEEK_PLUS_A
{


    public:

        bool lseek_own_new(const char * file_path)
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

            unsigned int num_writes = randint(10, 100);

            off_t offset = (off_t)randint(10, 1000);
            sprintf(write_buf, "Will call lseek() to SEEK_SET %d, then writing a few bytes\n", (int)offset);

            cout << write_buf;

            errno = 0;
            rval = lseek(fd, offset, SEEK_SET);

            errno = 0;
            int bytes_written = write(fd, write_buf, strlen(write_buf));

            errno =0;
            rval = close(fd);

            return true;
        }
};

int main()
{
    LSEEK_PLUS_A wpa = LSEEK_PLUS_A();

    char test_file[1024];
    sprintf(test_file, "%s%d", "/tmp/lseek_own_new_", randint(1, 1000));

    errno = 0;
    if (wpa.lseek_own_new((char *)test_file))
    {
        cout << "Called lseek() & write() to test file:: " << test_file << endl;
    }
    else {
        cerr << "Error writing to test file:: " << test_file << endl;
        cerr << "Possible custom errno::" << errno << endl;
    }

}

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

class MKFIFO_A
{
   private:
       const char *	fifo_path_a;
       char	write_buf[1024];
       char	read_buf[sizeof(write_buf)];
       unsigned int	rand_count = randint(1, sizeof(write_buf)-1);

       bool fifo_fork_read()
       {
           pid_t cpid = -1;

           errno = 0;

           if ((cpid=fork()) == 0) {

               cout << "Reader child pid::" << getpid() << endl;

               errno = 0;

               int	ch_fifo_fd = open(this->fifo_path_a, O_RDONLY);

               if (errno != 0 || ch_fifo_fd < 0)
               {
                   cerr << "Error " << strerror(errno) << " from open() on (FIFO) " << this->fifo_path_a << endl;

                   exit(1);
               }

               errno = 0;

               int rval = read(ch_fifo_fd, read_buf, (this->rand_count+1));

               if (errno != 0 || rval < 1)
               {
                   cerr << "Error " << strerror(errno) << " from read() on (FIFO) fd " << ch_fifo_fd << endl;
                   exit(2);
               }

               read_buf[rval] = '\0';
               cout << "Data from read() on FIFO::\n" << read_buf << endl;

               errno = 0;
               rval = close(ch_fifo_fd);

               exit(0);

           }

           else if (cpid == -1)
           {
               cout << "Error from fork() :: " << strerror(errno) << endl;
               exit(3);
           }

           else
           {   
               cout << "Writer parent pid:: " << getpid() << endl;

               int	par_fifo_fd = open(this->fifo_path_a, O_WRONLY);

               unsigned int min_val = 70;
               unsigned int max_val = 124;
               char * write_buf_ptr = (char *)rand_bytes(rand_count, min_val, max_val, this->write_buf);

               if (write_buf_ptr == (char *)NULL)
               {
                   cerr << "Error occured in rand_bytes() utility function." << endl;
                   (void)close(par_fifo_fd);
                   return false;
               }

               else
               {
                   // *( write_buf_ptr + 1 ) = '\n'; // Line break for easier check

                   *( write_buf_ptr + rand_count ) = '\n'; // Line break for easier check
               }

               *( write_buf_ptr + rand_count + 1 ) = '\0';

               errno = 0;
               int bytes_written = write(par_fifo_fd, write_buf_ptr, rand_count+1);

               if (bytes_written < (rand_count + 1))
               {
                   cerr << "Error " << strerror(errno) << " or fewer bytes " << bytes_written << " were written than requested " << (rand_count+1) << endl;
                   (void)close(par_fifo_fd);
                   return false;
               }

               errno = 0;
               int rval = close(par_fifo_fd);

               cout << "Wrote to FIFO " << this->fifo_path_a << " bytes::\n" << write_buf_ptr << endl;

               return true;
           }
       }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    public:

        bool fifo_try_a(const char * file_path)
        {

            struct stat		sb;

            // strcpy(this.fifo_path_a, file_path);
            this->fifo_path_a = file_path;

            errno = 0;
            int rval = stat(file_path, &sb);
            
            if (rval == 0 || errno == 0)
            {
                cerr << "File " << file_path << "should not exist yet for this test." << endl;
                return false;
            }

            cout << "Will create FIFO " << this->fifo_path_a << endl;
            errno = 0;
            rval = mkfifo(file_path, S_IRWXU|S_IRWXG|S_IRWXO);
            cout << "Created FIFO " << this->fifo_path_a << endl;

            if (rval != 0 || errno != 0) {
                cerr << "Error " << strerror(errno) << " trying to call read+write mkfifo() for " << file_path << endl;
                return false;
            }


            return fifo_fork_read();
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    MKFIFO_A m_a = MKFIFO_A();

    char test_file[1024];
    sprintf(test_file, "%s%d", "/tmp/fifo_try_a_", randint(1, 1000));

    cout << "Test path " << test_file << endl;

    errno = 0;
    if (m_a.fifo_try_a((char *)test_file))
    {
        cout << "Wrote to test file:: " << test_file << endl;
    }
    else {
        cerr << "Error writing to test file:: " << test_file << endl;
        cerr << "Possible custom errno::" << errno << endl;
    }
}

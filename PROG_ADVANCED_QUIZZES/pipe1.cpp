#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// #include <sys/types.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/stat.h>

#include "util_lib_1.h"

using namespace std;

class PIPE_A
{
   private:
       int 	pipe_read_fd;
       int 	pipe_write_fd;

       char	write_buf[1024];
       char	read_buf[sizeof(write_buf)];

       unsigned int	rand_count = randint(1, sizeof(write_buf)-1);

       bool pipe_fork_read()
       {
           pid_t cpid = -1;

           errno = 0;
           if ((cpid=fork()) == 0) {

               cout << "Reader child pid::" << getpid() << endl;

               errno = 0;
               int rval = read(this->pipe_read_fd, read_buf, (this->rand_count+1));
               if (errno != 0 || rval < 1)
               {
                   cerr << "Error " << strerror(errno) << " from read() on (pipe) fd " << this->pipe_read_fd << endl;
                   exit(2);
               }

               read_buf[rval] = '\0';
               cout << "Data from read() on pipe::\n" << read_buf << endl;

               errno = 0;
               rval = close(this->pipe_read_fd);
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

               unsigned int min_val = 70;
               unsigned int max_val = 124;
               char * write_buf_ptr = (char *)rand_bytes(rand_count, min_val, max_val, this->write_buf);

               if (write_buf_ptr == (char *)NULL)
               {
                   cerr << "Error occured in rand_bytes() utility function." << endl;
                   (void)close(this->pipe_write_fd);
                   return false;
               }
               else
               {
                   *( write_buf_ptr + rand_count ) = '\n'; // Line break for easier check
                   *( write_buf_ptr + rand_count + 1 ) = '\0';
               }

               errno = 0;
               int bytes_written = write(this->pipe_write_fd, write_buf_ptr, rand_count+1);

               if (bytes_written < (rand_count + 1))
               {
                   cerr << "Error " << strerror(errno) << " or fewer bytes " << bytes_written << " were written than requested " << (rand_count+1) << endl;
                   (void)close(this->pipe_write_fd);
                   return false;
               }

               errno = 0;
               int rval = close(this->pipe_write_fd);
               cout << "Wrote to pipe bytes::\n" << write_buf_ptr << endl;

               return true;
           }
       }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    public:

        bool pipe_try_a()
        {
            int	pipe_ary[2];
            errno = 0;
            int rval = pipe(pipe_ary);
            if (rval != 0 || errno != 0) {
                cerr << "Error " << strerror(errno) << " trying to call pipe()" << endl;
                return false;
            }

            this->pipe_read_fd = pipe_ary[0];
            this->pipe_write_fd = pipe_ary[1];
            return pipe_fork_read();
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    PIPE_A p_a = PIPE_A();

    p_a.pipe_try_a();
}

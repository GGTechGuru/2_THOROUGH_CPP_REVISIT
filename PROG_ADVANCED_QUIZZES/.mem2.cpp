#include <iostream>

#include <fcntl.h>

#include <math.h>

#include <string.h>

#include <sys/shm.h>

#include <sys/syscall.h>

#include <sys/wait.h>

#include <unistd.h>


#include "util_lib_1.h"

using namespace std;

class MEMFD_SECRET_1
{
    private:
        char		write_buf[1024];
        char		read_buf[sizeof(write_buf)];
        unsigned int	rand_count = randint(1, sizeof(write_buf)-1);

        int		pipe_read_fd;
        int		pipe_write_fd;

        int		mem_fd = -1;

    public:
        bool memfd_secret_try_a(const unsigned int limit_secs)
        {
            pid_t	cpid;
            int		pipe_ary[2];

            errno =0;
            // this->mem_fd = syscall(SYS_memfd_secret, 0);
            this->mem_fd = syscall(SYS_memfd_secret, 0);
            if (errno != 0 || this->mem_fd < 0)
            {
                cerr << "Error from memfd_secret():: " << strerror(errno) << endl;
                return false;
            }

            errno =0;
            int rval = ftruncate(this->mem_fd, 0);
            if (errno != 0 || rval != 0)
            {
                cerr << "Error from ftruncate():: " << strerror(errno) << endl;
                return false;
            }

            errno = 0;
            rval = write(this->mem_fd, (const char *)"Test", 4);
            if (errno != 0 || rval != 4)
            {
                cerr << "Error from small write in parent:: " << strerror(errno) << " returned " << rval << endl;
                return false;
            }

            errno = 0;
            rval = pipe2(pipe_ary, O_NONBLOCK);
            if (errno != 0 || rval != 0)
            {
                cerr << "Error " << strerror(errno) << " from pipe()." << endl;
                return false;
            }

            this->pipe_read_fd = pipe_ary[0];
            this->pipe_write_fd = pipe_ary[1];

            errno = 0;
            if ((cpid=fork()) == 0)
            {
                unsigned int	errs = 0;

                cout << "INFO: Child proc id " << getpid() << endl;

                useconds_t	limit_usecs = (useconds_t)(limit_secs * pow(10, 6) - 1);
                errno = 0;
                int rval = ualarm(limit_usecs, 0);
                if (errno != 0 || rval != 0)
                {
                    cerr << "Error " << strerror(errno) << " from call to ualarm() with usecs " << limit_usecs << endl;
                    exit(3);
                }

                errno = 0;
                int child_fd = dup(this->mem_fd);
                if (errno != 0 || child_fd < 0)
                {
                    cerr << "Child process call to dup(): error:: " << strerror(errno) << endl;
                    exit(5);
                }

                unsigned int min_val = 70;
                unsigned int max_val = 124;
                char * write_buf_ptr = (char *)rand_bytes(rand_count, min_val, max_val, this->write_buf);

                if (write_buf_ptr == (char *)NULL)
                {
                    cerr << "Error occured in rand_bytes() utility function." << endl;
                    return false;
                }
                else
                {
                   *( write_buf_ptr + rand_count ) = '\0';
                }

                errno = 0;
                // rval = write(child_fd, write_buf_ptr, rand_count);
                rval = write(this->mem_fd, write_buf_ptr, rand_count);
                if (errno != 0 || rval != rand_count)
                {
                    cerr << "Child process call to write() " << strerror(errno) << " or bytes written " << rval << " != requested number of " << rand_count << endl;
                    exit(7);
                }

                cout << "Child process wrote to (dup of) mem secret fd::\n" << write_buf_ptr << endl;

                errno = 0;
                rval = write(this->pipe_write_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from write(pipe_fd) " << strerror(errno) << " or bytes read " << rval << " != requested number of " << sizeof(errs) << endl;
                    exit(6);
                }

                exit(0);
            }
            else if (cpid < 0) {
                cerr << "Error " << strerror(errno) << " from fork() call." << endl;
                return false;
            }
            else {

                cout << "INFO: Parent proc id " << getpid() << endl;

                int wstatus = 0;
                errno = 0;
                pid_t pid = waitpid(cpid, &wstatus, 0);
                if (errno != 0 || pid != cpid || wstatus != 0)
                {
                    cerr << "Problem from waitpid() on child proc pid " << cpid << endl;
                    if (errno != 0) { cerr << "Error " << strerror(errno) << endl; }
                    if (wstatus != 0) { cerr << "Wait status " << wstatus << endl; }
                    return false;
                }
              
                char dest_buf[1024];

                errno = 0;
                int rval = lseek(mem_fd, 0, SEEK_SET);
                if (errno != 0 || rval != 0)
                {
                    cerr << "Child process: lseek() to 0 error status :: " << strerror(errno) << endl;
                    return false;
                }

                errno = 0;
                rval = read(mem_fd, dest_buf, rand_count);
                if (errno != 0 || rval != 0)
                {
                    cerr << "Child process: read() error status :: " << strerror(errno) << endl;
                    return false;
                }

                cout << "Parent got data from read on memsecret_fd::\n" << dest_buf << endl;

                unsigned int errs = 0;
                errno =0;
                rval = read(this->pipe_read_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from read(pipe_fd) " << strerror(errno) << " or bytes read " << rval << " != requested number of " << sizeof(errs) << endl;
                    return false;
                }

                errno = 0;
                (void)close(this->mem_fd);

                return errs == 0 ? true : false;
            }
        }

        ~MEMFD_SECRET_1()
        {
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    unsigned int	test_limit_secs = 1;

    MEMFD_SECRET_1	ms1 = MEMFD_SECRET_1();

    if (ms1.memfd_secret_try_a(test_limit_secs) == false)
        cerr << "One/more errors from memfd_secret_try_a()" << endl;

}

#include <iostream>

#include <fcntl.h>

#include <math.h>

#include <string.h>

#include <unistd.h>

#include <sys/shm.h>

#include <sys/wait.h>

#include "util_lib_1.h"

using namespace std;

class SHM_OPS_1
{
    private:
        char	write_buf[1024];
        char	read_buf[sizeof(write_buf)];
        unsigned int	rand_count = randint(1, sizeof(write_buf)-1);

        int	pipe_read_fd;
        int	pipe_write_fd;

        int	shm_id = 1;

    public:
        bool shm_try_a(const unsigned int limit_secs)
        {
            pid_t	cpid;
            int		pipe_ary[2];

            errno = 0;
            int rval = pipe2(pipe_ary, O_NONBLOCK);
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

                errno =0;
                void * shm_addr = shmat(this->shm_id, (void *)NULL, 0);
                if (errno != 0 || shm_addr == (void *)NULL)
                {
                    cerr << "Child process: got error " << strerror(errno) << " from shmat().";
                    exit(2);
                }

                cout << "Child process return (address) from shmat() " << (long)shm_addr << endl;

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

                strncpy((char *)shm_addr, write_buf_ptr, strlen(write_buf_ptr));
                cout << "Child wrote to shmat() address::\n" << write_buf_ptr << endl;

                errno = 0;
                rval = write(this->pipe_write_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from write(pipe_fd) " << strerror(errno) << " or bytes read " << rval << " != requested number of " << sizeof(errs) << endl;
                    return false;
                }

                exit(0);
            }
            else if (cpid < 0) {
                cerr << "Error " << strerror(errno) << " from fork() call." << endl;
                return false;
            }
            else {

                cout << "INFO: Parent proc id " << getpid() << endl;

                errno =0;
                void * shm_addr = shmat(this->shm_id, (void *)NULL, 0);
                if (errno != 0 || shm_addr == (void *)NULL)
                {
                    cerr << "Error " << strerror(errno) << " from shmat().";
                    return false;
                }
                cout << "Parent process return (address) from shmat() " << (long)shm_addr << endl;

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
                strncpy(dest_buf, (char *)shm_addr, rand_count);
                dest_buf[rand_count] = '\0';

                cout << "Parent got data from shmat address::\n" << dest_buf << endl;

                unsigned int errs = 0;
                errno =0;
                rval = read(this->pipe_read_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from read(pipe_fd) " << strerror(errno) << " or bytes read " << rval << " != requested number of " << sizeof(errs) << endl;
                    return false;
                }

                errno = 0;
                rval = shmdt(shm_addr);

                if (errno != 0 || rval != 0)
                {
                    cerr << "Parent process: error from shmdt() " << strerror(errno) << endl;
                    return false;
                }

                return errs == 0 ? true : false;
            }
        }

        ~SHM_OPS_1()
        {
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    unsigned int	test_limit_secs = 1;

    SHM_OPS_1	so1 = SHM_OPS_1();

    if (so1.shm_try_a(test_limit_secs) == false)
        cerr << "One/more errors from shm_try_a()" << endl;

}

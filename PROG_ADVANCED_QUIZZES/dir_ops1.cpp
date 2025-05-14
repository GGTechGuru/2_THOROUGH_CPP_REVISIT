#include <iostream>

#include <fcntl.h>

#include <math.h>

#include <string.h>

#include <unistd.h>

#include <sys/wait.h>

using namespace std;

class DIR_OPS_1
{
    private:
        int    pipe_read_fd;
        int    pipe_write_fd;

    public:
        bool chroot_try_a(const unsigned int limit_secs)
        {
            pid_t	cpid;
            int		pipe_ary[2];

            errno = 0;
            int rval = pipe2(pipe_ary, O_NONBLOCK);
            // int rval = pipe2(pipe_ary, 0);
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
                char *	cwd = (char *)NULL;

                char *	new_root_1 = (char *)"..";

                cout << "INFO: Child proc id " << getpid() << endl;

                useconds_t	limit_usecs = (useconds_t)(limit_secs * pow(10, 6) - 1);
                errno = 0;
                int rval = ualarm(limit_usecs, 0);
                if (errno != 0 || rval != 0)
                {
                    cerr << "Error " << strerror(errno) << " from call to ualarm() with usecs " << limit_usecs << endl;
                    exit(3);
                }

                cwd = getcwd(NULL, 0);
                cout << "Initial getcwd() " << cwd << endl;

                errno = 0;
                rval = chdir(new_root_1);
                if (errno != 0 || rval != 0)
                {
                    cerr << "Error " << strerror(errno) << " from chdir() to " << new_root_1 << endl;
                    exit(4);
                }

                cwd = getcwd(NULL, 0);
                cout << "Return from getcwd() after chdir() to " << new_root_1 << " is " << cwd << endl;

                errno =0;
                rval = chroot(new_root_1);
                if (errno != 0 || rval != 0)
                {
                    cerr << "Error " << strerror(errno) << " from chroot() to " << new_root_1 << endl;
                    exit(2);
                }

                cwd = getcwd(NULL, 0);
                cout << "Return from getcwd() after chroot() to " << new_root_1 << " is " << cwd << endl;

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
              

                unsigned int errs = 0;
                errno =0;
                rval = read(this->pipe_read_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from read(pipe_fd) " << strerror(errno) << " or bytes read " << rval << " != requested number of " << sizeof(errs) << endl;
                    return false;
                }

                return errs == 0 ? true : false;
            }
        }

        ~DIR_OPS_1()
        {
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    unsigned int	test_limit_secs = 1;

    DIR_OPS_1	do1 = DIR_OPS_1();

    if (do1.chroot_try_a(test_limit_secs) == false)
        cerr << "One/more errors from chroot_try_a()" << endl;

}

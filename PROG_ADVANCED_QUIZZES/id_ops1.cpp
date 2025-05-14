#include <iostream>

#include <fcntl.h>

#include <map>

#include <math.h>

#include <string.h>

#include <unistd.h>

#include <sys/wait.h>

using namespace std;

// #include <fstream>

class UID_OPS_1
{
    private:
        int    pipe_read_fd;
        int    pipe_write_fd;

    public:
        bool seteuid_try_a(const map<string, uid_t> users_ids, const unsigned int limit_secs)
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
                string	user_name;
                uid_t	user_id;
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

                // for user_name, user_id in users_ids
                // map<string, uid_t>:: iterator iter = user_ids::iterator();
                // for (map<string, uid_t>::iterator iter = users_ids.begin(); iter != users_ids.end(); iter++)

                for (const auto& pair: users_ids)
                {
                    // user_name = iter->first;
                    user_name = pair.first;

                    // user_id = iter->second;
                    user_id = pair.second;

                    cout << "Starting with euid " << geteuid() << endl;
                    errno = 0;
                    rval = seteuid(user_id);
                    if (errno != 0 || rval != 0)
                    {
                        cerr << "Error " << strerror(errno) << " from seteuid() to user " << user_name << " with uid " << user_id << endl;
                        errs++;
                    }
                    else
                    {
                        cout << "Succeeded seteuid() to user " << user_name << " with uid " << user_id << endl;
                    }
                }

                errno =0;
                rval = write(this->pipe_write_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from write(pipe_fd) " << strerror(errno) << " or bytes written != requested number of " << sizeof(errs) << endl;
                    exit(2);
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
                // int rval = waitpid(cpid, &wstatus, WNOHANG);
                pid_t pid = waitpid(cpid, &wstatus, 0);
                if (errno != 0 || pid != cpid || wstatus != 0)
                {
                    cerr << "Problem from waitpid() on child proc pid " << cpid << endl;
                    cerr << "Error " << strerror(errno) << endl;
                    cerr << "Wait status " << wstatus << endl;
                    return false;
                }

                unsigned int errs = 0;
                errno =0;
                rval = read(this->pipe_read_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from read(pipe_fd) " << strerror(errno) << " or bytes read != requested number of " << sizeof(errs) << endl;
                    return false;
                }

                return errs == 0 ? true : false;
            }
        }

        ~UID_OPS_1()
        {
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GID_OPS_1
{
    private:
        int    pipe_read_fd;
        int    pipe_write_fd;

    public:
        bool setegid_try_b(const map<string, gid_t> groups_gids, const unsigned int limit_secs)
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
                string	group_name;
                gid_t	group_id;
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

                for (const auto& pair: groups_gids)
                {
                    group_name = pair.first;
                    group_id = pair.second;

                    cout << "Starting with egid " << getegid() << endl;
                    errno = 0;
                    rval = setegid(group_id);
                    if (errno != 0 || rval != 0)
                    {
                        cerr << "Error " << strerror(errno) << " from setegid() to group " << group_name << " with gid " << group_id << endl;
                        errs++;
                    }
                    else
                    {
                        cout << "Succeeded setegid() to group " << group_name << " with gid " << group_id << endl;
                    }
                }

                errno =0;
                rval = write(this->pipe_write_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from write(pipe_fd) " << strerror(errno) << " or bytes written != requested number of " << sizeof(errs) << endl;
                    exit(2);
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
                    cerr << "Error " << strerror(errno) << endl;
                    cerr << "Wait status " << wstatus << endl;
                    return false;
                }

                unsigned int errs = 0;
                errno =0;
                rval = read(this->pipe_read_fd, (char *)(&errs), sizeof(errs));
                if (errno != 0 || rval != sizeof(errs))
                {
                    cerr << "Error from read(pipe_fd) " << strerror(errno) << " or bytes read != requested number of " << sizeof(errs) << endl;
                    return false;
                }

                return errs == 0 ? true : false;
            }
        }

        ~GID_OPS_1()
        {
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    unsigned int	test_limit_secs = 1;

    UID_OPS_1	uid1 = UID_OPS_1();

    // map<string, uid_t> users_ids = map<string, uid_t>();
    map<string, uid_t> users_ids = 
    {
        {"root", (uid_t)0},
        {"learnitall", (uid_t)1001}
    };
    if (uid1.seteuid_try_a(users_ids, test_limit_secs) == false)
        cerr << "One/more errors from seteuid_try_a()" << endl;

    GID_OPS_1	gid1 = GID_OPS_1();

    map<string, gid_t> groups_ids = 
    {
        {"root", (gid_t)0},
        {"users", (gid_t)100}
    };
    if (gid1.setegid_try_b(groups_ids, test_limit_secs) == false)
        cerr << "One/more errors from setegid_try_b()" << endl;

}

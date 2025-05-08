#include <iostream>
#include <unistd.h>

using namespace std;

class FORK_1
{
    private:

    public:

       void fork_try_a()
       {
           pid_t cpid = -1;

           errno = 0;
           if ((cpid=fork()) == 0) {
               cout << "From child process, pid == " << getpid() << endl;
               exit(0);
           }
           else if (cpid == -1)
           {
               cout << "Error from fork(). errno == " << errno << endl;
               exit(1);
           }
           else
           {
               cout << "From parent process, child pid from fork() was == " << cpid << endl;
           }
       }
};

int main()
{
    FORK_1	f_1 = FORK_1();
    f_1.fork_try_a();

}

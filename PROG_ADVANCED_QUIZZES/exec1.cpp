#include <iostream>

#include <errno.h>
#include <string.h>
#include <unistd.h>

using namespace std;

class EXEC_1
{
    private:

    public:

       void exec_try_a( char* dir_path )
       {
           pid_t cpid = -1;

           errno = 0;
           if ((cpid=fork()) == 0) {

               char* exec_path = (char *)"/usr/bin/ls";
               // char *argslist[] = { (char *)exec_path, (char *)"-a", (char *)"-l", dir_path, (char *)NULL };

               int index = 0;
               char* argslist[20];
               argslist[index++] = exec_path;
               argslist[index++] = (char *)"-l";
               argslist[index++] = dir_path;
               argslist[index++] = (char *)NULL;

               errno = 0;
               int rval = execv(argslist[0], argslist);

               // int rval = execl("/usr/bin/ls", (char *)"-a", (char *)"-l", (char *)"/etc", (char *)NULL);

               cout << "errno from exec:: " << strerror(errno) << endl;
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

int main(int arg_count, char** args)
{
    int index = 0;
    char** args_copy = args;
    cout << "List of args to this prog:" << endl;
    for (index=0; index<arg_count; index++)
        cout << *args_copy << endl;
        args_copy++;

    EXEC_1	e_1 = EXEC_1();
    e_1.exec_try_a( *(args+1) );

}

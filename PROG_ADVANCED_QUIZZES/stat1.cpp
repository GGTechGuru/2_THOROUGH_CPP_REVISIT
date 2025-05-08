#include <iostream>

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

using namespace std;

class STAT_1
{
    private:

    public:

       void stat_try_a( char* targ_path )
       {
           errno = 0;

           struct stat	statinfo;

           int rval = stat(targ_path, &statinfo);

           if (rval != 0 || errno != 0)
           {
               cout << "errno from stat:: " << strerror(errno) << endl;
               return;
           }

           cout << "Value of st_dev == " << statinfo.st_dev << endl;
           cout << "Value of st_ino == " << statinfo.st_ino << endl;
           cout << "Value of st_mode == " << statinfo.st_mode << endl;
           cout << "Value of st_nlink == " << statinfo.st_nlink << endl;
           cout << "Value of st_uid == " << statinfo.st_uid << endl;
           cout << "Value of st_gid == " << statinfo.st_gid << endl;
           cout << "Value of st_rdev == " << statinfo.st_rdev << endl;
           cout << "Value of st_size == " << statinfo.st_size << endl;
           cout << "Value of st_blksize == " << statinfo.st_blksize << endl;
           cout << "Value of st_blocks == " << statinfo.st_blocks << endl;
           cout << "Value of st_atime == " << statinfo.st_atime << endl;
           cout << "Value of st_mtime == " << statinfo.st_mtime << endl;
           cout << "Value of st_ctime == " << statinfo.st_ctime << endl;
       }
};

int main(int arg_count, char** args)
{
    int index = 0;
    char** args_copy = args;
    cout << "List of args to this prog:" << endl;
    for (index=0; index<arg_count; index++)
        cout << *(args_copy + index) << endl;

    STAT_1	s_1 = STAT_1();
    s_1.stat_try_a( *(args+1) );

}

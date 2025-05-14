#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

class DIR_OPS_2
{
    public:
        bool mkdir_try_a()
        {
            char *	tmp_dir = (char *)"/tmp/.some_dir";

            errno = 0;
            int rval = mkdir(tmp_dir, S_IRWXU|S_IRWXG);
            if (errno != 0 || rval != 0) {
                cerr << "Error from mkdir(): " << strerror(errno) << endl;
                return false;
            }

            errno = 0;
            rval = rmdir(tmp_dir);
            if (errno != 0 || rval != 0) {
                cerr << "Error from rmdir(): " << strerror(errno) << endl;
                return false;
            }

            return true;
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    DIR_OPS_2	do2 = DIR_OPS_2();

    if (do2.mkdir_try_a() == false)
        cerr << "One/more errors from mkdir_try_a()" << endl;

}

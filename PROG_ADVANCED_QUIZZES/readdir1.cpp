#include <iostream>

#include <sys/types.h>

#include <dirent.h>

#include <stdlib.h>

#include <unistd.h>

class LD
{
    public:

        void rd()
        {

            // std::string dir_path = "/tmp";
            char * dir_path = (char *)"/tmp";

            errno = 0;
            DIR* dirp = opendir((const char *)dir_path);

            std::cout << "Errno from opendir() == " << errno << "\n";

            struct dirent *de;

            while ((de = readdir(dirp)) != (struct dirent *)NULL)
            {
                std::cout << de->d_name << "\n";
            }

            (void)closedir(dirp);

        }
};

int main()
{
    LD ld = LD();

    ld.rd();

}

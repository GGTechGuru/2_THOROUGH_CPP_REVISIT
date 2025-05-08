#include <iostream>

#include <sys/types.h>

#include <fcntl.h>

#include <stdlib.h>

#include <unistd.h>

class IO_STUFF
{
    public:

        void read_file(const char * file_path)
        {
            char read_buf[1024];
            int fd =	-1;
            int rd_count =	-1;

            errno = 0;
            fd = open(file_path, O_RDONLY);
            std::cerr << "Errno from open() == " << errno << "\n";

            errno = 0;
            while ((rd_count = read(fd, read_buf, 1023)) > 0)
            {
                read_buf[rd_count] = '\0';
                std::cout << read_buf;
            }

            std::cout << "\n";

            (void)close(fd);

        }
};

int main()
{
    IO_STUFF io_s = IO_STUFF();

    io_s.read_file("/etc/hosts");

}

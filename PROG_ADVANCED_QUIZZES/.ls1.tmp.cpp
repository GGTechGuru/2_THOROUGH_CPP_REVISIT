#include <iostream>

#ifdef __WIN32

    #include <windows.h>

#else

    #include <sys/types.h>

    #include <dirent.h>

    #include <stdlib.h>

    #include <unistd.h>

#endif

struct DirentNode {
    char * ent_name;
    struct DirentNode * next;
}

class ListDir
{
    public:

        char ** dir_entries_list(char dir_path)
        {

            #ifdef __WIN32

                stdout::cerr << "Code not yet implemented for Windows subsystem\n";

                return -1;

            #endif

            errno = 0;

            DIR* dirp = opendir(dir_path);

            if (errno != 0 || dirp == (DIR *)NULL)
            {
                std::cerr << "Got null or errno ==" << errno << " from opendir() on path " << dir_path << "\n";

                return -1;
            }
            else
            {
                struct DirentNode * ent_ll_head = (struct DirentNode *)NULL;
                struct DirentNode * ent_ll_tail = (struct DirentNode *)NULL;

                (struct dirent *) de = (struct dirent *)NULL;

                while ( (de = readdir(dirp)) != (struct dirent *)NULL)
                {
                    char* ent_name = de->d_name;

                    if (ent_ll_head == (struct DirentNode *)NULL)
                    {

                        ent_ll_head = (struct DirentNode *)malloc(sizeof(struct DirentNode));

                        (char *) alloc_name = (char *)malloc((size_t)(strlen(ent_name)+1));

                        strcpy(alloc_name, ent_name);

                        ent_ll_head->ent_name = alloc_name;

                        ent_ll_head->next = (struct DirentNode *)NULL;

                        ent_ll_tail = ent_ll_head;
                    }
                    else
                    {
                        
                    }
                }
            }

        }
};

int main( char* argv[] )
{
    dp_count = sizeof(argv);

    if (dp_count < 1)
    {

        std::cerr << "Need at least 1 directory path.\n";

        exit(2);
    }
    else
    {
        ListDir ld = ListDir();

        int index = -1;

        char **entries = (char *)NULL;

        for (index=1; index < dp_count; index++)
        {

            dir_path = argv[index];

            rval = ld.list_dir(dir_path, entries);

            if (rval == 0)
            {
                next_entry = entries;

                while (next_entry != (char *)NULL)
                {
                    std::cout << *next_entry << "\n";

                    next_entry++;
                }
            }
        }
    }
}

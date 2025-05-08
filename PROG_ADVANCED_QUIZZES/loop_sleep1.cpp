#include <iostream>

#ifdef __WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

class LoopSleep
{
    private:
        void loc_sleep(int duration_sec)
        {
            #ifdef __WIN32
                Sleep(duration_sec);
            #else
                sleep(duration_sec);
            #endif
        }

    public:

        void loop_sleep(int duration, int loop_count)
        {
            std::cout << "Will sleep for " << duration << " seconds, for " << loop_count << " times.\n";

            int index;
            for (index=0; index<loop_count; index++) {

                loc_sleep(duration);

                std::cout << "Loop index::" << index << "\n";
            }
        }
};

int main()
{
    LoopSleep ls;

    long pid;

    pid = getpid();

    std::cout << "This process PID is:: " << pid << "\n";

    ls.loop_sleep(10, 20);
}

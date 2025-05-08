#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

void * rand_bytes(unsigned int count, unsigned short min_val, unsigned short max_val, char * opt_buf)
{

    // if (min_val >= max_val || min_val < 0)
    if (min_val >= max_val)
    {
        cerr << "Minimum byte value " << min_val << " should be non-zero and less than max_val " << max_val << endl;
        return (void *)NULL;
    }

    char * byte_buf;

    if (opt_buf != (char *)NULL)
    {
        byte_buf = opt_buf;
    }
    else byte_buf = (char *)malloc(count+1);

    unsigned short val_range = max_val - min_val + 1;
    unsigned short modulo = -1;

    struct timeval st;

    errno = 0;
    int rval = gettimeofday(&st, (struct timezone *)NULL);
    unsigned int microseconds = (unsigned int)(st.tv_usec);
    srandom(microseconds);

    char byte_val;
    for (int index=0; index<count; index++)
    {

        modulo = random() % val_range;

        byte_val = min_val + modulo;

        byte_buf[index] = byte_val;
    }

    return (void *) byte_buf;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

int randint(int incl_low, int incl_high)
{

    struct timeval st;

    if (incl_low >= incl_high)
    {
        cerr << "Range low " << incl_low << " should be less than range high " << incl_high << endl;
        errno = -1;
        return -1;
    }

    int incl_range = incl_high - incl_low + 1;

    errno = 0;
    int rval = gettimeofday(&st, (struct timezone *)NULL);
    unsigned int microseconds = (unsigned int)(st.tv_usec);
    srandom(microseconds);

    unsigned int modulo = random() % incl_range;

    int rand_val = incl_low + modulo;

    return rand_val;
}

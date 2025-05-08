
#include <iostream>

using namespace std;

extern void * rand_bytes(unsigned int count, unsigned short min_val, unsigned short max_val);

int main()
{
    unsigned int count = 100;
    char * rb = (char *)rand_bytes(count, 48, 122);

    // *(rb + count) = '\n';
    *(rb + count) = '\0';

    cout << "Return from rand_bytes()::\n" << rb << endl;
}

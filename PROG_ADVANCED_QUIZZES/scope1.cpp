#include <iostream>

#include <sys/types.h>

using namespace std;

class SCOPE_A
{
    private:
        bool bool_var_1 = false;

    protected:
        volatile int vol_var = -1;

        void vol_check() {
            for (; vol_var<11; vol_var++) {
                cout << "Value of vol_var is::" << vol_var << "\n";
            }
        }

    public:
        string pub_var = "Test of public variable";

        SCOPE_A()
        {
            static int	stat_var = 0;
            stat_var++;
            cout << "stat_var==" << stat_var << endl;
        }
};

/********************************************************************************/

class SCOPE_B: public SCOPE_A
{
    public:
        void vol_incr()
        {
            vol_var++;
        }
};

/********************************************************************************/

int main()
{
    SCOPE_A sa1 = SCOPE_A();
    SCOPE_B sb1 = SCOPE_B();
    SCOPE_A sa2 = SCOPE_A();


}

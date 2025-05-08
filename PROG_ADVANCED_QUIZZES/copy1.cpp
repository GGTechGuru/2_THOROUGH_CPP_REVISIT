#include <iostream>
using namespace std;

class SHALLOW_MOD_A
{
    public:
        int * val_to_copy = new int(25);

        SHALLOW_MOD_A()
        {
        }

        SHALLOW_MOD_A(const SHALLOW_MOD_A& trgt)
        {
            this->val_to_copy = trgt.val_to_copy;
        }
};


/********************************************************************************/

class DEEP_MOD_A
{
    public:
        int * val_to_copy = new int(25);

        DEEP_MOD_A()
        {
        }

        DEEP_MOD_A(const DEEP_MOD_A& trgt)
        {
            this->val_to_copy = new int(*trgt.val_to_copy);
        }
};

/********************************************************************************/

int main()
{

    SHALLOW_MOD_A	sma1 = SHALLOW_MOD_A();
    SHALLOW_MOD_A	sma2 = sma1;

    *(sma2.val_to_copy) = 18;

    cout << "sma1.val_to_copy " << *(sma1.val_to_copy) << endl;
    cout << "sma2.val_to_copy " << *(sma2.val_to_copy) << endl;



    DEEP_MOD_A	dma1 = DEEP_MOD_A();
    DEEP_MOD_A	dma2 = dma1;

    *(dma2.val_to_copy) = 39;

    cout << "dma1.val_to_copy " << *(dma1.val_to_copy) << endl;
    cout << "dma2.val_to_copy " << *(dma2.val_to_copy) << endl;

}

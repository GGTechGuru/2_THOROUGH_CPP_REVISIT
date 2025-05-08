#include <iostream>
using namespace std;

class ABSTRACT_A
{
    public:
        virtual void	identity() = 0;
};


/********************************************************************************/

class INSTANTIABLE_B: public ABSTRACT_A
{
    public:
        void identity() override
        {
            cout << "Class INSTANTIABLE_B" << endl;
        }
};

/********************************************************************************/

int main()
{

    INSTANTIABLE_B ib = INSTANTIABLE_B();
    ib.identity();

}

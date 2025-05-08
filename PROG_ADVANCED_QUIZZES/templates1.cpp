#include <iostream>
using namespace std;

class TEMPLATES_A
{
    public:
        template	<typename T>
        T add(T v1, T v2)
        {
            // cout << "Template add(): Type of v1 is ==" << typeof v1 << endl;
            return v1 + v2;
        }
};

template <typename T1, typename T2>
class TEMPLATED_CLASS_B
{
    public:
        T1	var_T1;
        T2	var_T2;

        void disp(T1 t1v1, T2 t2v1, int times)
        {
            for (int index=0; index<times; index++)
                cout << t1v1;
            cout << endl;

            for (int index=0; index<times; index++)
                cout << t2v1;
            cout << endl;
        }
};

/********************************************************************************/

int main()
{
    TEMPLATES_A	ta = TEMPLATES_A();

    cout << "template int add of 18 & 81 ==" << ta.add(18, 81) << endl;
    cout << "template float add of 19.6 & 0.9 ==" << ta.add(19.6, 0.9) << endl;

    TEMPLATED_CLASS_B<string, int>	tcb1 = TEMPLATED_CLASS_B<string, int>();

    tcb1.disp("Hello world! ", 2093, 5);
}

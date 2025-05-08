#include <iostream>

class C1
{
    public:

        void ack()
        {
            std::cout << "Basic class\n";
        }
};

int main()
{
    C1 c1;

    c1.ack();
}

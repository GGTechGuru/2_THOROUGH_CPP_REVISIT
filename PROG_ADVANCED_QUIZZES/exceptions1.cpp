#include <iostream>
#include <stdexcept>
#include <climits>

using namespace std;

class EXCEPTIONS_1
{
    private:

    public:
        void divide(int v1, int v2)
        {
            try
            {
                if (v2 == 0)
                    // throw new runtime_error("Cannot divide by zero.");
                    throw runtime_error("Cannot divide by zero.");
                else
                    cout << "Sum is " << v1 + v2 << endl;
            }
            catch (const runtime_error& e)
            {
                cerr << "Got an exception trying to divide " << v1 << " by " << v2 << endl;
                cerr << "Exception message was: " << e.what() << endl;
            }
            catch (const exception& e)
            {
                cerr << "Got an exception trying to add " << v1 << " and " << v2 << endl;
                cerr << "Exception message was: " << e.what() << endl;
            }
            catch (...)
            {
                cerr << "Other exception occurred." << endl;
            }
        }
};

int main()
{
    EXCEPTIONS_1	e1 = EXCEPTIONS_1();
    e1.divide(99, 0);

}

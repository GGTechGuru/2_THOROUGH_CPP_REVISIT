#include <iostream>
using namespace std;

class TYPES_A
{
    public:
        mutable int	a = 1;
        long		b = 2l;
        bool		c = true;
};

struct TNode1 {
    int 	val;
    struct TNode1	* l_node;
    struct TNode1	* r_node;
};

/********************************************************************************/

int main()
{
    TYPES_A ta1 = TYPES_A();
    TYPES_A * ta_ref;
    ta_ref = new TYPES_A();
    TYPES_A ta2 = TYPES_A();

    int		i_var = 25;
    int&	i_ref = i_var;

    cout << "i_var->" << i_var << ". i_ref->" << i_ref << endl;

    cout << "unsigned promotion 36u-64->" << 36u - 64 << endl;

    cout << "Size of class TYPES_A->" << sizeof(TYPES_A) << endl;
    cout << "Size of class TYPES_A variable->" << sizeof ta1 << endl;
    cout << "Size of class TYPES_A ref->" << sizeof ta_ref << endl;
    cout << "Size of value of TYPES_A ref->" << sizeof(*ta_ref) << endl;

    const TYPES_A cta3 = TYPES_A();

    cout << "Pre-update of const TYPE_A::a ==" << cta3.a << endl;
    cta3.a++;
    cout << "Post-update of const TYPE_A::a ==" << cta3.a << endl;

    struct TNode1 * tn1 = new struct TNode1;
    tn1->val = 81;
    tn1->l_node = (struct TNode1 *)NULL;
    tn1->r_node = (struct TNode1 *)NULL;

    cout << "Attempt struct field access before deletion->" << tn1->val << endl;
    delete tn1;
    cout << "Attempt struct field access after deletion->" << tn1->val << endl;
}

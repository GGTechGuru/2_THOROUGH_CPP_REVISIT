#include <iostream>
#include <map>
#include <vector>

using namespace std;

class HOME_A
{
    private:
        map<string, string> ks = map<string, string>();
        vector<string>	home_occupants = vector<string>();
        vector<string>	subsystems = vector<string>();

        bool sanity_check(string subsystem, map<string, string> new_status)
        {
            // Valid subsystem?
            // Valid new statuses? E.g., kitchen appliances occupied by home occupants / repair persons / cleaners?
            return true; // If subsystem & new status are valid and consistent.
        }

    public:
        map<string, string> kitchen_status()
        {
            return ks;
        }

        bool update_status(string subsystem, map<string, string> new_status)
        {
            if (sanity_check(subsystem, new_status))
            {
                // Update subsystem status
                return true;
            }
            else return false;
        }
};


/********************************************************************************/

class OFFICE_A
{
    private:
        map<string, string> os = map<string, string>();
        vector<string>	office_users = vector<string>();
        vector<string>	subsystems = vector<string>();

        bool sanity_check(string subsystem, map<string, string> new_status)
        {
            // Valid subsystem?
            // Valid new statuses?
            return true; // If subsystem & new status are valid and consistent.
        }

    public:
    public:
        map<string, string> office_status()
        {
            return os;
        }

        bool update_status(string subsystem, map<string, string> new_status)
        {
            if (sanity_check(subsystem, new_status))
            {
                // Update subsystem status
                return true;
            }
            else return false;
        }
};

/********************************************************************************/

class HOME_OFFICE_A: public HOME_A, public OFFICE_A
{
    private:
        map<string, float> tut = map<string, float>();
        map<string, vector<string>> domain_subsystems = map<string, vector<string>>();

        string subsystem_belongs(string subsystem)
        {
            // Look at list for subsystem owner
            return "NONE";
        }

    public:
        map<string, float> tax_use_tracking()
        {
            return tut;
        }

        bool update_status(string subsystem, map<string, string> new_status)
        {
            string domain=subsystem_belongs(subsystem);

            if (domain == "HOME_A")
                return HOME_A::update_status(subsystem, new_status);
            else if (domain == "OFFICE_A")
                return OFFICE_A::update_status(subsystem, new_status);
            else
                return false;
        }
};
/********************************************************************************/

int main()
{

    HOME_OFFICE_A	hoa1 = HOME_OFFICE_A();

    map<string, string> os = hoa1.office_status();
    map<string, string> ks = hoa1.kitchen_status();
    map<string, float> tut = hoa1.tax_use_tracking();

}

#include <iostream>
#include <fstream>
using namespace std;

class FILE_IO_1
{
    private:
        ifstream	fp_in;
        ofstream	fp_out;

    public:
        void copy_file(const string& in_path, const string& out_path)
        {
            fp_in.open(in_path);
            fp_out.open(out_path);

            if (!fp_in.is_open())
            {
                cerr << "Could not open file path: " << in_path << endl;
            }

            if (!fp_out.is_open())
            {
                cerr << "Could not open file path: " << out_path << endl;
            }

            string	line_buf;
            while (getline(fp_in, line_buf))
            {
                fp_out << line_buf << endl;
            }
        }

        ~FILE_IO_1()
        {
            if (fp_in.is_open())
                fp_in.close();

            if (fp_out.is_open())
                fp_out.close();
        }
};

int main()
{
    FILE_IO_1	fio1 = FILE_IO_1();

    fio1.copy_file("/etc/hosts", "/tmp/hosts");

}

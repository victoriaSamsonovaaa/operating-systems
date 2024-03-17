#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    string binaryFileName = argv[1];
    string reportFileName = argv[2];
    double price = stoi(argv[3]);
    ifstream fin(binaryFileName);
    if (!fin.is_open())
    {
        std::cerr << "Binary file error." << std::endl;
        return 1;
    }
    ofstream fout(reportFileName);
    if (!fout.is_open())
    {
        std::cerr << "Report file error." << std::endl;
        return 1;
    }
    Employee employee;
    while (fin >> employee.num)
    {

        fin >> employee.name >> employee.hours;
        fout << employee.num << ' ' << employee.name << ' ' << employee.hours << ' ' << employee.hours * price << '\n';
    }
    fin.close();
    fout.close();
    cout << "Report file was created!\n";
    return 0;
}
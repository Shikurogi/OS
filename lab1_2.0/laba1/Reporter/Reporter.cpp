#include <iostream>
#include <fstream>

struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {

    if (argc > 3)
    {
        std::string binFileName = argv[1];
        std::string reportFileName = argv[2];
        double moneyPerHour = atof(argv[3]);
        double salary;

        employee tmp;
        std::ofstream out(reportFileName, std::ios_base::out);
        std::ifstream input(binFileName, std::ios::binary | std::ios::in);

        out << binFileName << std::endl;
        while (input.read((char*)&tmp, sizeof(employee)))
        {
            salary = moneyPerHour * tmp.hours;
            out << tmp.num << " " << tmp.name << " " << tmp.hours << " " << salary << std::endl;
        }

        input.close();
        out.close();
    }

    else
    {
        std::cout << "Not enough arguments!";
    }

    return(0);
}

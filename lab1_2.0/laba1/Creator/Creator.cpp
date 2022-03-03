#include <iostream>
#include <fstream>

struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {

    if (argc > 2)
    {
        std::ofstream binFile;
        std::string fileName = argv[1];
        binFile.open(fileName, std::ofstream::binary);

        int size = atoi(argv[2]);
        if (size < 0)
            return 0;

        for (int i = 0; i < size; ++i)
        {
            employee tmp;
            std::cin >> tmp.num;
            std::cin >> tmp.name;
            std::cin >> tmp.hours;
            binFile.write((char*)&tmp, sizeof(employee));
        }
        binFile.close();
    }

    else
    {
        std::cout << "Not enough arguments!";
    }

    return 0;
}

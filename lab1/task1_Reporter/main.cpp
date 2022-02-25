#include <iostream>
#include <fstream>

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main(int argc, char *argv[]) {
    char binFileName[15];
    char reportFileName[15];
    double moneyPerHour = 0;
    double salary;
    scanf("%s", binFileName);
    scanf("%s", reportFileName);

    std::cin >> moneyPerHour;
    employee tmp;
    FILE* out = fopen(reportFileName, "w");
    std::ifstream input(binFileName, std::ios::binary | std::ios::in);

    fprintf(out, "%s\n", binFileName);
    while(input.read((char*)&tmp, sizeof(employee)))
    {
        salary = moneyPerHour * tmp.hours;
        fprintf(out, "%d %s %lf %lf\n", tmp.num, tmp.name, tmp.hours, salary);
    }

    input.close();
    fclose(out);
}

#include <iostream>
#include <winbase.h>
#include <fstream>

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main(int argc, char *argv[]) {

    FILE* binFile;
    char fileName[15];
    scanf("%s", fileName);
    binFile = fopen("employee.bin", "wb");

    int size;
    std::cin >> size;
    if (size < 0)
        return 0;

    for (int i = 0; i < size; ++i)
    {
        employee tmp;
        scanf("%d%s%lf", &tmp.num, &tmp.name, &tmp.hours);
        fwrite(&tmp, sizeof(employee), 1, binFile);
    }
    fclose(binFile);
    return 0;
}

#include <ostream>

struct employee {
    int num;
    char name[10];
    double hours;
    void output(std::ostream &out){
        out << "id: " << num << " " << "name: " << name << " " << "hours: " << hours << std::endl;
    }
};

int comp(const void* p1, const void* p2){
    return ((employee*)p1)->num - ((employee*)p2)->num;
}

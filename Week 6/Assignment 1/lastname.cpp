#include <ctime>
#include "lastname.h"
#include "lastname_array.h"


std::string generate_lastname()
{
    srand((unsigned) time(nullptr));
    return lastname[(rand() % lastname.size())];
}
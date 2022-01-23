#include <ctime>
#include "firstname.h"
#include "firstname_array.h"

std::string generate_firstname()
{
    srand((unsigned) time(nullptr));
    return firstname[(rand() % firstname.size())];
}
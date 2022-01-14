#include "firstnames.h"
#include "lastnames.h"
#include <random>
#include <chrono>

#define elements(x) sizeof(x)/sizeof(x[0])


int main(){
    std::default_random_engine engine;
    engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> fndist(0,elements(firstnames)-1);
    std::uniform_int_distribution<int> lndist(0,elements(lastnames)-1);
    printf("%s %s",firstnames[fndist(engine)], lastnames[lndist(engine)]);
}
#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include "miscutil.hpp"


/**
 * Returns a pointer to the best item in the array. which one that is is determined by the function used
 * @tparam data_t the type of data of the array elements
 * @param array the array to get the elements from
 * @param len the length of the array
 * @param is_better the function to use to determine whether the next element is actually better
 * @return a pointer to the element of the array that is considered the best
 */
template <typename data_t>
inline data_t* best(data_t* array, size_t len, bool(*is_better)(data_t&current_best,data_t&contestant)){
    if (!len) return nullptr;
    data_t* best = array;
    --len;
    while(len){
        ++array;
        if (is_better(*best,*array)){
            best = array;
        }
        --len;
    }
    return best;
}
/**
 *
 * @tparam data_t the type of data to get the average of
 */
inline float average(int* array, size_t size){
    uintmax_t total(0);
    for (size_t i=0;i!=size;++i){
        total += array[i];
    }
    return total/ (float) size;
}

int numbers[] = {
    88, 36, 55,  4, 
    33, 97, 19, 30,
    21, 87, 86, 54, 
    38, 16, 91, 15,
    98, 92, 28, 17,
};

int main(){
    sys::BusyMark mark;
    printf("BOOT!\n\n");
    printf("Numbers:\n");
    for (int i : numbers){
        printf("\t%2d\n",i);
    }
    printf("max: %2d\n",*best<const int>(numbers,elements(numbers),[](const int& x,const int& c){return c > x;}));
    printf("min: %2d\n",*best<const int>(numbers,elements(numbers),[](const int& x,const int& c){return c < x;}));
    printf("avg: %.2f\n",average(numbers,elements(numbers)));
}


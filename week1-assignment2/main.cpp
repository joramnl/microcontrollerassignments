#include "mbed.h"

int numbers[] = {
 88, 36, 55, 4, 33,
 97, 19, 30, 21, 87,
 86, 54, 38, 16, 91,
 15, 98, 92, 28, 17
};

template <typename T>
T& calcMax(T *array, size_t length) {
    size_t max = 0;

    for (size_t i = 0; i < length; i++) {
        if (array[i] > array[max])
        {
            max = i;
        }
    }

    return array[max];
}

template <typename T>
T& calcMin(T *array, size_t length) {
    size_t min = 0;

    for (size_t i = 0; i < length; i++) {
        if (array[i] < array[min])
        {
            min = i;
        }
    }

    return array[min];
}

float calcAvg(int *array, size_t length) {
    float total = 0;

    for (size_t i = 0; i < length; i++) {
        total += array[i];
    }

    return total / length;
}

int main()
{

    printf("Starting\n\r");

    float min = calcMin(numbers, sizeof(numbers) / sizeof(int));
    float max = calcMax(numbers, sizeof(numbers) / sizeof(int));
    float avg = calcAvg(numbers, sizeof(numbers) / sizeof(float));

    while (true) {

        printf("Min: %.1f\n\r", min);
        printf("Max: %.1f\n\r", max);
        printf("Avg: %.2f\n\r", avg);
        printf("------------\n\r\n\r");
        
        ThisThread::sleep_for(5000ms);
    }
}

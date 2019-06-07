#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <ctime>
#include <assert.h>
#include <fstream>
#include <math.h>

#define BOOST_TEST_MODULE test_main

#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;
BOOST_AUTO_TEST_SUITE(test_suite_main)

void swap(int* array, size_t index1, size_t index2)
{
    int temp = array[index1];
    array[index1] = array[index2];
    array[index2] = temp;
}

size_t partition(int* array, size_t begin, size_t end)
{
    int pivot = array[end - 1];
    size_t i = begin - 1;

    for(size_t j = begin; j < end - 2; j++)
    {
        if(array[j] <= pivot)
        {
             i++;
             swap(array, j, i);
        }
    }

    swap(array, i+1, end-1);
    return i + 1;
}

void quicksort(int* array, size_t begin, size_t end)
{
    if(begin < end)
    {
        size_t pivot = partition(array, begin, end);
        quicksort(array, begin, pivot);
        quicksort(array, pivot + 1, end);
    }
}

BOOST_AUTO_TEST_CASE(quicksort_test)
{
    int array[12] = {9, 5, 10, 8, 2, 1, 0, 3, 11, 4, 6, 7};
    size_t size = sizeof (array)/ sizeof (int);

    quicksort(array, 0, size);

    for(size_t i = 0; i < size; i++)
    {
        if(i == size - 1)
            break;
        BOOST_CHECK_MESSAGE(array[i] <= array[i + 1], "array wasn't sorted");
    }
}

BOOST_AUTO_TEST_SUITE_END()

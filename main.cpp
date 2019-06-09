#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <ctime>
#include <assert.h>
#include <fstream>
#include <math.h>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <type_traits>
#include <atomic>
#include <ctime>
#include <future>

#define BOOST_TEST_MODULE test_main

#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;
BOOST_AUTO_TEST_SUITE(test_suite_main)

typedef size_t (partitionProcedure)(int*, size_t, size_t);

void swap(int* array, size_t index1, size_t index2)
{
    int temp = array[index1];
    array[index1] = array[index2];
    array[index2] = temp;
}

size_t partitionRandomPivot(int* array, size_t begin, size_t end)
{
    std::srand(time(0));
    size_t pivotIndex = begin + std::rand() % (end - begin);
    int pivot = array[pivotIndex];
    size_t i = begin - 1;

    for(size_t j = begin; j < end; j++)
    {
        if(j == pivotIndex)
            continue;

        if(array[j] <= pivot)
        {
             i++;
             if(i == pivotIndex)
                 pivotIndex = j;
             swap(array, i, j);
        }
    }

    swap(array, i+1, pivotIndex);
    return i + 1;
}

size_t partitionLastPivot(int* array, size_t begin, size_t end)
{
    int pivot = array[end - 1];
    size_t i = begin - 1;

    for(size_t j = begin; j < end - 1; j++)
    {
        if(array[j] <= pivot)
        {
             i++;
             swap(array, i, j);
        }
    }

    swap(array, i+1, end-1);
    return i + 1;
}

void quicksort(int* array, size_t begin, size_t end, partitionProcedure partition)
{
    if(begin < end)
    {
        size_t pivot = partition(array, begin, end);
        quicksort(array, begin, pivot, partition);
        quicksort(array, pivot + 1, end, partition);
    }
}

int parallelQuicksort(int* array, size_t begin, size_t end, partitionProcedure partition)
{
    if(begin < end)
    {
        size_t pivot = partition(array, begin, end);
        auto left = std::async(std::launch::async, [&]()
        {
              return parallelQuicksort(array, begin, pivot, partition);
        });
        parallelQuicksort(array, pivot + 1, end, partition);
    }
    return 0;
}

BOOST_AUTO_TEST_CASE(quicksort_test_last_element_as_a_pivot)
{
    int array[12] = {9, 5, 10, 8, 2, 1, 0, 3, 11, 4, 6, 7};
    size_t size = sizeof (array)/ sizeof (int);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    quicksort(array, 0, size, partitionLastPivot);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();

    std::cout << "\nquicksort_test_last_element_as_a_pivot " << duration << " ns"<< std::endl;


    for(size_t i = 0; i < size; i++)
    {
        if(i == size - 1)
            break;
        BOOST_CHECK_MESSAGE(array[i] <= array[i + 1], "array wasn't sorted");
    }
}

BOOST_AUTO_TEST_CASE(quicksort_test_random_pivot)
{
    int array[12] = {9, 5, 10, 8, 2, 1, 0, 3, 11, 4, 6, 7};
    size_t size = sizeof (array)/ sizeof (int);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    quicksort(array, 0, size, partitionRandomPivot);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();

    std::cout << "\nquicksort_test_random_pivot " << duration << " ns" << std::endl;

    for(size_t i = 0; i < size; i++)
    {
        if(i == size - 1)
            break;
        BOOST_CHECK_MESSAGE(array[i] <= array[i + 1], "array wasn't sorted");
    }
}

BOOST_AUTO_TEST_CASE(quicksort_test_parallel_last_element_as_a_pivot)
{
    int array[12] = {9, 5, 10, 8, 2, 1, 0, 3, 11, 4, 6, 7};
    size_t size = sizeof (array)/ sizeof (int);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    parallelQuicksort(array, 0, size, partitionLastPivot);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << "\nquicksort_test_parallel_last_element_as_a_pivot " << duration << " ms" << std::endl;

    for(size_t i = 0; i < size; i++)
    {
        if(i == size - 1)
            break;
        BOOST_CHECK_MESSAGE(array[i] <= array[i + 1], "array wasn't sorted");
    }
}

BOOST_AUTO_TEST_CASE(quicksort_test_parallel_random_pivot)
{
    int array[12] = {9, 5, 10, 8, 2, 1, 0, 3, 11, 4, 6, 7};
    size_t size = sizeof (array)/ sizeof (int);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    parallelQuicksort(array, 0, size, partitionRandomPivot);

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    std::cout << "\nquicksort_test_parallel_random_pivot " << duration << " ms" << std::endl;

    for(size_t i = 0; i < size; i++)
    {
        if(i == size - 1)
            break;
        BOOST_CHECK_MESSAGE(array[i] <= array[i + 1], "array wasn't sorted");
    }
}

BOOST_AUTO_TEST_SUITE_END()

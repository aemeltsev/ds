#ifndef VECTOR_TESTS_H
#define VECTOR_TESTS_H

#include <iostream>
#include <ctime>
#include <gtest/gtest.h>

#include <vector>
#include "vector.h"

namespace vector_testing {
const std::size_t num_of_elements = 100001;
const int module = 1000000;
const double o1_time = 5 * 1e-06;

/**
 * @brief get_time_sec - get time in seconds
 * @return the time in seconds
 */
inline double get_time_sec()
{
    return static_cast<double>(clock())/ CLOCKS_PER_SEC;
}

/**
 * @brief get_new_time calculating the total time and setting the current time in seconds
 * @param sum_time - summary time
 * @param last_time - last time value
 */
void get_new_time(double &sum_time, double &last_time)
{
    sum_time += get_time_sec() - last_time;
    last_time = get_time_sec();
}

/**
 * @brief nop
 */
template <typename T>
void do_nothing(const T &elem)
{
    return;
}

/**
 * @brief container filling
 */
template<typename T>
void fill(scl::vector<T> *vec)
{
    for(std::size_t ind=0; ind<num_of_elements; ++ind)
    {
        vec->push_back(static_cast<int>(std::rand() % module));
    }
}

class VectorCheckFixture: public ::testing::Test
{
public:
    scl::vector<int> *vec;
    double sum_time = 0, last_time = 0;

    static void SetUpTestSuite() {std::cout << "Set Up Test Suite" << "\n";}
    static void TearDownTestSuite() {std::cout << "Tear Down Test Suite" << "\n";}

protected:

    void SetUp() override
    {
        vec = new scl::vector<int>;
        sum_time = 0;
        last_time = get_time_sec();
    }

    void TearDown() override
    {
        delete vec;
    }

};

class VectorCompareFixture: public ::testing::Test
{
public:
    scl::vector<int> *vec;
    std::vector<int> *vec_std;
    double sum_time, last_time;

    static void SetUpTestSuite() {std::cout << "Set Up Test Suite" << "\n";}
    static void TearDownTestSuite() {std::cout << "Tear Down Test Suite" << "\n";}

protected:

    void SetUp() override
    {
        vec = new scl::vector<int>;
        vec_std = new std::vector<int>;

        sum_time = 0;
        last_time = get_time_sec();
    }

    void TearDown() override
    {
        delete vec;
        delete vec_std;
    }
};

TEST_F(VectorCheckFixture, CheckPushBack)
{
    //Arrange
    //Act
    for(std::size_t i=0; i<num_of_elements; ++i)
    {
        vec->push_back(static_cast<int>(std::rand() % module));
        get_new_time(sum_time, last_time);
    }
    //Assert
    ASSERT_EQ(vec->size(), num_of_elements);

}

TEST_F(VectorCheckFixture, CheckPopBack)
{
    //Arrange
    fill(vec);
    last_time = get_time_sec();
    //Act
    for(std::size_t i=0; i<num_of_elements; ++i)
    {
        vec->pop_back();
        get_new_time(sum_time, last_time);
    }
    //Assert
    ASSERT_TRUE(vec->empty());

}

TEST_F(VectorCompareFixture, StandartOperations)
{
    for(std::size_t i=0; i<num_of_elements; ++i)
    {
        int k = static_cast<int>(std::rand() % module - module/2);
        vec->push_back(k);
        vec_std->push_back(k);
        ASSERT_EQ(vec->size(), vec_std->size());
        ASSERT_EQ(vec->front(), vec_std->front());
        ASSERT_EQ(vec->back(), vec_std->back());
    }
    const scl::vector<int> *cvec = new scl::vector<int>(*vec);
    for(std::size_t i=0; i<num_of_elements/2; ++i)
    {
        int k = rand() % module - module/2;
        ASSERT_EQ((*vec)[i], (*vec_std)[i]);
        ASSERT_EQ(vec->size(), cvec->size());
        ASSERT_EQ(vec->back(), vec_std->back());
        (*vec)[i] = k;
        (*vec_std)[i] = k;
        ASSERT_EQ((*vec)[i], (*vec_std)[i]);
        ASSERT_EQ(vec->front(), vec_std->front());
        ASSERT_EQ(vec->back(), vec_std->back());
    }
    delete cvec;
    for(std::size_t i=0; i<num_of_elements; ++i)
    {
        ASSERT_EQ(vec->front(), vec_std->front());
        ASSERT_EQ(vec->back(), vec_std->back());
        vec->pop_back();
        vec_std->pop_back();
        ASSERT_EQ(vec->size(), vec->size());
    }
    ASSERT_EQ(vec->empty(), vec_std->empty());
}


/*

int main()
{
    //
    scl::vector<int>::iterator iter = arr.begin();
    while(iter != arr.end())
    {
        if(*iter == 3){
            //
            iter = arr.erase(iter);
        }else{
            iter++;
        }
    }
    print_arr(std::cout, arr);


    //
    tmp = list.front();
    while(tmp != NULL)
    {
        if(tmp->m_val == 4.){
            tmp = list.insert(tmp, 1.);
            assert(tmp != NULL);
        }
        tmp = tmp->m_next;
    }
    print_list(std::cout, list);

    //delete 2-elements from tail
    list.pop_back();
    list.pop_back();
    print_list(std::cout, list);
    list.clear();
*/
}

#endif //VECTOR_TESTS_H

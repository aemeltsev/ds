#ifndef ARRAY_TESTS_H
#define ARRAY_TESTS_H

#include <iostream>
#include <ctime>
#include <gtest/gtest.h>

#include <forward_list>
#include "flist.h"

namespace flist_testing {
const std::size_t num_of_elements = 100000;
const int module = 10000;
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
void fill(scl::flist<T> *flist)
{
    for(std::size_t ind=0; ind<num_of_elements; ++ind)
    {
        flist->add(static_cast<T>(std::rand() % module));
    }
}

class FListCheckFixture: public ::testing::Test
{
public:
    scl::flist<int> *flist;
    double sum_time = 0, last_time = 0;

    static void SetUpTestSuite() {std::cout << "Set Up Test Suite" << "\n";}
    static void TearDownTestSuite() {std::cout << "Tear Down Test Suite" << "\n";}

protected:

    void SetUp() override
    {
        flist = new scl::flist<int>;
        sum_time = 0;
        last_time = get_time_sec();
    }

    void TearDown() override
    {
        delete flist;
    }

};

class VectorCompareFixture: public ::testing::Test
{
public:
    scl::flist<int> *flist;
    std::forward_list<int> *flist_std;
    double sum_time, last_time;

    static void SetUpTestSuite() {std::cout << "Set Up Test Suite" << "\n";}
    static void TearDownTestSuite() {std::cout << "Tear Down Test Suite" << "\n";}

protected:

    void SetUp() override
    {
        flist = new scl::flist<int>;
        flist_std = new std::forward_list<int>;

        sum_time = 0;
        last_time = get_time_sec();
    }

    void TearDown() override
    {
        delete flist;
        delete flist_std;
    }
};

TEST_F(FListCheckFixture, CheckAdd)
{
    //Arrange
    //Act
    for(std::size_t i=0; i<num_of_elements*2; )
    {
        ++i;
    }
    fill(flist);
    //Assert
    ASSERT_FALSE(flist->empty());
    ASSERT_EQ(flist->size(), num_of_elements);
}


TEST_F(FListCheckFixture, CheckAddAndTime)
{
    //Arrange
    //Act
    for(std::size_t i=0; i<num_of_elements*2; )
    {
        ++i;
    }
    for(std::size_t i=0; i<num_of_elements; ++i)
    {
        flist->add(static_cast<int>(std::rand() % module));
        get_new_time(sum_time, last_time);
    }
    //Assert
    ASSERT_FALSE(flist->empty());
    ASSERT_EQ(flist->size(), num_of_elements);
    ASSERT_LE(sum_time/num_of_elements, o1_time);
}

TEST_F(FListCheckFixture, CheckInsertAfter)
{
    //Arrange
    int odd=0;
    //Act
    for(std::size_t i=0; i<num_of_elements; )
    {
        ++i;
    }
    for(std::size_t i=0; i<num_of_elements/2; ++i)
    {
        if(i%2 == 0){
            flist->add(1);
            ++odd;
        }
        else{
            flist->add(static_cast<int>(std::rand() % module));
        }

    }
    /* num_of_elements/2 = 50000
     * odd = 25000
     * (num_of_elements/2)+odd = 75000
     */
    for(auto s_iter = flist->begin(); s_iter != flist->end(); s_iter++)
    {
        if(*s_iter == 1){
            s_iter = flist->insert_after(s_iter, 22);
            get_new_time(sum_time, last_time);
        }
    }
    //Assert
    ASSERT_FALSE(flist->empty());
    ASSERT_EQ(flist->size(), (num_of_elements/2)+odd+1);
    ASSERT_LE(sum_time/num_of_elements, o1_time);
}

TEST_F(FListCheckFixture, CheckErase)
{
    //Arrange
    int odd=0;
    //Act
    for(std::size_t i=0; i<num_of_elements; )
    {
        ++i;
    }
    for(std::size_t i=0; i<num_of_elements; ++i)
    {
        if(i%2 == 0){
            flist->add(1);
            ++odd;
        }
        else{
            flist->add(static_cast<int>(std::rand() % module));
        }

    }
    for(auto s_iter = flist->begin(); s_iter != flist->end(); s_iter++)
    {
        if(*s_iter == 1){
            flist->erase_after(s_iter);
            get_new_time(sum_time, last_time);
        }
    }
    //Assert
    ASSERT_FALSE(flist->empty());
    ASSERT_EQ(flist->size(), (num_of_elements/2));
    ASSERT_LE(sum_time/num_of_elements, o1_time);
}

TEST_F(FListCheckFixture, CheckPushFront)
{
    //Arrange
    //Act
    for(std::size_t i=0; i<num_of_elements; )
    {
        ++i;
    }
    for(std::size_t i=0; i<num_of_elements/2; ++i)
    {
        flist->add(static_cast<int>(std::rand() % module));
    }
    //Assert
    ASSERT_FALSE(flist->empty());
    ASSERT_EQ(flist->size(), (num_of_elements/2));
    for(std::size_t i=0; i<num_of_elements/2; ++i)
    {
        flist->push_front(std::rand() % module);
        get_new_time(sum_time, last_time);
    }
    //Assert
    ASSERT_EQ(flist->size(), num_of_elements);
    ASSERT_LE(sum_time/num_of_elements, o1_time);
}

TEST_F(FListCheckFixture, CheckPopFront)
{
    //Arrange
    //Act
    for(std::size_t i=0; i<num_of_elements; )
    {
        ++i;
    }
    //list.pop_front();
}

}

#endif //ARRAY_TESTS_H

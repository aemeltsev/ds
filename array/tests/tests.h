#ifndef ARRAY_TESTS_H
#define ARRAY_TESTS_H

#include <iostream>
#include <ctime>
#include <gtest/gtest.h>

#include "array.h"

namespace array_testing {

const int size = 10;
int count = 1;

scl::array<int, 0> farr;
scl::array<int, size> sarr{{1,2,3,4,5,6,7,8,9,10}};
scl::array<int, size> *tarr = new scl::array<int, size>;

TEST(CheckArr, CheckEmpty)
{
    //Arrange
    //Act
    //Assert
    ASSERT_EQ(farr.size(), 0);
    ASSERT_TRUE(farr.empty());
}

TEST(CheckArr, CheckFront)
{
    //Arrange
    //Act
    //Assert
    ASSERT_FALSE(sarr.empty());
    ASSERT_EQ(sarr.front(), 1);
}

TEST(CheckArr, CheckBack)
{
    //Arrange
    //Act
    //Assert
    ASSERT_FALSE(sarr.empty());
    ASSERT_EQ(sarr.back(), 10);
}

TEST(CheckArr, CheckSize)
{
    //Arrange
    //Act
    //Assert
    ASSERT_FALSE(sarr.empty());
    ASSERT_EQ(sarr.size(), 11);
}

TEST(CheckArr, CheckAccesByIndex)
{
    //Arrange
    //Act
    int first = 1;
    int second =(size/2)+1;
    int thrid = size;
    //Assert
    ASSERT_FALSE(sarr.empty());
    ASSERT_EQ(sarr[0], first);
    ASSERT_EQ(sarr[second-1], second);
    ASSERT_EQ(sarr[size-1], thrid);
}

TEST(CheckArr, CheckAt)
{
    //Arrange
    int first = size;
    int second = size+1;
    int thrid = size+20;
    //Act
    sarr.at(0) = first; //
    int i = (size/2)+1;
    sarr.at(i) = second;
    sarr.at(size-1) = thrid;
    //Assert
    ASSERT_FALSE(sarr.empty());
    ASSERT_EQ(sarr[0], first);
    ASSERT_EQ(sarr[i], second);
    ASSERT_EQ(sarr[size-1], thrid);
}

}

#endif //ARRAY_TESTS_H

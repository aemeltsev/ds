#ifndef ARRAY_TESTS_H
#define ARRAY_TESTS_H

#include <iostream>
#include <ctime>
#include <gtest/gtest.h>

#include <forward_list>
#include "flist.h"

namespace flist_testing {
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
void fill(scl::flist<T> *flist)
{
    for(std::size_t ind=0; ind<num_of_elements; ++ind)
    {
        flist->add(static_cast<int>(std::rand() % module));
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

/*template<class T>
void print_list(std::ostream &out, const typename scl::flist<T>& list)
{
    typename scl::flist<T>::iterator it;
    for(it = list.cbegin(); it!=list.cend(); it++)
    {
        out << *it << " ";
    }
    out << std::endl;
}


    scl::flist<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.add(5);
    print_list(std::cout, list);
    for(int i=0; i<16; ++i)
    {
        list.add(3 + static_cast<int>(std::rand()%3));
    }
    list.add(8);
    print_list(std::cout, list);

    //erase elements
    scl::flist<int>::iterator f_iter = list.begin();
    while(f_iter != list.end())
    {
        if(*f_iter == 3){
            //https://stackoverflow.com/questions/596162(second answer)
            f_iter= list.erase(f_iter);
        }
        else{
            f_iter++;
        }
    }
    print_list(stRUN_ALL_TESTS()d::cout, list);

    //insert elements after node
    scl::flist<int>::iterator s_iter = list.begin();
    while(s_iter != list.end())
    {
        if(*s_iter == 4){
            s_iter = list.insert_after(s_iter, 1);
        }
        else{
            s_iter++;
        }
    }
    print_list(std::cout, list);

    //push two elements before the head node and delete 1-st element from head
    list.push_front(3);
    list.push_front(3);
    list.pop_front();
    print_list(std::cout, list);
    list.clear();
*/

}

#endif //ARRAY_TESTS_H

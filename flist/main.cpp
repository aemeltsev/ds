#include <iostream>
#include <cassert>
#include "flist.h"

template<class T>
void print_list(std::ostream &out, const typename scl::flist<T>& list)
{
    typename scl::flist<T>::iterator it;
    for(it = list.cbegin(); it!=list.cend(); it++)
    {
        out << *it << " ";
    }
    out << std::endl;
}

int main()
{
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
    print_list(std::cout, list);

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

    return 0;
}

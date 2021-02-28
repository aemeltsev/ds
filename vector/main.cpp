#include <iostream>
#include <cassert>
#include "vector.h"

template<class T>
void print_list(std::ostream &out, const typename scl::vector<T>& vector)
{
    typename scl::vector<T>::iterator it;
    for(it = vector.cbegin(); it!=vector.cend(); it++)
    {
        out << *it << " ";
    }
    out << std::endl;
}

int main()
{
/*    scl::flist<int> list;
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

    scl::flist<int>::iterator iter = list.begin();
    while( iter != list.end())
    {
        if(*iter == 3){
            //
            iter= list.erase(iter);
        }
        else{
            iter++;
        }
    }
    print_list(std::cout, list);
/*
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
    return 0;
}

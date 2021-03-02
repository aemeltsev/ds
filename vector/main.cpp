#include <iostream>
#include <cassert>
#include "vector.h"

template<class T>
void print_arr(std::ostream &out, const typename scl::vector<T>& vector)
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
    scl::vector<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.push_back(3);
    print_arr(std::cout, arr);
    std::cout << "Size: " << arr.size() << "\n";
    std::cout << "Capacity: " << arr.capacity() << "\n";

    for(int i=0; i<16; ++i)
    {
        arr.push_back(3 + static_cast<int>(std::rand()%3));
    }
    arr.push_back(8);
    print_arr(std::cout, arr);
    std::cout << "Size: " << arr.size() << "\n";
    std::cout << "Capacity: " << arr.capacity() << "\n";

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

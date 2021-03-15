#ifndef ARRAY_H
#define ARRAY_H
#include <cstdint>
#include <string>
#include <new>
//#include <cstddef>
#include <cassert>


// Sequence containers library)
namespace scl {
/**
 * Very basic implementaion std`s array, but using classes
 * @brief This implementation of a conditionally "useful" array.
 */
template<class T, std::size_t N>
class array
{
private:
    T* m_arr;
    std::size_t m_size;

public:

    class iterator
    {
    public:
        iterator() noexcept {}
        explicit iterator(T* ptr)
            :m_ptr(ptr){}

        inline iterator& operator=(const iterator& other){m_ptr = other.m_ptr;}

        iterator& operator++(){++m_ptr; return *this;}
        iterator& operator--(){--m_ptr; return *this;}
        iterator operator++(int junk){iterator it = *this; ++m_ptr; return it;}
        iterator operator--(int junk){iterator it = *this; ++m_ptr; return it;}
        inline T& operator*(){return *m_ptr;}
        inline T* operator->(){return m_ptr;}
        bool operator==(const iterator& other){return m_ptr == other.m_ptr;}
        bool operator!=(const iterator& other){return m_ptr != other.m_ptr;}

    private:
        T* m_ptr;
    };

    array();
    ~array();
    array(std::initializer_list<T>& ilist);
    array(const array<T, N>& other);
    array& operator=(const array<T, N>& other);

    T& operator[](std::size_t pos);
    T& front();
    T& back();
    bool empty();
    std::size_t size();

    iterator begin();
    iterator cbegin() const;
    iterator end();
    iterator cend() const;
};

/**
 * @brief Default constructor
 */
template<class T, std::size_t N>
array<T, N>::array()
    :m_size(N)
{
    assert((m_size < 0) && "Size error - small size for allocate");
}

/**
 * @brief Destructor
 */
template<class T, std::size_t N>
array<T, N>::~array()
{
    delete[] m_arr;
}

/**
 * @brief For object make, constructor uses initializer_list
 */
template<class T, std::size_t N>
array<T, N>::array(std::initializer_list<T>& ilist)
    :m_size(ilist.size())
{
    m_arr = new (std::nothrow) T[m_size];
    if(m_arr != NULL)
    {
        std::size_t indx=0;
        for(auto &element : ilist)
        {
            m_arr[indx] = element;
            ++indx;
        }
    }
}

/**
 * @brief Copy constructor
 */
template<class T, std::size_t N>
array<T, N>::array(const array<T, N>& other)
    :m_size(other.m_size)
{
    m_arr = new (std::nothrow) T[m_size];
    if(m_arr != NULL)
    {
        for(std::size_t indx=0; indx<m_size; ++indx)
        {
            m_arr[indx] = other.m_arr[indx];
        }
    }
    else
    {
        assert(m_arr && "Array error: unable to allocate memory in default constructor");
    }
}

/**
 * @brief Assignment operator
 */
template<class T, std::size_t N>
array<T, N>& array<T, N>::operator=(const array<T, N>& other)
{
    if(this != &other)
    {
        delete[] m_arr;

        m_size = other.m_size;
        m_arr = new (std::nothrow) T[m_size];
        if(m_arr != NULL)
        {
            for(std::size_t indx=0; indx<m_size; ++indx)
            {
                m_arr[indx] = other.m_arr[indx];
            }
        }
        else
        {
            assert(m_arr && "Array error: unable to allocate memory in default constructor");
        }
    }
}

/**
 * @brief Operator - access by index
 * @param pos The position of the element.
 * @return A reference to the element at specified location index.
 *         With bounds checking.
 */
template<class T, std::size_t N>
T& array<T, N>::operator[](std::size_t pos)
{
    assert((pos<0 || pos>=m_size) && "Array error: pos out of range");
    return m_arr[pos];
}

/**
 * @brief Get a first element from the array.
 * @return The first element from the array
 */
template<class T, std::size_t N>
T& array<T, N>::front()
{
    return m_arr[0];
}

/**
 * @brief Get a back element from the array.
 * @return The back element from the array
 */
template<class T, std::size_t N>
T& array<T, N>::back()
{
    return m_arr[m_size-1];
}

/**
 * @brief Checks whether the container is empty.
 * @return True if is empty, false otherwise.
 */
template<class T, std::size_t N>
bool array<T, N>::empty()
{
    return m_size == 0;
}

/**
 * @brief The number of elements.
 * @return The number of elements in the container.
 */
template<class T, std::size_t N>
std::size_t array<T, N>::size()
{
    return m_size;
}

/**
 * @brief Operator equal-to
 * @param lhs The left operand of the expression. Transfer by the const reference
 * @param rhs The right operand of the expression. Transfer by the const reference
 * @return The bool value to returned
 */
template<class T, std::size_t N>
bool operator==(const array<T, N>& lhs, const array<T, N>& rhs)
{
    if(lhs.size() != rhs.size()){
        return false;
    }
    for(std::size_t indx=0; indx<N; ++indx)
    {
        if(lhs[indx] != rhs[indx]) return false;
    }
    return true;
}

/**
 * @brief Operator not equal-to
 * @param lhs The left operand of the expression. Transfer by the const reference
 * @param rhs The right operand of the expression. Transfer by the const reference
 * @return The bool value to returned
 */
template<class T, std::size_t N>
bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return !(lhs == rhs);
}

/**
 *
 */
template<class T, std::size_t N>
bool operator<(const array<T, N>& lhs, const array<T, N>& rhs)
{
    if(lhs.size() > rhs.size()){
        return false;
    }
    for(std::size_t indx=0; indx<N; ++indx)
    {
        if(lhs[indx] > rhs[indx]) return false;
    }
    return true;
}

/**
 *
 */
template<class T, std::size_t N>
bool operator>(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return (rhs < lhs);
}

template<class T, std::size_t N>
typename array<T, N>::iterator array<T, N>::begin()
{
    //TODO
}

template<class T, std::size_t N>
typename array<T, N>::iterator array<T, N>::cbegin() const
{
    //TODO
}

template<class T, std::size_t N>
typename array<T, N>::iterator array<T, N>::end()
{
    //TODO
}

template<class T, std::size_t N>
typename array<T, N>::iterator array<T, N>::cend() const
{
    //TODO
}

}

#endif //ARRAY_H

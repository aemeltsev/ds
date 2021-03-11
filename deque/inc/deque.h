#ifndef DEQUE_H
#define DEQUE_H
#include <cstdint>
#include <string>
#include <cstddef>
#include <limits>
#include <cassert>

/**
 * @brief DEFAULT_BUFFER_SIZE
 */
constexpr std::size_t DEFAULT_BUFFER_SIZE = 512U;

// Sequence containers library)
namespace scl {

/**
 * @brief chunk_size
 * @param chunk_size
 * @return
 */
inline std::size_t s_buff_size(std::size_t b_size)
{
    return (b_size < DEFAULT_BUFFER_SIZE) ? std::size_t(DEFAULT_BUFFER_SIZE/b_size) : std::size_t(1);
}

/**
 * See
 * https://stackoverflow.com/questions/6292332
 * https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_deque.h
 */
/**
 * @brief
 */
template<class T> class deque;

template<class T>
class deque_iterator{

    const deque<T>* m_deq_ptr;
    std::size_t m_indx_base;

public:
    using value_type = T;
    using iterator = deque_iterator<T>;

    deque_iterator();
    deque_iterator(const deque<T>& d_other);
    deque_iterator(const iterator& i_other);
    iterator& operator=(const iterator& i_other);

    T& retrive() const;
    bool valid() const;

    void init(const deque<T>& d_other);
    void rinit(const deque<T>& d_other);

    bool operator==(const iterator& i_other) const;
    bool operator!=(const iterator& i_other) const;
    T& operator*() const;
    T& operator[](std::size_t indx);
    iterator& operator++();
    iterator& operator++(int junk);
    iterator& operator--();
    iterator& operator--(int junk);

};

template<class T>
deque_iterator<T>::deque_iterator()
    :m_deq_ptr(nullptr)
    ,m_indx_base(0){}

template<class T>
deque_iterator<T>::deque_iterator(const deque<T>& d_other)
    :m_deq_ptr(&d_other)
    ,m_indx_base(0){}

template<class T>
deque_iterator<T>::deque_iterator(const typename deque_iterator<T>::iterator& i_other)
    :m_deq_ptr(i_other.m_deq_ptr)
    ,m_indx_base(i_other.m_indx_base){}

/**
 * @brief Basic implementation the double-ended queue using circular array
 */
template<class T>
class deque{

public:
    friend class deque_iterator<T>;
    using value_type = T;
    using iterator = deque_iterator<T>;

    std::size_t buffer_size(){return s_buff_size(sizeof(T));}

    deque();
    deque(std::size_t size, const T& val);
    deque(const deque<T>& other);

    ~deque();

    deque<T>& operator=(const deque<T>& other);

    T& operator[](std::size_t val);
    const T& operator[](std::size_t val) const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;
    void push_front(const T& val);
    void pop_front();
    void push_back(const T& val);
    void pop_back();
    void clear();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    iterator begin();
    iterator end();
    iterator rbegin();
    iterator rend();

private:
    T* m_buffer;
    std::size_t m_size;
    std::size_t m_begin;
    std::size_t m_end;
};

/**
 * @brief The equality operator overloaded
 * @param x The reference on to the first deque object
 * @param y The reference on to the second deque object
 * @return Bool value after checking deque size and checking every item individually
 */
template<class T>
bool operator==(const deque<T>& x, const deque<T> y)
{
    if(x.size() != y.size()){
        return false;
    }
    for(std::size_t i=0; i<x.size(); ++i)
    {
        if(x[i] != y[i]){
            return false;
        }
    }
    return true;
}

/**
 * @brief The not equality operator overloaded
 * @param x The reference on to the first deque object
 * @param x The reference on to the first deque object
 * @return Bool value using equality aperator
 */
template<class T>
bool operator!=(const deque<T>& x, const deque<T> y)
{
    return !(x == y);
}

/**
 * @brief Default constructor
 */
template<class T>
deque<T>::deque()
    :m_buffer(nullptr)
    ,m_size(0)
    ,m_begin(0)
    ,m_end(0)
{

    m_buffer = new (std::nothrow) T[buffer_size()];
    if(m_buffer != NULL)
    {
        m_size = buffer_size();
    }
    else
    {
        assert(m_buffer && "Deque error: unable to allocate memory in default constructor");
    }
}

/**
 * @brief Destructor
 */
template<class T>
deque<T>::~deque()
{
    delete [] m_buffer;
    m_size = 0;
}

/**
 * @brief Copy constructor
 */
template<class T>
deque<T>::deque(const deque<T>& other)
    :m_size(other.m_size)
    ,m_begin(other.m_begin)
    ,m_end(other.m_end)
{
    m_buffer = new (std::nothrow) T[buffer_size()];
    if(m_buffer != NULL)
    {
        for(std::size_t i=0; i<m_size; ++i)
        {
            m_buffer[i] = other.m_buffer[i];
        }
    }
    else
    {
        assert(m_buffer && "Deque error: unable to allocate memory in default constructor");
    }
}

/**
 * @brief Assignment operator
 */
template<class T>
deque<T>& deque<T>::operator=(const deque<T>& other)
{
    if(this != other)
    {
        T* new_buffer = new (std::nothrow) T[other.m_size];
        if(m_buffer != NULL)
        {
            delete [] m_buffer;
            m_buffer = new_buffer;
            m_size = other.m_size;
            m_begin = other.m_begin;
            m_end = other.m_end;

            for(std::size_t i=0; i<m_size; ++i)
            {
                m_buffer[i] = other.m_buffer[i];
            }
        }
        else
        {
            assert(m_buffer && "Deque error: unable to allocate memory in default constructor");
        }
    }
    return *this;
}

template<class T>
T& deque<T>::operator[](std::size_t val)
{
    assert(val>=size() && "Deque index value is out of range");
    return *(m_buffer + (m_begin + val) % m_size);
}

template<class T>
const T& deque<T>::operator[](std::size_t val) const
{
    assert(val>=size() && "Deque index value is out of range");
    return *(m_buffer + (m_begin + val) % m_size);
}

template<class T>
bool deque<T>::empty() const noexcept
{
    return size() == 0 ? true : false;
}

template<class T>
std::size_t deque<T>::size() const noexcept
{
    return (m_end - m_begin + m_size) % m_size;
}

template<class T>
void deque<T>::push_front(const T& val)
{
    //TODO
}

template<class T>
void deque<T>::pop_front()
{
    //TODO
}

template<class T>
void deque<T>::push_back(const T& val)
{
    //TODO
}

template<class T>
void deque<T>::pop_back()
{
    //TODO
}

template<class T>
void deque<T>::clear()
{
    //TODO
}

template<class T>
T& deque<T>::front()
{
    //TODO
}

template<class T>
const T& deque<T>::front() const
{
    //TODO
}

template<class T>
T& deque<T>::back()
{
    //TODO
}

template<class T>
const T& deque<T>::back() const
{
    //TODO
}

template<class T>
typename deque<T>::iterator deque<T>::begin()
{
    //TODO
}

template<class T>
typename deque<T>::iterator deque<T>::end()
{
    //TODO
}

template<class T>
typename deque<T>::iterator deque<T>::rbegin()
{
    //TODO
}

template<class T>
typename deque<T>::iterator deque<T>::rend()
{
    //TODO
}

}

#endif //DEQUE_H

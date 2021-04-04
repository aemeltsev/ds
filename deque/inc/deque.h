#ifndef DEQUE_H
#define DEQUE_H
#include <cstdint>
#include <string>
//#include <new>
//#include <cstddef>
#include <cassert>

// see https://www.cs.fsu.edu/~lacher/courses/COP5517/lectures/deques/script.html
/**
 * @brief DEFAULT_BUFFER_SIZE
 */
constexpr std::size_t DEFAULT_BUFFER_SIZE = 256U;

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
    friend class deque<T>;
    const deque<T>* m_deq_ptr;
    std::size_t m_indx_base;
    bool get_local(std::size_t val)
    {
        if(m_deq_ptr->m_begin <= m_deq_ptr->m_end)
        {
            return ((m_deq_ptr->m_begin <= val) && (val <= m_deq_ptr->m_end));
        }
        else
        {
            return (((m_deq_ptr->m_begin <= val) && (val < m_deq_ptr->m_size)) || val <= m_deq_ptr->m_end);
        }
    }

public:
    using value_type = T;
    using iterator = deque_iterator<T>;

    deque_iterator();
    deque_iterator(const deque<T> *d_other, std::size_t index);
    deque_iterator(const iterator& i_other);
    iterator& operator=(const iterator& i_other);

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
deque_iterator<T>::deque_iterator(const deque<T>* d_other, std::size_t index)
    :m_deq_ptr(d_other)
    ,m_indx_base(index){}

template<class T>
deque_iterator<T>::deque_iterator(const typename deque_iterator<T>::iterator& i_other)
    :m_deq_ptr(i_other.m_deq_ptr)
    ,m_indx_base(i_other.m_indx_base){}

template<class T>
typename deque_iterator<T>::iterator& deque_iterator<T>::operator=(const typename deque_iterator<T>::iterator& i_other)
{
    if(this != &i_other)
    {
        m_indx_base = i_other.m_indx_base;
        m_deq_ptr = i_other.m_deq_ptr;
    }
    return *this;
}

template<class T>
bool deque_iterator<T>::operator==(const iterator& i_other) const
{
    if((m_deq_ptr != i_other.m_deq_ptr)||(m_indx_base != i_other.m_indx_base))
    {
        return false;
    }
    return true;
}

template<class T>
bool deque_iterator<T>::operator!=(const iterator& i_other) const
{
    return !(*this == i_other);
}

template<class T>
T& deque_iterator<T>::operator*() const
{
    assert((m_deq_ptr == nullptr) && "m_deq_ptr - novalid pointer");
    assert(get_local(m_indx_base) && m_indx_base != m_deq_ptr->m_end);
    return *(m_deq_ptr->m_buffer + m_indx_base);
}

template<class T>
T& deque_iterator<T>::operator[](std::size_t indx)
{
    assert((m_deq_ptr == nullptr) && "m_deq_ptr - novalid pointer");
    assert(get_local(m_indx_base) && m_indx_base != m_deq_ptr->m_end);
    return m_deq_ptr->operator[](m_indx_base + indx);
}

template<class T>
typename  deque_iterator<T>::iterator& deque_iterator<T>::operator++()
{
    ++m_indx_base;
    return *this;
}

template<class T>
typename  deque_iterator<T>::iterator& deque_iterator<T>::operator++(int junk)
{
    iterator itr(*this);
    operator++();
    return itr;
}

template<class T>
typename  deque_iterator<T>::iterator& deque_iterator<T>::operator--()
{
    --m_indx_base;
    return *this;
}

template<class T>
typename  deque_iterator<T>::iterator& deque_iterator<T>::operator--(int junk)
{
    iterator itr(*this);
    operator--();
    return itr;
}

/**
 * @brief Basic implementation the double-ended queue using circular array
 */
template<class T>
class deque{

    void copy(std::size_t new_size);
    void check_realloc();
    void add_length();
    void sub_length();

public:
    //friend class deque_iterator<T>;
    //using value_type = T;
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
    const T& cfront() const;
    T& back();
    const T& cback() const;

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

template<class T>
void deque<T>::copy(std::size_t new_size)
{
    std::size_t prev_size = size();
    T* new_m_buffer = new (std::nothrow) T[new_size];
    if(new_m_buffer != NULL){
        for(std::size_t i = 0, p = m_begin; i < size(), p != m_end; p = (p+1) % m_size)
        {
            new_m_buffer[i] = m_buffer[p];
        }
        delete[] m_buffer;
        m_buffer = new_m_buffer;
        m_begin = 0;
        m_end = (new_size == m_size*2) ? (m_size-1) : prev_size;
    }
    else{
        assert(m_buffer && "Deque error: unable to allocate memory");
    }
}

template<class T>
void deque<T>::check_realloc()
{
    if(size() == m_size - 1)
    {
        add_length();
    }
    if((size()*4 == m_size) && m_size > 4)
    {
        sub_length();
    }
}

template<class T>
void deque<T>::add_length()
{
    copy(m_size*2);
    m_size *= 2;
}

template<class T>
void deque<T>::sub_length()
{
    copy(m_size/2);
    m_size /= 2;
}

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
    ,m_begin(0)
    ,m_end(0)
{

    m_buffer = new (std::nothrow) T[buffer_size()];
    if(m_buffer != NULL){
        m_size = buffer_size();
    }
    else{
        assert(m_buffer && "Deque error: unable to allocate memory");
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
    if(m_buffer != NULL){
        for(std::size_t i=0; i<m_size; ++i){
            m_buffer[i] = other.m_buffer[i];
        }
    }
    else{
        assert(m_buffer && "Deque error: unable to allocate memory");
    }
}

/**
 * @brief Assignment operator
 */
template<class T>
deque<T>& deque<T>::operator=(const deque<T>& other)
{
    if(this != other){
        T* new_buffer = new (std::nothrow) T[other.m_size];
        if(m_buffer != NULL){
            delete [] m_buffer;
            m_buffer = new_buffer;
            m_size = other.m_size;
            m_begin = other.m_begin;
            m_end = other.m_end;

            for(std::size_t i=0; i<m_size; ++i){
                m_buffer[i] = other.m_buffer[i];
            }
        }
        else{
            assert(m_buffer && "Deque error: unable to allocate memory");
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
    check_realloc();
    m_begin = (m_begin - 1 + m_size) % m_size;
    m_buffer[m_begin] = val;
}

template<class T>
void deque<T>::pop_front()
{
    assert((size() == 0) && "Deque error: pop_front() empty deque");
    check_realloc();
    m_begin = (m_begin + 1) % m_size;
}


template<class T>
void deque<T>::push_back(const T& val)
{
    check_realloc();
    m_buffer[m_end] = val;
    m_end = (m_end + 1) % m_size;
}

template<class T>
void deque<T>::pop_back()
{
    assert((size() == 0) && "Deque error: pop_back() empty deque");
    check_realloc();
    m_end = (m_end - 1 + m_size) % m_size;
}

template<class T>
void deque<T>::clear()
{
    m_size = 0;
    m_begin = 0;
    m_end = 0;
}

template<class T>
T& deque<T>::front()
{
    assert((m_begin == m_end) && "Deque error: front() called on empty deque");
    if (size() == 0){
        exit (EXIT_FAILURE);
    }
    return m_buffer[m_begin];
}

template<class T>
const T& deque<T>::cfront() const
{
    assert((m_begin == m_end) && "Deque error: cfront() called on empty deque");
    if (size() == 0){
        exit (EXIT_FAILURE);
    }
    return m_buffer[m_begin];
}

template<class T>
T& deque<T>::back()
{
    assert((m_begin == m_end) && "Deque error: back() called on empty deque");
    if (size() == 0){
        exit (EXIT_FAILURE);
    }
    if (m_end == 0){
        return m_buffer[m_size - 1];
    }
    return m_buffer[m_end - 1];
}

template<class T>
const T& deque<T>::cback() const
{
    assert((m_begin == m_end) && "Deque error: back() called on empty deque");
    if (size() == 0){
        exit (EXIT_FAILURE);
    }
    if (m_end == 0){
        return m_buffer[m_size - 1];
    }
    return m_buffer[m_end - 1];
}

template<class T>
typename deque<T>::iterator deque<T>::begin()
{
    iterator it;
    it.m_deq_ptr = this;
    it.m_indx_base = 0;
    return it;
}

template<class T>
typename deque<T>::iterator deque<T>::end()
{
    iterator it;
    it.m_deq_ptr = this;
    it.m_indx_base = size();
    return it;
}

template<class T>
typename deque<T>::iterator deque<T>::rbegin()
{
    iterator it;
    it.m_deq_ptr = this;
    it.m_indx_base = size()-1;
    return it;
}

template<class T>
typename deque<T>::iterator deque<T>::rend()
{
    iterator it;
    it.m_deq_ptr = this;
    it.m_indx_base = 0 - 1;
    return it;
}

}

#endif //DEQUE_H

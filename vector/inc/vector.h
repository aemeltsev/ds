#ifndef VECTOR_H
#define VECTOR_H
#include <cstdint>
#include <string>
#include <cmath>
#include <cstddef>
#include <limits>
#include <cassert>

/**
 * @brief MAX_VECTOR_SIZE - returns the largest finite value of the size_t type
 * @brief DEFAULT_CAPACITE_SIZE - use by default
 */
constexpr std::size_t MAX_VECTOR_SIZE = std::numeric_limits<std::size_t>::max();
constexpr std::size_t DEFAULT_CAPACITE_SIZE = 4U;

// Sequence containers library)
namespace scl {

/**
 * Very basic implementation std`s vector
 * @brief This implementation conditionally "useful" vector.
 *  It is controversially, maybe, the main useful methods - push_back, erase, operator[],
 *  besides the ctor, dtor.
 *  And anothers resize, clear, reserve etc.
 *
 */
template<class T>
class vector
{
private:
    T* m_arr = nullptr;
    std::size_t m_cap = DEFAULT_CAPACITE_SIZE;
    std::size_t m_size = 0;
    
    inline void move_storage(T* dest, T* from, std::size_t n);
    inline void swap(vector<T>& other);

public:

    class iterator
    {
    public:
        iterator(T* ptr)
            :m_ptr(ptr){}
        iterator operator++(){iterator it = *this; m_ptr++; return it;}
        iterator operator++(int junk){m_ptr++; return *this;}
        T& operator*(){return *m_ptr;}
        T* operator->(){return m_ptr;}
        bool operator==(const iterator& other){return m_ptr == other.m_ptr;}
        bool operator!=(const iterator& other){return m_ptr != other.m_ptr;}

    private:
        T* m_ptr;
    };

    vector() noexcept;
    explicit vector(std::size_t in);
    explicit vector(const vector<T>& other);
    ~vector();
    vector<T>& operator=(const vector<T>& other);

    inline std::size_t capacity() const {return m_cap;}
    inline std::size_t size() const {return m_size;}
    bool empty() const {return m_size == 0;}

    iterator begin(){return iterator(m_arr);};
    iterator cbegin() const {return iterator(m_arr);};
    iterator end(){return iterator(m_arr+m_size);};
    iterator cend() const {return iterator(m_arr+m_size);};
    T& front(){return m_arr[0];};
    T& back(){return m_arr[m_size-1];};
    void push_back(const T& data);
    void pop_back(){--m_size;};

    void reserve(std::size_t cap);
    void resize(std::size_t size);

    iterator erase(const iterator &pos);
    void clear();

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;
};

template<class T>
vector<T>::vector() noexcept
{
    m_arr = new (std::nothrow) T[m_cap];
}

template<class T>
vector<T>::vector(std::size_t in)
{
    m_size = in;
    m_cap = in+in/2+1;
    m_arr = new (std::nothrow) T[m_cap];
}

template<class T>
vector<T>::vector(const vector<T>& other)
{
    m_size = other.m_size;
    m_cap = other.m_cap;
    m_arr = new T[m_cap];
    for(std::size_t i=0; i<m_size; ++i)
    {
        m_arr[i] = other.m_arr[i];
    }
}

template<class T>
vector<T>::~vector()
{
    delete[] m_arr;
}

template<class T>
vector<T>& vector<T>::operator=(const vector<T>& other)
{
    if(this == &other){
        return *this;
    }
    swap(vector<T>(other));
    return *this;
}

/**
 * @brief
 */
template<class T>
void vector<T>::push_back(const T& data)
{
    if(m_size >= m_cap)
    {
        reserve(m_cap+m_cap/2 +1);
    }
    m_arr[m_size++] = data;
}

/**
 * @brief
 */
template<class T>
void vector<T>::reserve(std::size_t cap)
{
    if(m_arr == nullptr){
        m_size = 0;
        m_cap = 0;
    }
    T* new_arr = new (std::nothrow) T[cap];
    assert(new_arr);
    uint32_t l_size = cap < m_size ? cap : m_size;

    for(uint32_t indx=0; indx<l_size; ++indx)
    {
        new_arr[indx] = m_arr[indx];
    }
    m_cap = cap;
    delete [] m_arr;
    m_arr = new_arr;
}

/**
 * @brief - Resize policy
 */
template<class T>
void vector<T>::resize(std::size_t size)
{
    reserve(size);
    m_size = size;
}

template<class T>
typename vector<T>::iterator vector<T>::erase(const iterator& pos)
{
    iterator ptr_iter = &m_arr[pos-m_arr];
    ptr_iter->~T(); /**< Manually calling destructor for pos element >*/
    move_storage(pos, pos+1, m_size-(pos-m_arr)); /**< or use std::memmove >*/
    --m_size; /**< decrement size >*/
    return pos;
}

template<class T>
void vector<T>::clear()
{
    m_cap=0;
    m_size=0;
    m_arr = nullptr;
}

template<class T>
T& vector<T>::operator[](std::size_t index)
{
    assert(index>=0 && index<m_size);
    return m_arr[index];
}

template<class T>
const T& vector<T>::operator[](std::size_t index) const
{
    assert(index>=0 && index<m_size);
    return m_arr[index];
}

/**
 * @brief move_storage -
 * @param dest
 * @param from
 * @param n
 */
template<class T>
void vector<T>::move_storage(T* dest, T* from, std::size_t n)
{
    if(dest<from){
        T *tmp_dest=dest, *tmp_from=from;
        for(size_t i=0; i<n; i++){
            *tmp_dest++ = std::move(*tmp_from++);
        }
    }
    else if(dest>from){
        T *tmp_dest=dest+n-1, *tmp_from=from+n-1;
        for(size_t i=n; i>0; i--){
            *tmp_dest-- = std::move(*tmp_from--);
        }
    }
    else
        return;
}

/**
 *
 */
template<class T>
void vector<T>::swap(vector<T>& other)
{
    std::swap(m_size, other.m_size);
    std::swap(m_cap, other.m_cap);
    std::swap(m_arr, other.m_arr);
}
}

#endif //VECTOR_H

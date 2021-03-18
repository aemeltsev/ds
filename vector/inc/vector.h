#ifndef VECTOR_H
#define VECTOR_H
#include <cstdint>
#include <string>
//#include <cstddef>
#include <new>
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
        iterator() noexcept {}
        explicit iterator(T* ptr):m_ptr(ptr){}

        inline iterator& operator=(const iterator& other){m_ptr = other.m_ptr; return *this;}

        iterator& operator++(){++m_ptr; return *this;}//++i
        iterator& operator--(){--m_ptr; return *this;}//--i
        iterator operator++(int junk){iterator it = *this; ++m_ptr; return it;}//i++
        iterator operator--(int junk){iterator it = *this; --m_ptr; return it;}//i--
        inline T& operator*(){return *m_ptr;}
        inline T* operator->(){return m_ptr;}
        bool operator==(const iterator& other){return m_ptr == other.m_ptr;}
        bool operator!=(const iterator& other){return m_ptr != other.m_ptr;}
        friend vector<T>::iterator vector<T>::erase(iterator& pos);

    private:
        T* m_ptr;
    };

    vector() noexcept;
    explicit vector(std::size_t in);
    explicit vector(const vector<T>& other);
    ~vector();
    vector<T>& operator=(const vector<T>& other);

    inline std::size_t capacity() const;
    inline std::size_t size() const;
    bool empty() const;

    iterator begin();
    iterator cbegin() const;
    iterator end();
    iterator cend() const;
    T& front();
    T& back();
    void push_back(const T& data);
    void pop_back();

    void reserve(std::size_t cap);
    void resize(std::size_t size);

    iterator erase(iterator &pos);
    void clear();

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;
};

/**
 * @brief Default ctor
 */
template<class T>
vector<T>::vector() noexcept
{
    m_arr = new (std::nothrow) T[m_cap];
}

/**
 * @brief Ctor with set size parameters
 */
template<class T>
vector<T>::vector(std::size_t in)
{
    m_size = in;
    m_cap = in+in/2+1;
    m_arr = new (std::nothrow) T[m_cap];
}

/**
 * @brief Copy ctor
 */
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

/**
 * @brief Dtor
 */
template<class T>
vector<T>::~vector()
{
    delete[] m_arr;
}

/**
 * @brief Assignment operator CASI
 */
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
 * @brief The number of elements that the container has currently allocated space for.
 * @return The number of elements that can be held in currently allocated storage.
 */
template<class T>
std::size_t vector<T>::capacity() const
{
    return m_cap;
}

/**
 * @brief The number of elements.
 * @return The number of elements in the container.
 */
template<class T>
std::size_t vector<T>::size() const
{
    return m_size;
}

/**
 * @brief Checks whether the container is empty.
 * @return True if is empty, false otherwise.
 */
template<class T>
bool vector<T>::empty() const
{
    return m_size == 0;
}

/**
* @brief Begin iterator.
* @return An iterator to the first element.
*/
template<class T>
typename vector<T>::iterator vector<T>::begin()
{
    return iterator(m_arr);
}

/**
 * @brief Const begin iterator.
 * @return An const iterator to the first element.
 */
template<class T>
typename vector<T>::iterator vector<T>::cbegin() const
{
    return iterator(m_arr);
}

/**
 * @brief End iterator.
 * @return An iterator to one past the last element.
 */
template<class T>
typename vector<T>::iterator vector<T>::end()
{
    return iterator(m_arr+m_size);
}

/**
 * @brief Const end iterator.
 * @return An const iterator to one past the last element.
 */
template<class T>
typename vector<T>::iterator vector<T>::cend() const
{
    return iterator(m_arr+m_size);
}

/**
 * @brief Get a first element from the array.
 * @return The first element from the array
 */
template<class T>
T& vector<T>::front()
{
    assert(size() == 0 || capacity() == 0 && "vector is empty - size() == 0");
    return m_arr[0];
};

/**
 * @brief Get a back element from the array.
 * @return The back element from the array
 */
template<class T>
T& vector<T>::back()
{
    assert(size() == 0 || capacity() == 0 && "vector is empty - size() == 0");
    return m_arr[m_size-1];
}

/**
 * @brief Adds an element to the end.
 *        If no space to add an element, then the capacity of the vector is increased.
 * @param The const reference to the element for add.
 */
template<class T>
void vector<T>::push_back(const T& data)
{
    if(m_size >= m_cap)
    {
        reserve(m_cap+(m_cap/2)+1);
    }
    m_arr[m_size++] = data;
}

/**
 * @brief Removes the last element of the container.
 */
template<class T>
void vector<T>::pop_back()
{
    assert(size() == 0 && "vector is empty - size() == 0");
    //m_arr[m_size]->~T();
    --m_size;
}

/**
 * @brief Increases the capacity of the container to a value greater or equal to new_cap.
 *        If new_cap is greater than the current capacity(), new storage is allocated,
 *        otherwise the method does nothing.
 *        see https://stackoverflow.com/questions/43728706
 * @param cap new capacity of the container.
 */
template<class T>
void vector<T>::reserve(std::size_t cap)
{
    if(m_arr == nullptr){
        m_size = 0;
        m_cap = 0;
    }
    T* new_arr = new (std::nothrow) T[cap];
    assert(new_arr && "cannot allocate memory - new_arr");
    std::size_t l_size = cap < m_size ? cap : m_size;

    for(std::size_t indx=0; indx<l_size; ++indx)
    {
        new_arr[indx] = m_arr[indx];
    }
    m_cap = cap;
    delete[] m_arr;
    m_arr = new_arr;
}

/**
 * @brief Resize policy. Sets the size of the container to the specified value.
 * @param size new container size.
 */
template<class T>
void vector<T>::resize(std::size_t size)
{
    reserve(size);
    m_size = size;
}

/**
* @brief Erases an element at pos.
*        see https://codereview.stackexchange.com/questions/154008
* @param pos Iterator to the element to remove.
* @return Iterator following the last removed element.
*         If the iterator pos refers to the last element, the end() iterator is returned.
*/
template<class T>
typename vector<T>::iterator vector<T>::erase(iterator& pos)
{
    std::size_t i=0;
    auto it = (*this).begin();

    for(it; it != pos; it++, i++);

    for(auto it = pos++; it != (*this).end(); it++, i++){
        m_arr[i] = m_arr[i+1];
    }

    m_size--;
    return pos;

    //TODO
    //https://codereview.stackexchange.com/questions/231112
    /*
     * std::move(position + 1, end(), position);
     * --end_position;
     * std::allocator_traits<Alloc>::destroy(allocator, end_position);
     * return position;
     */

    /* http://www.cplusplus.com/forum/beginner/70325/
     * template<class T>
     * void Vec<T>::erase( iterator i )// iterator is of type T*
     * {
     *  // avail : points to one past the end of the last intialized object in the memory block created by allocator:allocate
     *  // data : points to the first storage in the memory block created by allocator::allocate
     *  	if(i < avail && i >= data){
     *      for(iterator it = i; it != avail -1; ++it)
     *  		{
     *  			iterator after = it +1;
                    *it = *after;
                }
            alloc.destroy(avail-1);
            --avail;
            }
       }
     */
}

/**
 * @brief Remove elements from the container
 *        The capacity and size of the vector resets.
 */
template<class T>
void vector<T>::clear()
{
    m_cap=0;
    m_size=0;
    m_arr = nullptr;
}

/**
 * @brief Operator array subscripting.
 * @param index The index of the element.
 * @return A reference to the element at specified location index.
 *         With bounds checking.
 */
template<class T>
T& vector<T>::operator[](std::size_t index)
{
    assert((index>=0 && index<m_size) && "index out of range - m_arr[index]");
    return m_arr[index];
}

/**
 * @brief Constant operator array subscripting
 * @param index The index of the element.
 * @return A constant reference to the element at specified location index.
 *         With bounds checking.
 */
template<class T>
const T& vector<T>::operator[](std::size_t index) const
{
    assert((index>=0 && index<m_size) && "index out of range - m_arr[index]");
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
 * @brief Deep swap two array use std::swap
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

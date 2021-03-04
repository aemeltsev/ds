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
inline std::size_t chunk_size(std::size_t chunk_size)
{
    return (chunk_size < DEFAULT_BUFFER_SIZE) ? std::size_t(DEFAULT_BUFFER_SIZE/chunk_size) : std::size_t(1);
}

/**
 * See
 * https://stackoverflow.com/questions/6292332
 * https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_deque.h
 */
template<class T, std::size_t buf_size=0>
class deque{

public:
    class iterator{
        typedef T**  map_pointer;
    private:
        /**
         * @brief The map of the pointers to the chunk.
         */
        map_pointer m_node;

        /**
         * @brief pointers to the chunk
         */
        T* m_current; /**< the current element of the chunk */
        T* m_first; /**< the begin element of the chunk */
        T* m_last; /**< the end element of the chunk */

        /**
         * @brief ch_size
         * @return
         */
        std::size_t ch_size()
        {
            return chunk_size(sizeof(T));
        }

        /**
         * @brief set_node
         * @param new_node
         */
        void set_node(map_pointer new_node)
        {
            m_node = new_node;
            m_first = *new_node;
            m_last = m_first + ch_size();
        }

    public:
        /**
         * @brief Ctor
         * @param x
         * @param y
         */
        iterator(T* x, map_pointer y)
            :m_current(x)
            ,m_first(*y)
            ,m_last(*y + ch_size())
            ,m_node(y)
        {}

        /**
         * @brief Ctor
         */
        iterator()
            :m_current(0)
            ,m_first(0)
            ,m_last(0)
            ,m_node(0)
        {}

        /**
         * @brief operator *
         * @return
         */
        inline T& operator*(){
            return *m_current;
        }

        /**
         * @brief operator++ Prefix forms of increment.
         * @return reference
         */
        iterator& operator++()
        {
            ++m_current;
            if(m_current == m_last){ /**< If it reach the end of the chunk. */
                set_node(m_node + 1); /**< Skip to the next chunk. */
                m_current = m_first;
            }
            return *this;
        }

        /**
         * @brief operator++ Postfix forms of increment.
         * @param junk Simple junk from different postfix prefix operations.
         * @return iterator
         */
        iterator operator++(int junk)
        {
            iterator tmp = *this;
            ++*this; /**< invoke prefix++ */
            return tmp;
        }

        /**
         * @brief operator-- prefix forms of decrement
         * @return reference
         */
        iterator& operator--()
        {
            if(m_current == m_first){ // if it pointer to the begin of the chunk
                set_node(m_node - 1); //skip to the prev chunk
                m_current = m_last;
            }
            --m_current;
            return *this;
        }

        /**
         * @brief operator-- Postfix forms of decrement.
         * @param junk Simple junk from different postfix prefix operations.
         * @return iterator
         */
        iterator operator--(int junk)
        {
            iterator tmp = *this;
            --*this;
            return tmp;
        }

        /**
         * @brief operator+=
         * @param num
         * @return
         */
        iterator& operator+=(std::ptrdiff_t num)
        {
            std::ptrdiff_t offset = num + (m_current + m_first);
            if(offset >= 0 && offset < std::ptrdiff_t(ch_size())){
                m_current += num;
            }
            else {
                std::ptrdiff_t node_offset;
                if(offset>0){
                    node_offset = offset/std::ptrdiff_t(ch_size());
                }else {
                    node_offset = -((-offset -1)/std::ptrdiff_t(ch_size())) - 1;
                }

                set_node(m_node+node_offset);

                m_current = m_first+(offset-node_offset*ch_size());
            }
            return *this;
        }

        /**
         * @brief operator+
         * @param num
         * @return
         */
        iterator operator+(std::ptrdiff_t num)
        {
            iterator tmp = *this;
            return tmp += num; /**< Reuse  operator+=. */
        }

        /**
         * @brief operator-=
         * @param num
         * @return
         */
        iterator& operator-=(std::ptrdiff_t num)
        {
            return *this += -num; /**< Reuse  operator+=. */
        }

        /**
         * @brief operator-
         * @param num
         * @return
         */
        iterator operator-(std::ptrdiff_t num)
        {
            iterator tmp = *this;
            return tmp -= num; /**< Reuse  operator-=. */
        }

        /**
         * @brief operator[] Random access (iterator can skip n steps) invoke operator + ,operator *
         * @param num
         * @return
         */
        T& operator[](std::ptrdiff_t num)
        {
            return *(*this + num);
        }

        friend inline std::ptrdiff_t operator-(const iterator& x, const iterator& y);
        friend inline std::ptrdiff_t operator+(const iterator& x, const iterator& y);
    };

private:
//TODO
public:
//TODO
};

}

#endif //DEQUE_H

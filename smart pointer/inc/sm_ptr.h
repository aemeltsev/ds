#ifndef SMPTR_H
#define SMPTR_H
#include <cassert>
#include <utility>

namespace pl {

template<typename T>
struct DefaultDeleter

{
    void operator()(T *ptr)
    {
        if(ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
};

template<typename T, typename Deleter = DefaultDeleter<T>>
class sm_ptr
{
private:
    T* m_data;
    Deleter m_deleter;

    void ptr_deleter()
    {
        if(m_data)
        {
            m_deleter(m_data);
            m_data = nullptr;
        }
    }

public:
    //prohibition implicit type conversion
    explicit sm_ptr(T* data = nullptr)
        :m_data(data)
    {}

    //remove copy ctor
    sm_ptr(const sm_ptr<T, Deleter>& other) = delete;

    sm_ptr(const sm_ptr<T, Deleter>&& other)
        :m_data(other.m_data)
    {
        other.m_data = nullptr;
    }

    //remove assignment operator
    sm_ptr<T, Deleter>& operator=(const sm_ptr<T, Deleter>& other) = delete;

    sm_ptr<T, Deleter>& operator=(const sm_ptr<T, Deleter>&& other)
    {
        if(&other == this)
        {
            return *this;
        }

        delete m_data;

        m_data = other.m_data;
        other.m_data = nullptr;

        return *this;
    }

    //remove assignment to other pointer
    sm_ptr<T, Deleter>& operator=(T* other) = delete;

    //use default deleter
    ~sm_ptr()
    {
        ptr_deleter();
    }

    T& operator*() const
    {
        assert(m_data);
        return *m_data;
    }

    T* operator->() const noexcept
    {
        return m_data;
    }

    T* get() const noexcept
    {
        assert(m_data);
        return m_data;
    }

    explicit operator bool() const noexcept
    {
        return *this == nullptr;
    }

    void swap(sm_ptr<T, Deleter>& other) noexcept
    {
        if(&other == this)
        {
            return *this;
        }

        std::swap(m_data, other.m_data);
    }

    T* release() noexcept
    {
        T *new_ptr = m_data;
        m_data = nullptr;
        return new_ptr;
    }

    void reset(T* ptr = *T()) noexcept
    {
        ptr_deleter();
        m_data = ptr;
    }
};
}

#endif //SMPTR_H

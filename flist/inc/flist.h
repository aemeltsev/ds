#ifndef FLIST_H
#define FLIST_H
#include <cstdint>
#include <string>
#include <stdexcept>

// Sequence containers library)
namespace scl {

//forward linked list
template<class T>
class flist
{
private:
    //node
    struct tnode
    {
        T m_val;
        tnode *m_next;
    };

    tnode *m_head;
    tnode *m_tail;
    std::size_t m_count;

    //private methdots
    void copy_flist(const flist<T> &other);
    // Search prev node for pos
    tnode* prev(tnode* pos);
    // Return next node for this
    tnode* next(tnode* pos);

public:
    class iterator
    {
    public:
        iterator() noexcept {}
        explicit iterator(tnode *node):ptr_node(node){}

        inline iterator& operator=(const iterator& other){ptr_node = other.ptr_node; return *this;}
        inline T& operator*(){return ptr_node->m_val;}
        //inline T* operator->(){return &ptr_node->m_val;}
        inline iterator& operator++(){ptr_node=ptr_node->m_next; return *this;} //++i
        inline iterator& operator++(int junk){iterator ret(*this); ++*this; return ret;} //i++
        inline bool operator ==(const iterator& other) const {return ptr_node == other.ptr_node;}
        inline bool operator !=(const iterator& other) const {return ptr_node != other.ptr_node;}
        friend flist<T>::iterator flist<T>::insert_after(const iterator &pos, const T &data);
        friend flist<T>::iterator flist<T>::erase_after(iterator& pos);
        friend void flist<T>::swap(iterator &first, iterator &second);

    private:
        tnode *ptr_node;
    };

    flist() noexcept;
    ~flist();
    explicit flist(const flist &other);
    flist &operator=(const flist &other);

    // Adds the provided value to the end of the linked list.O(1)
    void add(const T &data);
    // Insert node, with value data, before pos node
    iterator insert_after(const iterator& pos, const T &data);
    // Removes the element at pos
    iterator erase_after(iterator& pos);
    void push_front(const T& data);
    void pop_front();
    // Delete all nodes
    void clear();
    // Swapping of the two nodes
    void swap(iterator &first, iterator &second);
    //Capacity
    inline std::size_t size() const noexcept;
    inline bool empty() const noexcept;
    inline T get_value(tnode *node);
    inline void set_value(tnode *node, const T &val);
    //Access
    inline iterator begin();
    inline iterator cbegin() const;
    inline iterator end();
    inline iterator cend() const;
};

/**
 * @brief Default ctor
 */
template<class T>
flist<T>::flist() noexcept
    :m_head(nullptr)
    ,m_tail(nullptr)
    ,m_count(0)
{}

/**
 * @brief Copy ctor
 */
template<class T>
flist<T>::flist(const flist &other)
{
    copy_flist(other);
}

/**
 * @brief Assignment operator
 */
template<class T>
flist<T>& flist<T>::operator=(const flist &other)
{
    if(this == &other){
        return *this;
    }

    if(empty()){
        clear();
    }
    copy_flist(other);
    return *this;
}

/**
 * @brief Dtor
 */
template<class T>
flist<T>::~flist()
{
    clear();
}

/**
 * @brief
 */
template<class T>
void flist<T>::add(const T &data)
{
    //1. alloc new node instance.
    tnode *tmp = new (std::nothrow) tnode();
    if(tmp != NULL){
        tmp->m_val = data;
        tmp->m_next = nullptr;
        //2. checks whether list empty.
        if(m_head == nullptr){
            /* If the list is empty,
             * the new node is added simply by assigning the m_head
             * and m_tail references to the new node.
             */
            m_head = tmp;
            m_tail = tmp;
        }else{
            /* If the list is not empty, the node is added to the end
             * of the list and the m_tail reference is updated
             * to point to the new end of the list.
             */
            m_tail->m_next = tmp;
            m_tail = tmp;
        }
        ++m_count;
    }
}

/**
 * @brief
 */
template<class T>
typename flist<T>::iterator flist<T>::insert_after(const iterator& pos, const T &data)
{
    //Add temp node and this iterator position as current node
    tnode *result = new (std::nothrow) tnode();
    tnode *curr_node = pos.ptr_node;

    if(result != NULL){
        result->m_val = data;

        if(curr_node->m_next == nullptr){
            // If pos - tail node, adding after him
            curr_node->m_next = result;
        }else{
            // If insert in the middle of the list
            result->m_next = curr_node->m_next;
            curr_node->m_next = result;
        }
        m_count++;
        return iterator(result);
    }
}

/**
 * @brief
 */
template<class T>
typename flist<T>::iterator flist<T>::erase_after(iterator& pos)
{
    tnode* curr_pos = pos.ptr_node;
    tnode* tmp = curr_pos->m_next;

    curr_pos->m_next = tmp->m_next;
    delete tmp;
    --m_count;
    return iterator(curr_pos->m_next);
}

/**
 * @brief
 */
template<class T>
void flist<T>::push_front(const T& data)
{
    tnode *result = new (std::nothrow) tnode();

    if(result != NULL){
        result->m_val = data;

        result->m_next = m_head;
        m_head = result;
        ++m_count;
    }
}

/**
 * @brief
 */
template<class T>
void flist<T>::pop_front()
{
    if(m_count == 1){
        m_head = nullptr;
        m_tail = nullptr;
    }
    else{
        tnode *temp_head = m_head;
        m_head = m_head->m_next;
        delete temp_head;
    }
    --m_count;
}

/**
 * @brief
 */
template<class T>
void flist<T>::clear()
{
    if(empty()){
        return;
    }

    tnode *current_tnode = m_head;
    while(current_tnode != nullptr){
        tnode* next_tnode = current_tnode->m_next;
        delete current_tnode;
        current_tnode = next_tnode;
        --m_count;
    }
    m_head = nullptr;
    m_tail = nullptr;
}

/**
 *
 */
template<class T>
void flist<T>::swap(iterator& first, iterator& second)
{
    tnode* first_node = first.ptr_node;
    tnode* second_node = second.ptr_node;

    // First or second is not exist
    if((first_node == NULL)||(second_node == NULL)) return;

    // If one node is specified twice, there is nothing to change
    if(first_node == second_node) return;
    // If first and second node the are near, swap them
    if(second_node->m_next == first_node){
        tnode *tmp = first_node;
        first_node = second_node;
        second_node = tmp;
    }

    tnode *prev_first_node = prev(first_node);
    tnode *prev_second_node = prev(second_node);
    tnode *next_first_node = next(first_node);
    tnode *next_second_node = next(second_node);
    // Swap of neighboring nodes
    if(prev_first_node == second_node){
        if(prev_first_node != NULL){
            prev_first_node->m_next = second_node;
        }else{
            m_head = second_node;
        }
        second_node->m_next = first_node;
        first_node->m_next = second_node;
        return;
    }
    // Swap of distant nodes
    if(prev_first_node !=NULL){
        prev_first_node->m_next = second_node;
    }else{
        m_head = second_node;
    }
    if(second_node != NULL){
        prev_second_node->m_next = first_node;
    }
    else{
        m_head = first_node;
    }
    second_node->m_next = next_first_node;
    first_node->m_next = next_second_node;
}

template<class T>
inline std::size_t flist<T>::size() const noexcept
{
    return m_count;
}

template<class T>
inline bool flist<T>::empty() const noexcept
{
    return m_head == NULL;
}

//TODO - edit method using std::advance, or self implementation
template<class T>
inline T flist<T>::get_value(tnode *node)
{
    if(node != NULL)
    return node->m_val;
}

//TODO - edit method using std::advance, or self implementation
template<class T>
inline void flist<T>::set_value(tnode *node, const T &val)
{
    node->m-val = val;
}

template<class T>
inline typename flist<T>::iterator flist<T>::begin()
{
    if(empty()){
        throw std::runtime_error("List is empty.");
    }
    return iterator(m_head);
}

template<class T>
inline typename flist<T>::iterator flist<T>::cbegin() const
{
    if(empty()){
        throw std::runtime_error("List is empty.");
    }
    return iterator(m_head);
}

template<class T>
inline typename flist<T>::iterator flist<T>::end()
{
    return iterator(m_tail->m_next);
}

template<class T>
inline typename flist<T>::iterator flist<T>::cend() const
{
    return iterator(m_tail->m_next);
}

/**
 * @brief Private internal method. Check the head node of validity
 *        and then sequentially adds nodes with the given value.
 * @param other const reference other flist for copy
 */
template<class T>
void flist<T>::copy_flist(const flist<T>& other)
{
    if(other.m_head != nullptr){
        tnode *tmp = other.m_head;
        while(tmp != nullptr){
            this->add(tmp->m_val);
            tmp = tmp->m_next;
        }
    }
}

/**
 * @brief Private internal method.
 */
template<class T>
typename flist<T>::tnode* flist<T>::prev(tnode* pos)
{
    // If list empty, or one head node return
    if(empty()||(pos == m_head)){
        return pos;
    }
    tnode* tmp = m_head;
    // Next node equall pos, tmp - previev node
    while(tmp->m_next != pos)
    {
        tmp = tmp->m_next;
    }
    return tmp;
}

/**
 * @brief Private internal method.
 */
template<class T>
typename flist<T>::tnode* flist<T>::next(tnode* pos)
{
    if(empty()){
        return pos;
    }
    return pos->m_next;
}

}
#endif // FLIST_H

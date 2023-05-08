/// \file shared_ptr.hpp
/// \brief Defines the shared_ptr class.
#ifndef SMART_PTR___SHARED_PTR_H
#define SMART_PTR___SHARED_PTR_H

/// \brief A smart pointer that retains shared ownership of an object through a pointer.
/// \tparam object_type The type of the object.
template <class object_type>
class shared_ptr
{
public:
    // CONSTRUCTORS
    /// \brief Creates a new, empty shared_ptr instance.
    shared_ptr()
        : m_object(nullptr),
          m_use_count(nullptr)
    {}
    /// \brief Creates a new shared_ptr instance.
    /// \param pointer A pointer to an object instance to manage.
    shared_ptr(object_type* pointer)
        : m_object(pointer),
          m_use_count(new size_t(1))
    {}
    /// \brief Copy constructs from another shared pointer instance.
    /// \param other The shared_ptr instance to copy.
    shared_ptr(const shared_ptr<object_type>& other)
        : m_object(other.m_object),
          m_use_count(other.m_use_count)
    {
        // Increment use count.
        shared_ptr::increment_use_count();
    }
    /// \brief Move constructs from another shared pointer instance.
    /// \param other The shared_ptr instance to move.
    shared_ptr(shared_ptr<object_type>&& other)
        : m_object(other.m_object),
          m_use_count(other.m_use_count)
    {
        // Clear other's instance/use count.
        // NOTE: use count remains the same due to move.
        other.m_object = nullptr;
        other.m_use_count = nullptr;
    }
    ~shared_ptr()
    {
        // Decrement use count.
        shared_ptr::decrement_use_count();
    }

    // RESET
    /// \brief Resets the shared_ptr to nullptr.
    void reset()
    {
        // Decrement use count.
        shared_ptr::decrement_use_count();

        // Reset object and use count.
        shared_ptr::m_object = nullptr;
        shared_ptr::m_use_count = nullptr;
    }
    /// \brief Resets the shared_ptr to a new instance.
    /// \param pointer The pointer to the new object instance to manage.
    void reset(object_type* pointer)
    {
        // Decrement use count
        shared_ptr::decrement_use_count();

        // Store new object and create new reference count.
        shared_ptr::m_object = pointer;
        shared_ptr::m_use_count = new size_t(1);
    }

    // ASSIGNMENT
    /// \brief Copy assigns this shared_ptr from another shared_ptr.
    /// \param other The shared_ptr instance to copy.
    /// \return A reference to this shared_ptr.
    shared_ptr<object_type>& operator=(const shared_ptr<object_type>& other)
    {
        // Decrement current use count.
        shared_ptr::decrement_use_count();

        // Copy object and use count.
        shared_ptr::m_object = other.m_object;
        shared_ptr::m_use_count = other.m_use_count;

        // Increment new use count.
        shared_ptr::increment_use_count();

        return *this;
    }
    /// \brief Move assigns this shared_ptr from another shared_ptr.
    /// \param other The shared_ptr instance to move.
    /// \return A reference to this shared_ptr.
    shared_ptr<object_type>& operator=(shared_ptr<object_type>&& other)
    {
        // Decrement current use count.
        shared_ptr::decrement_use_count();

        // Copy object and use count.
        shared_ptr::m_object = other.m_object;
        shared_ptr::m_use_count = other.m_use_count;

        // NOTE: Use count for new object remains the same.

        // Clear object and use count on other instance.
        other.m_object = nullptr;
        other.m_use_count = nullptr;
        
        return *this;
    }

    // ACCESS
    /// \brief Gets the pointer to the managed object instance.
    /// \return A pointer to the object instance.
    object_type* get() const
    {
        return shared_ptr::m_object;
    }
    /// \brief Dereferences the pointer to the managed object instance.
    /// \return A pointer to the object instance.
    object_type* operator->() const
    {
        return shared_ptr::m_object;
    }
    /// \brief Dereferences the pointer to the managed object instance.
    /// \return A reference to the object instance.
    object_type& operator*() const
    {
        return *shared_ptr::m_object;
    }
    /// \brief Checks if this shared_ptr references an object instance.
    /// \return TRUE if this shared_ptr references an object instanec, false if it is nullptr.
    operator bool() const
    {
        return shared_ptr::m_object != nullptr;
    }

    // USE
    /// \brief Gets the number of shared_ptrs referencing the managed object instance.
    /// \return The number of references.
    size_t use_count() const
    {
        // Check if use count exists.
        if(shared_ptr::m_use_count)
        {
            return *shared_ptr::m_use_count;
        }
        else
        {
            return 0;
        }
    }
    /// \brief Indicates if this shared_ptr is the only reference to the managed object instance.
    /// \return TRUE if the shared_ptr is the only reference, otherwise FALSE.
    bool unique() const
    {
        // Check if use count exists.
        if(shared_ptr::m_use_count)
        {
            return *shared_ptr::m_use_count == 1;
        }
        else
        {
            return false;
        }
    }

private:
    // OBJECT
    /// \brief A pointer to the shared object instance.
    object_type* m_object;
    /// \brief A pointer to the shared reference count of the object instance.
    size_t* m_use_count;

    // USE COUNT
    /// \brief Decrements the use count of the shared object, and frees it if no more references exist.
    void decrement_use_count()
    {
        // Decrement use count and check if its zero.
        if(shared_ptr::m_use_count && --(*shared_ptr::m_use_count) == 0)
        {
            // Clean up managed object.
            delete shared_ptr::m_object;
            // Clean up use count.
            delete shared_ptr::m_use_count;
        }
    }
    /// \brief Increments the use count of the shared object.
    void increment_use_count()
    {
        // Check if there is a managed object.
        if(shared_ptr::m_use_count)
        {
            ++(*shared_ptr::m_use_count);
        }
    }
};

// UTILITIES
/// \brief Creates a shared_ptr managing a new instance of an object.
/// \tparam object_type The type of the object.
/// \tparam args The variadic types of the object's constructor parameters.
/// \param arguments The arguments to pass to the object's constructor.
/// \return A shared_ptr managing a new instance of the object.
template <class object_type, class... args>
shared_ptr<object_type> make_shared(args&&... arguments)
{
    return shared_ptr<object_type>(new object_type(arguments...));
}

#endif
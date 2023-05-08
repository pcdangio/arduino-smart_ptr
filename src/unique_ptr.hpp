/// \file unique_ptr.hpp
/// \brief Defines the unique_ptr class.
#ifndef SMART_PTR___UNIQUE_PTR_H
#define SMART_PTR___UNIQUE_PTR_H

/// \brief A smart pointer that retains unique ownership of an object through a pointer.
/// \tparam object_type The type of the object.
template <class object_type>
class unique_ptr
{
public:
    // CONSTRUCTORS
    /// \brief Creates a new, empty unique_ptr instance.
    unique_ptr()
        : m_object(nullptr)
    {}
    /// \brief Creates a new unique_ptr instance.
    /// \param pointer A pointer to an object instance to manage.
    unique_ptr(object_type* pointer)
        : m_object(pointer)
    {}
    /// \brief Move constructs from another unique pointer instance.
    /// \param other The unique_ptr instance to move.
    unique_ptr(unique_ptr<object_type>&& other)
        : m_object(other.m_object)
    {
        // Clear other's instance.
        other.m_object = nullptr;
    }
    unique_ptr(const unique_ptr<object_type>& other) = delete;
    ~unique_ptr()
    {
        // Delete the object instance.
        delete unique_ptr::m_object;
    }

    // RESET
    /// \brief Resets the unique_ptr to nullptr.
    void reset()
    {
        // Delete and reset the object instance.
        delete unique_ptr::m_object;
        unique_ptr::m_object = nullptr;
    }
    /// \brief Resets the unique_ptr to a new instance.
    /// \param pointer The pointer to the new object instance to manage.
    void reset(object_type* pointer)
    {
        // Delete old instance and store new instance.
        delete unique_ptr::m_object;
        unique_ptr::m_object = pointer;
    }
    
    // ASSIGNMENT
    /// \brief Move assigns this unique_ptr from another unique_ptr.
    /// \param other The unique_ptr instance to move.
    /// \return A reference to this unique_ptr.
    unique_ptr<object_type>& operator=(unique_ptr<object_type>&& other)
    {
        // Delete old instance.
        delete unique_ptr::m_object;

        // Store new instance.
        unique_ptr::m_object = other.m_object;

        // Remove instance from other object.
        other.m_object = nullptr;
        
        return *this;
    }
    unique_ptr<object_type>& operator=(const unique_ptr<object_type>& other) = delete;

    // ACCESS
    /// \brief Gets the pointer to the managed object instance.
    /// \return A pointer to the object instance.
    object_type* get() const
    {
        return unique_ptr::m_object;
    }
    /// \brief Dereferences the pointer to the managed object instance.
    /// \return A pointer to the object instance.
    object_type* operator->() const
    {
        return unique_ptr::m_object;
    }
    /// \brief Dereferences the pointer to the managed object instance.
    /// \return A reference to the object instance.
    object_type& operator*() const
    {
        return *unique_ptr::m_object;
    }
    /// \brief Checks if this unique_ptr references an object instance.
    /// \return TRUE if this unique_ptr references an object instanec, false if it is nullptr.
    operator bool() const
    {
        return unique_ptr::m_object != nullptr;
    }

private:
    // OBJECT
    /// \brief A pointer to the unique object instance.
    object_type* m_object;
};

// UTILITIES
/// \brief Creates a unique_ptr managing a new instance of an object.
/// \tparam object_type The type of the object.
/// \tparam args The variadic types of the object's constructor parameters.
/// \param arguments The arguments to pass to the object's constructor.
/// \return A unique_ptr managing a new instance of the object.
template <class object_type, class... args>
unique_ptr<object_type> make_unique(args&&... arguments)
{
    return unique_ptr<object_type>(new object_type(arguments...));
}

#endif
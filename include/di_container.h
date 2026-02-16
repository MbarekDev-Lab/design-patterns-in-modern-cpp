#pragma once

#include <memory>
#include <map>
#include <functional>
#include <typeinfo>
#include <iostream>

using namespace std;

// ============================================================================
// DEPENDENCY INJECTION CONTAINER - Custom Implementation
// ============================================================================
// A simplified DI container that demonstrates the core concepts of
// dependency injection without external libraries.
// ============================================================================

namespace di
{
    // ========================================================================
    // TYPE WRAPPER - Store type information
    // ========================================================================

    class TypeWrapper
    {
    public:
        explicit TypeWrapper(const type_info &info) : info_(info) {}

        bool operator==(const TypeWrapper &other) const
        {
            return info_.get() == other.info_.get();
        }

        bool operator<(const TypeWrapper &other) const
        {
            return info_.get().before(other.info_.get());
        }

        const type_info &get() const { return info_.get(); }

    private:
        reference_wrapper<const type_info> info_;
    };

    // ========================================================================
    // BINDING - Maps interface to implementation
    // ========================================================================

    class Binding
    {
    public:
        Binding(const TypeWrapper &from, const TypeWrapper &to,
                function<void *()> creator)
            : from_(from), to_(to), creator_(creator)
        {
        }

        const TypeWrapper &from() const { return from_; }
        const TypeWrapper &to() const { return to_; }
        void *create() const { return creator_(); }

    private:
        TypeWrapper from_;
        TypeWrapper to_;
        function<void *()> creator_;
    };

    // ========================================================================
    // INJECTOR - The DI Container
    // ========================================================================

    class Injector
    {
    public:
        Injector() = default;

        // Register a binding
        void bind(const Binding &binding)
        {
            bindings_.insert_or_assign(binding.from(), binding);
        }

        // Create an instance with dependencies
        template <typename T>
        shared_ptr<T> create()
        {
            TypeWrapper type_key(typeid(T));

            auto it = bindings_.find(type_key);
            if (it != bindings_.end())
            {
                void *ptr = it->second.create();
                return shared_ptr<T>(static_cast<T *>(ptr));
            }

            // If not found, create directly
            return make_shared<T>();
        }

        // Get singleton instance
        template <typename T>
        shared_ptr<T> get_singleton()
        {
            TypeWrapper type_key(typeid(T));

            auto it = singletons_.find(type_key);
            if (it != singletons_.end())
            {
                return static_pointer_cast<T>(it->second);
            }

            auto instance = create<T>();
            singletons_.insert_or_assign(type_key, instance);
            return instance;
        }

    private:
        map<TypeWrapper, Binding> bindings_;
        map<TypeWrapper, shared_ptr<void>> singletons_;
    };

    // ========================================================================
    // DSL - Domain Specific Language for binding
    // ========================================================================

    class BindDsl
    {
    public:
        explicit BindDsl(const TypeWrapper &from) : from_(from) {}

        template <typename TImpl>
        Binding to()
        {
            return Binding(from_, TypeWrapper(typeid(TImpl)),
                           []()
                           { return new TImpl(); });
        }

        template <typename TImpl, typename TFactory>
        Binding to(TFactory factory)
        {
            return Binding(from_, TypeWrapper(typeid(TImpl)),
                           [factory]()
                           { return new TImpl(factory()); });
        }

    private:
        TypeWrapper from_;
    };

    // ========================================================================
    // CONVENIENCE FUNCTIONS
    // ========================================================================

    template <typename T>
    BindDsl bind()
    {
        return BindDsl(TypeWrapper(typeid(T)));
    }

    class ContainerBuilder
    {
    public:
        ContainerBuilder &add_binding(const Binding &binding)
        {
            injector_.bind(binding);
            return *this;
        }

        Injector build() const { return injector_; }

    private:
        Injector injector_;
    };

} // namespace di

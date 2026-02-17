#pragma once

#include <map>
#include <memory>
#include <iostream>
#include <string>

using namespace std;

// ============================================================================
// MULTITON PATTERN
// ============================================================================
// Multiton is a variation of the Singleton pattern that creates a pool of
// singletons, accessed by key. Each key has exactly one instance.
// ============================================================================

namespace multiton_violation
{
    // ========================================================================
    // PROBLEM: Managing multiple instances without pattern
    // ========================================================================

    class Logger
    {
    public:
        Logger(const string &name) : name_(name), id_(++counter_)
        {
            cout << "Logger [" << name_ << "] created (ID: " << id_ << ")\n";
        }

        void log(const string &message) const
        {
            cout << "[" << name_ << "] " << message << "\n";
        }

    private:
        string name_;
        int id_;
        static int counter_;
    };
    int Logger::counter_ = 0;

    // Problem: Caller must manage instance lifecycle
    // This creates duplicate instances and wastes memory
    class LoggerManager
    {
    public:
        static shared_ptr<Logger> get_logger(const string &name)
        {
            // ❌ WRONG: Creates new instance every time!
            return make_shared<Logger>(name);
        }
    };

} // namespace multiton_violation

namespace multiton_solution
{
    // ========================================================================
    // SOLUTION: Multiton Pattern
    // ========================================================================
    // Each key maps to exactly one singleton instance.
    // Provides both pooling and singleton benefits.

    template <typename T, typename Key = string>
    class Multiton
    {
    public:
        static shared_ptr<T> get(const Key &key)
        {
            if (const auto it = instances_.find(key); it != instances_.end())
            {
                return it->second;
            }

            auto instance = make_shared<T>(key);
            instances_[key] = instance;
            return instance;
        }

        // Get all instances (for testing/cleanup)
        static const map<Key, shared_ptr<T>> &get_all_instances()
        {
            return instances_;
        }

        // Clear all instances (for cleanup)
        static void clear_all()
        {
            instances_.clear();
        }

    protected:
        Multiton() = default;
        virtual ~Multiton() = default;

    private:
        static map<Key, shared_ptr<T>> instances_;
    };

    template <typename T, typename Key>
    map<Key, shared_ptr<T>> Multiton<T, Key>::instances_;

    // ========================================================================
    // ALTERNATE: Using enum keys
    // ========================================================================
    template <typename T, typename KeyEnum>
    class EnumMultiton
    {
    public:
        static shared_ptr<T> get(KeyEnum key)
        {
            if (const auto it = instances_.find(key); it != instances_.end())
            {
                return it->second;
            }

            auto instance = make_shared<T>();
            instances_[key] = instance;
            return instance;
        }

        static int instance_count()
        {
            return instances_.size();
        }

    protected:
        EnumMultiton() = default;
        virtual ~EnumMultiton() = default;

    private:
        static map<KeyEnum, shared_ptr<T>> instances_;
    };

    template <typename T, typename KeyEnum>
    map<KeyEnum, shared_ptr<T>> EnumMultiton<T, KeyEnum>::instances_;

} // namespace multiton_solution

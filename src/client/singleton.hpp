#pragma once

#include <memory>
#include <utility>
#include <stdexcept>

template <class T>
class Singleton {
public:
    template <class... Args>
    static void initialize(Args&&... args)
    {
        _ptr = std::make_unique<T>(std::forward<Args>(args)...);
    }

    static T& instance()
    {
        if (!_ptr) {
            throw std::runtime_error(
                "Singleton: instance() called before initialize()");
        }
        return *_ptr;
    }

protected:
    Singleton() {}
    
private:
    static std::unique_ptr<T> _ptr;
};

template <class T>
std::unique_ptr<T> Singleton<T>::_ptr;
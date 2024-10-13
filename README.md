# Expose Api
A modern C++ single-header library for static and runtime reflection of class members (functions and variables)

The library is mainly tested  on GCC and Clang, using C++20, but C++17 works as well, though it is not officially supported.

MSVC and other compilers are not officially supported. As of writing, the library compiles, but it's not guaranteed that new version will continue to work.

The example in main.cpp shows how to use the reflection system to access class members, without knowing the type of the instance.

All you need to know is in the example, the library is plug-and-play. Just include the header and start using the reflection system.

```cpp
#include "expose_api.hpp"
#include <iostream>

using namespace mgm;


// You must inherit from ExposeApiRuntime to use the runtime features of the reflection system, but it is not necessary for static reflection
class Foo : public ExposeApiRuntime {
public:
    // Enable the reflection system for this class
    // This macro needs to be the first thing in the class
    EXPOSE_CLASS("Foo")

    // Declare a function member with the reflection system, that accepts two integers
    void MFUNC(function)(int a, int b);

    // Declare a different function, that returns an int
    int MFUNC(sum)();

    // Declare 2 variable members
    int MVAR(x);
    int MVAR(y);

    // Default constructor
    Foo() : x(0), y(0) {}
};

// Definition for the first reflected function
void Foo::function(int a, int b) {
    x = a;
    y = b;
}

// Definition for the second reflected function
int Foo::sum() {
    return x + y;
}


int main() {
    // Create an instance of Foo
    Foo* foo = new Foo{};

    // Cast the instance to ExposeApiRuntime
    // This isn't needed to access the reflection system, either runtime or static
    // This is done to show accessing members without knowing the type of the instance
    ExposeApiRuntime* foo_runtime = dynamic_cast<ExposeApiRuntime*>(foo);

    // Get a function member
    auto function = foo_runtime->get_member("function");

    // Check if the member is a function
    if (function.is_function()) {
        // Set x to 13 and y to 17
        function.call(13, 17);
    }

    // Get a variable member (same as a function)
    auto x = foo_runtime->get_member("x");

    // Check if the member is a variable    
    if (x.is_variable()) {
        // Set x to 42
        x.get<int>() = 42;
    }

    // Get another function member
    auto sum = foo_runtime->get_member("sum");

    if (sum.is_function()) {
        // Get the sum of x and y
        // This time we need to specify the return type, because it's not void, and the api cannot deduce it automatically
        int result = sum.call<int>();

        // Print the result (should be 42 + 17 = 59)
        std::cout << "The sum of x and y is: " << result << std::endl;
    }
    
    // Get a multimap of string names, to members
    // It is a multimap because there can be multiple functions with the same name, but different signatures
    auto members = foo_runtime->get_all_members();

    for (auto& [name, member] : members) {
        // Check if the member is a variable
        if (member.is_variable()) {
            member.get<int>() = 0;
        }
    }

    // You can still call functions directly, without using the reflection system
    int result = foo->sum();

    // Print the result
    // The original class is affected by changes made through the reflection system
    // After changing both x and y to 0, the result should be 0
    std::cout << "The sum of x and y is: " << result << std::endl;
    
    return 0;
}
```

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
    void MFUNC(function)(int a, float b);

    // Declare a different function, that returns an int
    int MFUNC_CONST(sum)();

    // Declare 2 variable members
    int MVAR(x);
    int MVAR(y);

    // Default constructor
    Foo() : x(0), y(0) {}
};

// Definition for the first reflected function
void Foo::function(int a, float b) {
    x = a;
    y = (int)b;
}

// Definition for the second reflected function
int Foo::sum() const {
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
        // The types need to be correct, because the api cannot deduce them automatically, so make sure to pass the correct types
        function.call(13, 17.0f);
    }

    // Get a variable member (same as a function)
    auto x = foo_runtime->get_member("x");

    // Check if the member is a variable    
    if (x.is_variable()) {
        // Set x to 42
        x.get<int>() = 42;
    }

    // Get another function member
    // This one can be marked const, because the original function is also const
    const auto sum = foo_runtime->get_member("sum");

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

    // You can also get the name of the type through the reflection system
    std::cout << "The type name of the instance is: " << foo_runtime->get_type_name() << std::endl;
    
    return 0;
}
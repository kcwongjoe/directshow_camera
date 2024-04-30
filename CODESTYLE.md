# Programming Style

// Following the google c++ style guide
// https://google.github.io/styleguide/cppguide.html

```cpp
#ifndef MY_HEADER_H // _H suffix
#define MY_HEADER_H

const int THIS_IS_A_CONSTANT = 0;
int g_myVariable = 0; // g_ prefix for global variable

class MyClass
{
    public:
        int publicVariable;
        static int s_myVariable;     // s_ prefix for static variable

        void DoSomething(bool firstVriable)
        {
            bool secondVariable = m_myVariable;
        }

        // Getter and Setter for m_myVariable
        bool getVariable();
        void setVariable(bool);
        bool isVariable();

    private:
        bool m_myVariable; // m_ prefix for class member
}

#endif
```
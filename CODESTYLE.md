# Programming Style

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

        void doSomething(bool firstVriable)
        {
            bool secondVariable = m_variable;
        }

        // Getter and Setter for m_variable
        bool getVariable();
        void setVariable(bool);

    private:
        bool m_myVariable; // m_ prefix for class member
}

#endif
```
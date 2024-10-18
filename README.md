# String-Like_number

### support Integer, Float as string

#### utility

- support base operation (add, subtract, product, divide)
- can use C-Like printf (with several format)
- many other method is include

Example

```c++
#include "ustring.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    ustr::string_int n1 = "123";
    ustr::string_float n2 = "1.23";

    n1.uprintf("This is printf: %d\n");
    // "This is printf: 123"
    n2.uprintf("This is printf: %f\n");
    // "This is printf: 1.23"

    std::cout << n1 << std::endl << n2 << std::endl;
    // same as uprintf at above
    // 123
    // 1.23
}
```

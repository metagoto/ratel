#include <iostream>


#ifndef NDEBUG

#define LOG(a) std::cout << a << std::endl;

#else

#define LOG(a)

#endif

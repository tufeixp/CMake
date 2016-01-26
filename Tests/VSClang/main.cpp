// use some fancy C++14 feature that VS doesn't have

#include <stdio.h>

#ifdef __clang_version__ 
template<typename T>
constexpr T doThing{};
#endif

int main(){
#ifdef __clang_version__ 
    printf("This was compiled with Clang\r\n");
#else
    printf("This was compiles with CL\r\n");
#endif
}

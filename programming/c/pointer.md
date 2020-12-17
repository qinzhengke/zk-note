C语言中的pointer{#c_pointer}
==========================


\section 指针中的“+”和“-”号

指针的加号（减号）运算符得到的结果和指针的类型是有关系的，每一次+1（-1），相当于指针偏移sizeof(类型)个字节。
在memcpy这些操作的时候千万要注意。

\code{.cpp}
#include <stdint.h>
#include <stdio.h>

int main()
{
    char *p = 0;
    int16_t *p2 = 0;
    int32_t *p4 = 0;
    int64_t *p8 = 0;
    
    printf("%p,%p,%p,%p", p+1,p2+1,p4+1,p8+1);  // 运行结果: 0x1,0x2,0x4,0x8 
}

\endcode
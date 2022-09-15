# std::string


## stringstream中的“<<”和std::to_string的一个区别

对于 char 和 unsigned char类型， stringstream<<得到的结果是char，例如char a=10，输出的字符串是'\10'，而不是“10”
 
to_string的到的结果则是“10”

```cpp
#include <iostream>
#include <sstream>
using namespace std;
int main(){
char a = 33;
unsigned char b = 34;
stringstream ss;
ss<<a<<","<<b;
cout<<ss.str()<<endl;
cout<<to_string(a)<<","<<to_string(b)<<endl;
}
```
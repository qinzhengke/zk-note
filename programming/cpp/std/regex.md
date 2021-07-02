C++中的regex{#cpp_regex}
=======================

\section 基础用法

\code{cpp}
#include <string>
#include <regex>
using namespace std;

int main(){
    smatch m_idx;
    string str = "/path/to/312.bin----/819.bin";
    if(regex_search(str, m_idx, regex("/([0-9]+).(bin)"))){
        for(int i=0; i<m_idx.size(); i++){
            printf("m_idx[%d]:%s\n", i, m_idx[i].str().c_str());
        }
    }

}

\endcode

运行结果

\code
m_idx[0]:/312.bin
m_idx[1]:312
m_idx[2]:bin
\endcode

我们可以看到，首先，只匹配到了“/312.bin”这个字符串，后面的“/819.bin”并没有匹配到。
另外，有的时候，我们只想要匹配的一部分结果，例如“312”这个数字，那么只要在regex中使用括号选择想要输出的部分，
如果匹配成功，得到一个匹配数组，[0]元素就是整体匹配的结果，后面会依次输出我们选择的括号中的结果。
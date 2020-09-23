从json到struct{#cpp_json2struct}
===============================

1. https://github.com/qicosmos/iguana

基于静态反射的序列化库，非入侵式，

唯一的风险点就是依赖C++17，gcc 7.50是没有问题的。


1. https://github.com/xyz347/x2struct

只需要每个结构体后面加一句话就OK，是目前能找到使用起来最简单的方法了，还能支持必需参数和可选参数。

虽然一个变量仍然要写两边，但是
(1)如果涉及必选和可选，那无可厚非，
(2)不用以字符串形式重写，这样就能保证运行不会出错，不会出现字符串写错的问题。

唯一的问题就是该方法是入侵式的，必需在结构体的定义就要依赖x2struct。

\code{.cpp}
#include <iostream>
#include "x2struct/x2struct.hpp" // include this header file

using namespace std;

struct User {
    int64_t id;
    string  name;
    string  mail;
    User(int64_t i=0, const string& n="", const string& m=""):id(i),name(n),mail(m){}
    XTOSTRUCT(O(id, name, mail)); // add XTOSTRUCT at the end of struct/class
};

struct Group {
    string  name;
    int64_t master;
    vector<User> members;
    XTOSTRUCT(O(name, master, members)); //  add XTOSTRUCT at the end of struct/class
};

int main(int argc, char *argv[]) {
    Group g;
    g.name = "C++";
    g.master = 2019;
    g.members.resize(2);
    g.members[0] = User(1, "Jack", "jack@x2struct.com");
    g.members[1] = User(2, "Pony", "pony@x2struct.com");

    string json = x2struct::X::tojson(g);  // C++ object to json
    cout<<json<<endl;

    Group n;
    x2struct::X::loadjson(json, n, false); // json to C++ object
    cout<<n.name<<endl;

    vector<int> vi;
    x2struct::X::loadjson("[1,2,3]", vi, false); // load vector directory
    cout<<vi.size()<<','<<vi[1]<<endl;

    map<int, int> m;
    x2struct::X::loadjson("{\"1\":10, \"2\":20}", m, false); // load map directory
    cout<<m.size()<<','<<m[2]<<endl;

    return 0;
}
\endcode

作者是中国人，200+的星，还是挺厉害的。

2. https://www.codeproject.com/Articles/805182/ZetJsonCpp

文档详细，但是仍然要写挺多的重复代码，如下所示

\code{.cpp}
typedef struct{
    CParserVarInt32<_CONST_CHAR("length")>  m_length;
    CParserVarBool<_CONST_CHAR("use_space")> m_use_space;
}tIdent;

typedef struct
{
    // Default encoding for text
    CParserVarString<_CONST_CHAR("encoding")>         m_encoding;
    
    // Plug-ins loaded at start-up
    CParserVarArrayString<_CONST_CHAR("plug-ins")>      m_plugins;
        
    // Tab indent size
    CParserVarPropertyGroup<tIdent,_CONST_CHAR("indent")> m_indent;
}tSampleJson;
\endcode


3. https://www.boost.org/doc/libs/1_48_0/doc/html/boost_propertytree/tutorial.html

boost的property_tree库，手写的地方很多，没有c++语言反射，要把每个变量的名字用字符串重写一遍。

基本上就是一个json paser，可用性不高。

4. protobuf

需要从协议开始定义，因为其结构体的定义是编译前生成的，并且是“入侵式”的，也就是说你定义结构体的地方必需包含protobuf。
但是定义我们的工程本身可能也是多个模块组成，不一定想要定义结构体的地方也要包含protobuf。

5. chromium/mojo

6. 元编程实现C++结构体这段反射 https://cloud.tencent.com/developer/article/1522545
C++中的构造函数与析构函数{#cpp_constructor}
======================================


\section 容器成员变量是否会随着父类的析构而清空？

回答：是的，如下代码所示

\code{.cpp}
// swap queues
#include <iostream>       // std::cout
#include <queue>          // std::queue, std::swap(queue)
#include <memory>

using namespace std;

class A {
    public:
    ~A(){cout<<"A deconst"<<endl;}
};

class B{
public:
    B(){
        qa.push(shared_ptr<A>(new A));
        qa.push(shared_ptr<A>(new A));
    }
    ~B(){ cout<<"B deconst"<<endl; }
    queue<shared_ptr<A>> qa;
};

int main ()
{
  vector<shared_ptr<B>> q;
  q.push_back(shared_ptr<B>(new B));
  q.erase(q.begin());
  
  cout<<"-----------------"<<endl;
  
  q.push_back(shared_ptr<B>(new B));
  q.clear();

  return 0;
}
\endcode

运行结果：

\code
B deconst
A deconst
A deconst
-----------------
B deconst
A deconst
A deconst
\endcode
# C++中的构造函数与析构函数

## 容器成员变量是否会随着父类的析构而清空？

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

# 构造函数的参数隐式转换为对象

C++原来一直可以将构造函数的参数直接隐式转换成对象，如下代码所示，C++98就行。

\code{cpp}
// Example program
#include <iostream>
#include <string>

struct A{
    A(int x) : x_(x){ }
    int x_;
};

void func(A a){
    printf("%d\n", a.x_);
}

int main(){
    func(5);
    A a2 = 6;
    func(a2);
}

\endcode

运行结果：

\code
5
6
\endcode

乍一看挺神奇的，从没有这么写过。
如果不了解这种特性，那么有的时候出现问题可能一下子反应不过来。

举个例子
# std::list

## list容器的倒序访问

\code{.cpp}
// list::rbegin/rend
#include <iostream>
#include <list>

int main ()
{
  std::list<int> mylist;
  for (int i=1; i<=5; ++i) mylist.push_back(i);

  std::cout << "mylist backwards:";
  for (std::list<int>::reverse_iterator rit=mylist.rbegin(); rit!=mylist.rend(); ++rit)
    std::cout << ' ' << *rit;

  std::cout << '\n';

  return 0;
}
\endcode

输出

\code
mylist backwards:5 4 3 2 1 
\endcode


## list容器为空时的迭代结束条件
容器为空时，begin()==end()，rbegin()==rend()，所以不需要先判断是不是空，直接用rbegin()!=rend()来结束循环是没问题的，
这能够节省一些不必要的检查代码。

\code{.cpp}
// list::rbegin/rend
#include <iostream>
#include <list>

int main ()
{
  std::list<int> mylist;

  std::cout << "mylist backwards:";
  for (std::list<int>::reverse_iterator rit=mylist.rbegin(); rit!=mylist.rend(); ++rit)
    std::cout << ' ' << *rit;

  std::cout << '\n';

  return 0;
}
\endcode

输出

\code
mylist backwards:
\endcode


# list容器insert示例

\code{.cpp}
// inserting into a list
#include <iostream>
#include <list>
#include <vector>

int main ()
{
  std::list<int> mylist;
  std::list<int>::iterator it;

  // set some initial values:
  for (int i=1; i<=5; ++i) mylist.push_back(i); // 1 2 3 4 5

  it = mylist.begin();
  ++it;       // it points now to number 2           ^

  mylist.insert (it,10);                        // 1 10 2 3 4 5

  // "it" still points to number 2                      ^
  mylist.insert (it,2,20);                      // 1 10 20 20 2 3 4 5

  --it;       // it points now to the second 20            ^

  std::vector<int> myvector (2,30);
  mylist.insert (it,myvector.begin(),myvector.end());
                                                // 1 10 20 30 30 20 2 3 4 5
                                                //               ^
  std::cout << "mylist contains:";
  for (it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
\endcode


# revserse_iterator的base()函数

\code{.cpp}
// reverse_iterator::base example
#include <iostream>     // std::cout
#include <iterator>     // std::reverse_iterator
#include <vector>       // std::vector

int main () {
  std::vector<int> myvector;
  for (int i=0; i<10; i++) myvector.push_back(i);

  typedef std::vector<int>::iterator iter_type;

  std::reverse_iterator<iter_type> rev_end (myvector.begin());
  std::reverse_iterator<iter_type> rev_begin (myvector.end());

  std::cout << "myvector:";
  for (iter_type it = rev_end.base(); it != rev_begin.base(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
\endcode


# 向空的list插入元素
\code{.cpp}
// inserting into a list
#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main ()
{
  std::list<int> mylist;
  
  cout<<(mylist.rbegin()==mylist.rend())<<endl;

    for(auto it=mylist.rbegin(); it!=mylist.rend(); it++){
        
        mylist.insert (mylist.rbegin().base(),10);
        break;
    }

  for (auto it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
\endcode

输出

\code
1
\endcode
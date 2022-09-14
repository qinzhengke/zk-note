# 反射（reflection）

## 如何遍历类中的成员变量？

\code{cpp}

class A:
    def __init__(self):
        self.x = 0
        self.y = 1.0
        self.z = '2'
        
a = A()

for name, value in vars(a).items():
    print('attr, name:%s, type:%s, value:%s' % (name,type(value),value))
\endcode
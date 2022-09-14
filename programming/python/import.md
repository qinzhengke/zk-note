# import

## 如何引入其他py文件中的内容？

\code{python}
# a.py

class A:
    def __init__(self):
        self.x = 0

# b.py
sys.path.append('./xxx/xxx') # where a.py is
from a import *
print A().x
\endcode
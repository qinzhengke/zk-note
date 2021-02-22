异常（Exception）{#python_exception}
==================================

\section 如何打印更多的异常信息

使用try-except语句时，默认没有太多的调试信息，这会给代码的调试带来困难，通常我们只能知道某个函数出错了，但是该函数内容很多，问题无从查起。

为了解决这个问题，这里给出一种打印except信息的一般方法，包括抛出异常的源文件、行号、错误信息等等。

\code{python}
import linecache
import sys

def print_exception():
    exc_type, exc_obj, tb = sys.exc_info()
    f = tb.tb_frame
    lineno = tb.tb_lineno
    filename = f.f_code.co_filename
    linecache.checkcache(filename)
    line = linecache.getline(filename, lineno, f.f_globals)
    print 'EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj)

try:
    print 1/0
except:
    print_exception()
\endcode
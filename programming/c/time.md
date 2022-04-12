标准库time.h{#c_time}
====================

# 几个概念

- time_t：time since epoch，是一个抽象的时间，它究竟是怎么存储的，C语言标准并没有对其进行定义。虽然大部分编译器实现都是用一个整型变量存储秒数，从UTC epoch开始。
- tm：是一个具体表达日历时间的结构体，它包含了年月日时分秒。
- UNIX time：UNIX系统时间，它的epoch和UTC是相同的，但是并没有考虑闰秒，所以，UNIX时间和UTC时间是不相同的。
- tm中的“daylight saving time”：也就是夏令时，一种为了节约照明电力专门设计的时间，每年春季时钟拨快1小时，秋季回调1小时。

# tm_make_time tm类型与make_time()函数

tm是一个结构体，包含了日历时间所需要的所有分成：年、月、日、时、分、秒

\htmlonly
<table class="boxed">
<tbody><tr><th>Member</th><th>Type</th><th>Meaning</th><th>Range</th></tr>
<tr><td><samp>tm_sec</samp></td><td><code>int</code></td><td>seconds after the minute</td><td><code>0-60*</code></td></tr>
<tr><td><samp>tm_min</samp></td><td><code>int</code></td><td>minutes after the hour</td><td><code>0-59</code></td></tr>
<tr><td><samp>tm_hour</samp></td><td><code>int</code></td><td>hours since midnight</td><td><code>0-23</code></td></tr>
<tr><td><samp>tm_mday</samp></td><td><code>int</code></td><td>day of the month</td><td><code>1-31</code></td></tr>
<tr><td><samp>tm_mon</samp></td><td><code>int</code></td><td>months since January</td><td><code>0-11</code></td></tr>
<tr><td><samp>tm_year</samp></td><td><code>int</code></td><td>years since 1900</td><td><code> </code></td></tr>
<tr><td><samp>tm_wday</samp></td><td><code>int</code></td><td>days since Sunday</td><td><code>0-6</code></td></tr>
<tr><td><samp>tm_yday</samp></td><td><code>int</code></td><td>days since January 1</td><td><code>0-365</code></td></tr>
<tr><td><samp>tm_isdst</samp></td><td><code>int</code></td><td>Daylight Saving Time flag</td><td><code> </code></td></tr>
</tbody></table>
\endhtmlonly

# 日期加上秒数得到新的日期

\code{cpp}
#include <ctime>
#include <cstdio>
int main(){
    time_t epoch = 0;
    time_t t1 = epoch + 1606785706;
    struct tm *ptm = gmtime(&t1);
    printf("date:%d,%d,%d\n", ptm->tm_year, ptm->tm_mon, ptm->tm_mday);
}

\endcode

输出为：

\code{bash}
date:120,11,1
\endcode

注意，tm.tm_year以1900年为起点，也就是公元2020年，1606785706这个秒数本身是从1970年1月1日0时0分0秒开始计算的。
另外，tm.mon的编码区间是[0,11]，所以tm.mon=11表示的是12月。
最后，这个日期和时间，实际上是Unix时间，它和UTC共用一个epoch，但是并没有考虑闰秒，而UTC是考虑了闰秒的。


# 计算两个日期之间的秒数

\code{cpp}
/* time example */
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

int main ()
{
  time_t timer;
  struct tm y2k = {0};
  double seconds;

  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

  time(&timer);  /* get current time; same as: timer = time(NULL)  */

  seconds = difftime(timer,mktime(&y2k));

  printf ("%.f seconds since January 1, 2000 in the current timezone", seconds);

  return 0;
}
\endcode
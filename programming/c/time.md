标准库time.h{#c_time}
====================

\section tm_make_time tm类型与make_time()函数

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

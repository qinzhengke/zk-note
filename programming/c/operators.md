# C语言中的运算符

# 运算符优先级表格


## 运算符优先级cheatsheet

\htmlonly
<table class="wikitable" border="1"> 

<tbody><tr>
<th style="text-align: left"> 优先级
</th>
<th style="text-align: left"> 运算符
</th>
<th style="text-align: left"> 描述
</th>
<th style="text-align: left"> 结合性
</th></tr>
<tr>
<th rowspan="6"> 1
</th>
<td style="border-bottom-style: none"> <code>++</code> <code>--</code>
</td>
<td style="border-bottom-style: none"> 后缀自增与自减
</td>
<td style="vertical-align: top" rowspan="6"> 从左到右
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>()</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 函数调用
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>[]</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 数组下标
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>.</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 结构体与联合体成员访问
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>-&gt;</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 结构体与联合体成员通过指针访问
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>(<i>type</i>){<i>list</i>}</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 复合字面量<span class="t-mark">(C99)</span>
</td></tr>
<tr>
<th rowspan="8"> 2
</th>
<td style="border-bottom-style: none"> <code>++</code> <code>--</code>
</td>
<td style="border-bottom-style: none"> 前缀自增与自减<sup id="cite_ref-1" class="reference"><a href="#cite_note-1">[注 1]</a></sup>
</td>
<td style="vertical-align: top" rowspan="8"> 从右到左
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>+</code> <code>-</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 一元加与减
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>!</code> <code>~</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 逻辑非与逐位非
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>(<i>type</i>)</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 类型转型
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>*</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 间接（解引用）
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>&amp;</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 取址
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>sizeof</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 取大小<sup id="cite_ref-2" class="reference"><a href="#cite_note-2">[注 2]</a></sup>
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>_Alignof</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 对齐要求<span class="t-mark">(C11)</span>
</td></tr>
<tr>
<th> 3
</th>
<td> <code>*</code> <code>/</code> <code>%</code>
</td>
<td> 乘法、除法及余数
</td>
<td style="vertical-align: top" rowspan="11"> 从左到右
</td></tr>
<tr>
<th> 4
</th>
<td> <code>+</code> <code>-</code>
</td>
<td> 加法及减法
</td></tr>
<tr>
<th> 5
</th>
<td> <code>&lt;&lt;</code> <code>&gt;&gt;</code>
</td>
<td> 逐位左移及右移
</td></tr>
<tr>
<th rowspan="2"> 6
</th>
<td style="border-bottom-style: none"> <code>&lt;</code> <code>&lt;=</code>
</td>
<td style="border-bottom-style: none"> 分别为 &lt; 与 ≤ 的关系运算符
</td></tr>
<tr>
<td style="border-top-style: none"> <code>&gt;</code> <code>&gt;=</code>
</td>
<td style="border-top-style: none"> 分别为 &gt; 与 ≥ 的关系运算符
</td></tr>
<tr>
<th> 7
</th>
<td> <code>==</code> <code>!=</code>
</td>
<td> 分别为 = 与 ≠ 关系
</td></tr>
<tr>
<th> 8
</th>
<td> <code>&amp;</code>
</td>
<td> 逐位与
</td></tr>
<tr>
<th> 9
</th>
<td> <code>^</code>
</td>
<td> 逐位异或（排除或）
</td></tr>
<tr>
<th> 10
</th>
<td> <code>|</code>
</td>
<td> 逐位或（包含或）
</td></tr>
<tr>
<th> 11
</th>
<td> <code>&amp;&amp;</code>
</td>
<td> 逻辑与
</td></tr>
<tr>
<th> 12
</th>
<td> <code>||</code>
</td>
<td> 逻辑或
</td></tr>
<tr>
<th> 13
</th>
<td> <code>?:</code>
</td>
<td> 三元条件<sup id="cite_ref-3" class="reference"><a href="#cite_note-3">[注 3]</a></sup>
</td>
<td style="vertical-align: top" rowspan="6"> 从右到左
</td></tr>
<tr>
<th rowspan="5"> 14<sup id="cite_ref-4" class="reference"><a href="#cite_note-4">[注 4]</a></sup>
</th>
<td style="border-bottom-style: none"> <code>=</code>
</td>
<td style="border-bottom-style: none"> 简单赋值
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>+=</code> <code>-=</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 以和及差赋值
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>*=</code> <code>/=</code> <code>%=</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 以积、商及余数赋值
</td></tr>
<tr>
<td style="border-bottom-style: none; border-top-style: none"> <code>&lt;&lt;=</code> <code>&gt;&gt;=</code>
</td>
<td style="border-bottom-style: none; border-top-style: none"> 以逐位左移及右移赋值
</td></tr>
<tr>
<td style="border-top-style: none"> <code>&amp;=</code> <code>^=</code> <code>|=</code>
</td>
<td style="border-top-style: none"> 以逐位与、异或及或赋值
</td></tr>
<tr>
<th> 15
</th>
<td> <code>,</code>
</td>
<td> 逗号
</td>
<td> 从左到右
</td></tr></tbody></table>

\endhtmlonly
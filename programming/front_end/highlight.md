# highlight.js：一个代码高亮库

## 前言

这里的代码高亮不是指软件开发时编辑器或者IDE的语法高亮，而是写文档时，把代码显示到html文档中能够高亮阅览。

使用Doxygen写文档时，发现Doxygen对语言高亮的支持很有限，只有C++能比较正常的显示出来，其他的语言例如Python，Javascript，json，cmake等等都没法高亮。
所以需要一种代码高亮工具，能够嵌入Doxygen。

其实能够嵌入Doxygen的要求很低，只要能在hmtl中使用，那么在就可以在Doxygen的\htmlonly宏中使用。

## 引入highlight.js

highlight.js是一个好用的代码高亮工具，地址：https://highlightjs.org/，使用方法，

1. 根据网页提示，下载highlight.js。

2. 根据以下方式来使用


<script src="highlight/highlight.pack.js"></script>
<link rel="stylesheet" href="highlight/styles/github.css">
<script>hljs.highlightAll();</script>
<pre><code class="html">
&ltscript src="highlight/highlight.pack.js"&gt&lt/script&gt
&ltlink rel="stylesheet" href="highlight/styles/github.css"&gt
&ltscript&gthljs.highlightall();&lt/script&gt
&ltpre&gt&ltcode class="python"&gt
...
&lt/code&gt&lt/pre&gt
&lt/code&gt
</code></pre>
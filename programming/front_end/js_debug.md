Javascript调试常见问题{#js_debug}
===============================


# 如何打印输出？
console.log()，在浏览器调试界面（chrome中按F2），选择console选项卡可以看到日志输出．


# 动态html内容如何查看？
在浏览器调试界面中，选择Elements，上面的内容即为当前的html内容，包括使用javascript动态生成的．

与之相比，Sources标签页展示的只是纯粹的源代码．


# 我的动态html元素为什么没有出现？

一个常见的原因是js运行出错，导致js退出执行，所以要好好看控制台的输出．


# button的onclick无效？

一种原因是写html标的时候，函数没有写上括号．

\code{.js}
<button onclick="my_function"> Button </button>     // 错误
<button onclick="my_function()"> Button </button>     // 正确
\endcode

这里实际上有解释器的问题的，因为没加()的时候，解释器确实识别到了响应函数，但是并没有执行．
不信可以更改my_function为一个不存在的函数，控制台就会报错．
这就导致开发者认为函数没有问题，往别的方向思考．


# javascript字符串替换

如下代码所示，注意，第一个参数不是忘了加引号，确实是没有引号的，特别神奇的写法．
下面代码的意思是替换所有的换行符号为空．
/g和vim里面的一样，就是替换所有．

\code{.js}
    str.replace(/\n/g, "");
\endcode


# 构造对象

\code{.js}
function Persion(name, age){
    this.name = name;
    this.age = age;
}

var persion1 = new Persion("Bob", 13);

\endcode

注意，this后面用的是"."符号，并且一定要用new．
写惯C++后，有可能会出错．


# 使用$("#id")代替document.getElement

明显$("#id")要简洁很多，但是要注意，原来的value要变成val()
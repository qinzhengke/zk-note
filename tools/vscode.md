Visual Studio Code{#vscode}
================

\section 如何添加宏定义？
step 1: ctrl+shift+p，选择"C/C++ Edit configurations..."，或者直接“ctrl+p”，输入“c_cpp_properties.json”，来到，C/C++配置页面。
step 2: 在"configurations"项目下的"defines"插入宏定义即可。

举例：
```{.json}
{
  "configurations": [
    {
      "name": "Win32",
      "defines": ["_DEBUG", "UNICODE", "_UNICODE"],
      "compilerPath": "C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64/cl.exe",
      "windowsSdkVersion": "10.0.17763.0",
      "intelliSenseMode": "msvc-x64",
      "cStandard": "c11",
      "cppStandard": "c++17"
    }
  ],
  "version": 4
}
```

\section 如何使用clang-format进行代码格式化？

步骤1：`File`->`Preference`->`Settings`->搜索“format”，选择“Editor:Format On Save”。

步骤2：同样在`Settings`界面，搜索“C_Cpp”，在“C_Cpp:Clang_format_style”下方的编辑框中填入“file”，即使用工作空间的“.clang-format”文件作为style，工作空间就是vscode打开的目录，例如打开目录的时候使用“code .”，那么工作空间就是“.”，记住不是.vscode目录！

\section markdown代码片

1. “File”->“Preference”->“User snippets”，然后在下拉列表中选择“markdown.json”

2. 按照模板来编辑，简单的例子如下：

3. markdown格式是默认没有打开intellsense功能的，需要手动打开，在“ctrl+p”->“输入>”->“输入language”->“选择markdown”->“edit in settings.json”
按照如下方式添加：

~~~{.json}
{
    "cmake.configureOnOpen": true,
    "C_Cpp.updateChannel": "Insiders",
    "http.proxySupport": "off",
    "[markdown]": {
      "editor.quickSuggestions":true
    }
}
~~~

\section 代码片的设置语法

~~~{.json}
	// Place your snippets for markdown here. Each snippet is defined under a snippet name and has a prefix, body and 
	// description. The prefix is what is used to trigger the snippet and the body will be expanded and inserted. Possible variables are:
	// $1, $2 for tab stops, $0 for the final cursor position, and ${1:label}, ${2:another} for placeholders. Placeholders with the 
	// same ids are connected.
	// Example:
	// "Print to console": {
	// 	"prefix": "log",
	// 	"body": [
	// 		"console.log('$1');",
	// 		"$2"
	// 	],
	// 	"description": "Log output to console"
	// }
	"Inline function": {
		"prefix": "if",
		"body": [
			"\\f$ $0 \\f$"
		],
		"description": "inline formula"
  },
~~~

其实按照前面的注释非常容易看懂，$1和$2是tab跳转位置，代码片出来后，光标会自动跳转到$1位置，然后输入字符，按tab键跳到$2。
问题是，光标$1和$2的编辑模式很奇怪，仍然处于当前代码片编辑模式下，这种模式下无法嵌套地触发其他代码片。

然而我们通常要在光标处触发其他代码片，这就需要使用到$0，它表示，光标最终的位置，即如果没有$x，代码片自动输出时就退出了代码片模式，自然就可以触发其他的代码片。

\section Latex常用的代码片

参见 \subpage vscode_markdown_snippet
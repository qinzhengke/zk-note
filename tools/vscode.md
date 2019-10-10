Visual Studio Code{#vs_code}
================


## C/C++篇

### 如何添加宏定义？
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

### 如何使用clang-format进行代码格式化？
步骤1：`File`->`Preference`->`Settings`->搜索“format”，选择“Editor:Format On Save”。
步骤2：同样在`Settings`界面，搜索“C_Cpp”，在“C_Cpp:Clang_format_style”下方的编辑框中填入“file”，即使用工作空间的“.clang-format”文件作为style，工作空间就是vscode打开的目录，例如打开目录的时候使用“code .”，那么工作空间就是“.”，记住不是.vscode目录！

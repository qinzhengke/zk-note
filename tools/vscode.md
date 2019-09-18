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

C++文件IO{#cpp_file_io}
======================

\section cpp_binary_file C++中的二进制文件读取

这一小节研究一下c++对二进制文件读写的用法。

首先定义两个结构体A和B，如下代码所示，其中“#pragma”语句是保证数据存储时1字节对齐，编译器一般默认4字节对齐，例如一个结构体按照变量计算得61个字节，那么编译器会在存储这个结构体时补上3个字节，保证4字节对齐。字节对齐虽然能够优化存储空间，但是在读写文件的时候会造成麻烦，特别是读取端代码不知道结构体形式，而只是知道变量列表的时候，读文件顺序会出错。

```c++
#include <iostream>
#include <fstream>
using namespace std;

#pragma pack(1)
typedef struct _A
{
    int index,time_stamp;
} A;

typedef struct _B
{
   float data1,data2,data3;
}B;
#pragma pack()

```

然后是写文件部分，其实写文件很简单，一般不考虑写的位置和文件尾什么的，下面代码展示了存储两个结构体，分别是A和B，构成一个数据块。通过调整N可以增加数据块的个数，但是目前我们只用一个数据块

```c++
int write_file_01()
{
    const int N = 1;
    A a[N];
    B b[N];

    for(int i=0; i<N; i++)
    {
        a[i].index = i;
        a[i].time_stamp = i*2;
        b[i].data1 = i;
        b[i].data1 = i*i;
        b[i].data3 = i*i*i;
    }

    ofstream out_file;
    out_file.open("file_01",ios::binary);
    for(int i=0; i<N; i++)
    {
        out_file.write((char*)(&(a[i])), sizeof(A));
        out_file.write((char*)(&(b[i])), sizeof(B));
    }
    out_file.close();

    return 1;
}
```

读取文件就比较麻烦了，特别是使用不完全读取时，特别是在使用seekg函数时。我们看一下代码，我们一般使用eof()函数来检测是否读取到文件末尾，但是，我们有时候不想读取所有内容，而只是其中一部分。例如数据由Header部分和Data部分组成串联在一起，Header描述了Data部分的内部结构，即”HDHDHDHD......“的形式。我每次只想读取H部分，因为D部分太长了，我只能使用seekg()函数略过。

一开始很自然得想到如下的代码，用read()函数读取H部分，然后用seekg()函数略过D部分，知道走到文件的末尾。然而，这段代码运行的时候其实是无限循环，如图所示！

```c++
int read_file_01() // 错误的使用方式！
{
    A aa;
    B bb;
    ifstream in_file;
    in_file.open("file_01", ios::binary);
    while(!in_file.eof())
    {
        in_file.read((char*)(&aa), sizeof(A));
        cout<<"aa.index="<<aa.index<<" aa.time_stamp="<<aa.time_stamp<<endl;
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
        in_file.seekg(sizeof(B),ios::cur);
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
    }
    in_file.close();
    return 1;
}
```

![](files/read_binary_file_error_01.png)

我们可以看到，在seekg()函数将文件指针走到文件末尾时，再用read()函数，这时候eof就会置位，但是此时再seekg()之后，eof又会变成0了！这感觉不太合常理，只用eof不太可靠，事实上，cplusplus网站给出了比较合理的用法：

> Reaching the *End-of-File* sets the eofbit. But note that operations that reach the *End-of-File* may also set the failbit if this makes them fail (thus setting both eofbit and failbit).

我们可以这样修改读取函数：

```c++
int read_file_01_correct()//不太完美的读取方法
{
    A aa;
    B bb;
    ifstream in_file;
    in_file.open("file_01", ios::binary);
    while(!in_file.eof() && !in_file.fail())
    {
        in_file.read((char*)(&aa), sizeof(A));
        cout<<"aa.index="<<aa.index<<" aa.time_stamp="<<aa.time_stamp<<endl;
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
        in_file.seekg(sizeof(B),ios::cur);
        cout<<"eofbit = "<<in_file.eof() << ", failbit = "<<in_file.fail()<<", badbit = "<<in_file.bad()<< endl;
    }
    in_file.close();
    return 1;
}
```



其运行结果如下，程序不再是无限循环了，但是这也并不完美，因为事实上我们只期待一次循环，而这里多出了一次无用的循环，问题在于seekg()函数与read()函数不同，当执行seekg()函数走过文件的末尾时，它并不会将eofbit置1！所以只有等下次循环read()失败时，才能得到结束循环的条件。多了一次无用循环即不好看也有隐患，谁知道在那个无用循环中程序会做什么出格的事情呢？seekg()函数第一次超出文件末尾不将eof置位，第二次超出文件末尾时反而将eof从1置0，感觉seekg函数该做的不做，不该做的却反倒做了。

![](files/read_binary_file_error_02.png)

我目前在使用的最稳定可靠的方案是构建一个check函数，这个函数里面执行read命令，查看文件指针是否越界，然后再将因为执行read函数而位移的指针拨会即可，如下代码所示。这种方式需要新建一个函数，而且不是最漂亮的，但是这是我目前认为最可靠的一个方案，毕竟软件首要的指标是可靠！

```c++
inline bool check_eof(ifstream & file)
{
    char bit;
    bool s = false;
    file.read(&bit, sizeof(char));
    if(file.eof() || file.fail())
        s = true;
    file.seekg(-1, ios::cur);
    return s;
}

int read_file_01_perfect() // 完美的读取方案
{
    A aa;
    ifstream in_file;
    in_file.open("file_01", ios::binary);
    while(!check_eof(in_file))
    {
        in_file.read((char*)(&aa), sizeof(A));
        cout<<"aa.index="<<aa.index<<" aa.time_stamp="<<aa.time_stamp<<endl;
        cout<<"e, f, b = "<<in_file.eof()<<", "<<in_file.fail()<<", "<<in_file.bad()<<endl;
        in_file.seekg(sizeof(B), ios::cur);
        cout<<"e, f, b = "<<in_file.eof()<<", "<<in_file.fail()<<", "<<in_file.bad()<<endl;
    }
    in_file.close();
    return 1;
}
```

运行结果如下图所示，只有一个循环，很完美。

![](files/read_binary_file_error_03.png)

main函数里面的代码由于实验需要进行部分屏蔽。

```c++
int main(int argc, char *argv[])
{
    write_file_01();
//    read_file_01_correct();
    read_file_01_perfect();
    //write_file_02();
    //read_file_02();
    return 0;
}
```
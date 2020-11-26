Rust：超严格静态类型语言（精神小伙）{#rust}
===========


\section 文档

书籍：The Rust Programming Language，官方学习资料，必看。
来源：
- 在线版本，https://doc.rust-lang.org/book/title-page.html
- 离线版本，rustup docs --book

<hr>
\section playground

rust官网提供了一个在线编译运行rust代码的工具，即playground，可以不用下载rust到本地，直接在网页端运行rust代码，网站地址为：https://play.rust-lang.org/。
对于一些小的代码验证，特别的有用。
C++也早有类似的网站，例如cpp.sh

<hr>
\section rust安全性解决的问题

- Memory leak
- Double free
- Data race

<hr>
\section mut

rust非常强调默认immutable的概念，如果定义了一个mut变量，但是没有去mut它，那么编译器会提出警告，提示没有必要定义mut，如下代码所示：

\code{rust}
fn main(){
    let mut x = 10;
    println!("x = {}", x);
}
\endcode

编译结果：

\code{bash}
   Compiling playground v0.0.1 (/playground)
warning: variable does not need to be mutable
 --> src/main.rs:2:9
  |
2 |     let mut x = 10;
  |         ----^
  |         |
  |         help: remove this `mut`
  |
  = note: `#[warn(unused_mut)]` on by default

warning: 1 warning emitted

    Finished dev [unoptimized + debuginfo] target(s) in 0.51s
     Running `target/debug/playground`
\endcode

<hr>
\section showdow

rust中的变量可以中途被“替换”，例如本来变量a是一个i32，但是可以重写a为String，这种特性叫做“shadow”。

shadow存在的意义是避免一些类型转换过程中多次命名，让代码命名看起来更简短清晰，例如“a_int”，“a_float”以及“a_str”都可以用“a”代替。

这是官方给出的说法。

> Shadowing thus spares us from having to come up with different names, such as spaces_str and spaces_num; instead, we can reuse the simpler spaces name.

shadowu有一个小问题，和C++中的override一样，如果出现了拼写错误，没有shadow成功，是不会报错的，有可能导致逻辑不符合预期，增加调试时间。

不知道rust的shadow有没有类似C++的override关键字。

<hr>
\section 编译期间的溢出检查

rust编译器很强，能够在编译期间检查出是否存在类型溢出，有意思的是，这种检查是在语法检查之后进行的，如下代码所示

\code{rust}
fn main(){
    let mut x: u8 = 255;
    x = x+1;    // 类型溢出
    println!("x={}",x);
    let y = 10;
    y = 11;     // 一处语法错误
    println!("y={}",y);
}
\endcode

我们故意在类型溢出后面加入一段无关的语法错误，编译结果首先报出了语法错误，说明编译器是先检查语法错误的，并且发现错误后直接停止了编译。

\code{bash}
   Compiling playground v0.0.1 (/playground)
warning: value assigned to `y` is never read
 --> src/main.rs:5:9
  |
5 |     let y = 10;
  |         ^
  |
  = note: `#[warn(unused_assignments)]` on by default
  = help: maybe it is overwritten before being read?

error[E0384]: cannot assign twice to immutable variable `y`
 --> src/main.rs:6:5
  |
5 |     let y = 10;
  |         -
  |         |
  |         first assignment to `y`
  |         help: make this binding mutable: `mut y`
6 |     y = 11;
  |     ^^^^^^ cannot assign twice to immutable variable

error: aborting due to previous error; 1 warning emitted

For more information about this error, try `rustc --explain E0384`.
error: could not compile `playground`

To learn more, run the command again with --verbose.
\endcode

如果我们把语法错误代码注释掉，如下所示：

\code{rust}
fn main(){
    let mut x: u8 = 255;
    x = x+1;
    println!("x={}",x);
    // let y = 10;
    // y = 11;
    // println!("y={}",y);
}
\endcode

那么运行结果为：

\code{bash}
   Compiling playground v0.0.1 (/playground)
    Finished dev [unoptimized + debuginfo] target(s) in 0.50s
     Running `target/debug/playground`
thread 'main' panicked at 'attempt to add with overflow', src/main.rs:3:9
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace
\endcode

<hr>
\section 整型字面量无法给浮点变量赋值

rust的类型检查真的是非常严格，整型的字面常量都无法给浮点变量赋值，如下代码所示：

\code{rust}
fn main(){
    let x: [f32;5] = [1,2,3,4,5];
    println!("x={:?}", x);
}
\endcode

编译结果：

\code{bash}
   Compiling playground v0.0.1 (/playground)
error[E0308]: mismatched types
 --> src/main.rs:2:23
  |
2 |     let x: [f32;5] = [1,2,3,4,5];
  |                       ^
  |                       |
  |                       expected `f32`, found integer
  |                       help: use a float literal: `1.0`

error: aborting due to previous error

For more information about this error, try `rustc --explain E0308`.
error: could not compile `playground`

To learn more, run the command again with --verbose.
\endcode

<hr>
\section ownership三个原则

- Each value in Rust has a variable that's called its owner.
- There can be only one owner at a time.
- When the owner goes out of scope, the value will be dropped.

翻译过来就是：

- 在Rust中，每一个“值”都会对应一个“拥有者”
- 同一时间只能有一个“拥有者”
- 当“拥有者”离开了可视范围，“值”就会被抛下。
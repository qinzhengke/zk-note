std::queue{#cpp_queue}
======================

<hr>
\section std::queue不支持clear()操作

这很奇怪，原因待调查，清空队列的几种方式是

\code{.cpp}
// 方法一
queue<int> q1;
// process
// ...
q1 = queue<int>();

// 方法二
while (!Q.empty()) Q.pop();

// 方法三
void clear(queue<int>& q) {
	queue<int> empty;
	swap(empty, q);
}
\endcode
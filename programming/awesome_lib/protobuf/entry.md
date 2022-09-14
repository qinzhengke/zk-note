# Protobuf

## 常见问题

### multi_proto_input 如何输入多个proto文件？

\code
protoc --cpp_out=../../protoc --proto_path=./ *.proto
\endcode

### 如何读写将基于protobuf描述的streaming文件？

protobuf本身没有定义数据的边界，如果想要把多块protobuf数据存到同一个文件，则用户必须自行定义一种文件格式，描述每一个protobuf数据的起点和长度。

参见protobuf官方文档：

\code
Streaming Multiple Messages

If you want to write multiple messages to a single file or stream, it is up to you to keep track of where one message ends and the next begins. The Protocol Buffer wire format is not self-delimiting, so protocol buffer parsers cannot determine where a message ends on their own. The easiest way to solve this problem is to write the size of each message before you write the message itself. When you read the messages back in, you read the size, then read the bytes into a separate buffer, then parse from that buffer. (If you want to avoid copying bytes to a separate buffer, check out the CodedInputStream class (in both C++ and Java) which can be told to limit reads to a certain number of bytes.)
\endcode

> https://developers.google.com/protocol-buffers/docs/techniques

基于protobuf的streaming类型文件的相关设计有一个专门的词汇来描述：“length-delimiting messages”，或者“delimited streaming format”

> https://medium.com/@seb.nyberg/length-delimited-protobuf-streams-a39ebc4a4565

## 如何在cmake中自动执行protoc生成pb.h和pb.cc

\code{bash}
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER message.proto)
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})
\endcode
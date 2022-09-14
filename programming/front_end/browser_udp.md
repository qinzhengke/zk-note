# 浏览器里调用udp接口

经过一番调研，发现浏览器貌似是不能调用udp接口的，甚至不能调用tcp接口。

网络上有很多人尝试和提问，回答都是不行，

例如： https://stackoverflow.com/questions/40307161/browser-as-udp-dgram-client

提问者问为什么dgram在node运行模式可以成功调用udp接口，但是挪到了浏览器就不行。

票数最高的回答则是浏览器不能建立UDP链接，甚至TCP连接都不行，只能建立HTTP连接。

回答者还给出了2种方案：

1. 建立浏览器插件中转
2. 建立代理进行中转。

这两种方案都并不好

再例如 https://www.tutorialfor.com/questions-6798.htm

这哥们做了很多的尝试，对我们非常有指导意义，很有意思，这里截取出来

\code
We are currently investigating whether an SNMP search for printer devices can be performed from a Web browser.
Premise: A web browser (Chrome for the time being) in a clean PC (no OS required).
What I want to do: I want to do UDP communication using only client-side JavaScript.

What i am trying to do is:
1. Install net-snmp (https://www.npmjs.com/package/net-snmp) from npm of Node.js
2. Install browserify (https://www.npmjs.com/package/browserify) from npm in Node.js
3. Convert sample code of net-snmp with browserify
4). Check with a web browser

4 failed when executing dgram.createsocket is not a function error on the client side.

As a result of various investigations, dgram cannot be browsed because dgram is processed inside Node.js,
In general, it is impossible to communicate on the client side other than WebSocket, XHR, and WebRTC.
I understand that UDP socket communication has been retired as a draft in W3C.
(https://www.w3.org/standards/history/tcp-udp-sockets)

Try it
Try replacing A, dgram with chrome-dgram (https://www.npmjs.com/package/chrome-dgram)
Since dgram cannot be used on the client side, I replaced the corresponding part with chrome-dgram and verified it.
SNMP search can be done with this method, but to install and run on the Chrome application
It is a little different from what I want to achieve.

Try replacing B, dgram with dgram-browserify (https://www.npmjs.com/package/dgram-browserify)
This tried replacing dgram with another package dgram-browserify.
As a result, dgram-browserify is created using WebSocket instead of dgram.createsocket
SNMP search could not be performed because the UDP port was not used for transmission.

Try replacing with C, mock-dgram (https://www.npmjs.com/package/mock-dgram)
This is because the data on the PC was exchanged by making it look like UDP communication
UDP communication was not actually performed.

SNMP search from an external program using D, NaCl + PPAPI
We have confirmed that this is also possible, but prepare an external program in addition to the Web browser as in the case A
It is a little different from what I want to achieve because it is necessary.

Because I am a beginner of JavaScript, I don't know much about it, but for me, WebRTC, DNS, etc. from a web browser
Even file descriptors that can communicate with UDP because UDP data is exchanged
Isn't it possible to communicate from the UDP port if you can make it with JavaScript? I think.

Are there any other means of UDP communication (SNMP search) from a web browser than the above means?
that's all, thank you very much.
\endcode

总结一下，提问者尝试了

1. chrome app方法，虽然成功，但是需要创建chrome app，用户还需要下载一个插件，和web应用的精神不符，（要下载插件，那为什么不直接做一个本地应用呢，反正用户都得下载）。

2. 使用dgram-browserify，这个库调用了websocket代替dgram.createsocket，根本就没有UDP协议。

3. 使用mock-dgram，说这是一个假的UDP接口，根本没有真正的发送UDP包，所以叫做mock，

4. 使用外部程序，虽然成功，但是和1一样，需要创建另一个app，失去了webUI的意义。

从作者的多次尝试可以看出里面的心路历程。

# 浏览器不能发送udp包的原因

这篇文章给出了原因：https://gafferongames.com/post/why_cant_i_send_udp_packets_from_a_browser/

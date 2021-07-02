Google Chrome{#google_chrome}
=============================

\section 如何以不同的profile来运行chrome？

有的时候，我们希望使用不同的配置来运行chrome，例如常规的浏览模式，不希望带有账户的本地模式，以及为了某个应用（区块链项目网页版）独立而独立运行的模式。

chrome原生支持以不同的profile运行，方式如下：

```
google-chrome-stable --profile-directory="my_profile"
```

不同的模式下，安装的插件、书签都是独立的。
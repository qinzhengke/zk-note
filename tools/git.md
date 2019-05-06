版本管理工具：git{#git}
=========

## permission too open

```shell
chmod 700 ./private_key_file
```


## Change commit editor
```shell
git config --global core.editor "vim"
```

## 设置本地分支track远程分支
```shell
# checkout local branch
git checkout my_branch
# setup the tracked remote branch.
git branch --set-upstream-to=origin/my_branch
#或者
git branch -u origin/my_branch
```

## git加载私钥

使用ssh生成密钥对之后，将公钥添加至服务器，

ssh不会自动找到私钥，即使将私钥放在~/.ssh文件夹下，私钥需要添加至ssh

使用ssh-add命令添加，

```shell
ssh-add ~/.ssh/zk_rsa
```

如果出现了cannot connect to agent 提示，则执行**ssh-agent bash命令**，即

```shell
ssh-agent bash
```

## fetch和pull的区别

fetch命令只是获取远程代码，并不会直接影响到本地代码，自然也不会出现冲突。pull命令实际是可以认为是fetch+merge，会加载到本地代码，并且尝试去合并。在smartgit中，仓库fetch之后会有一个向下的绿色箭头图标，pull之后就会消失。

## git 查看某个文件修改历史

突然有一天，发现自己要用的某个源文件中被修改了！想查看到底是谁修改的，但是手动浏览太慢了，特别是主干提交非常多的情况。使用gitk命令可以直接查看某个文件的修改历史。gitk是一个gui形式的git浏览工具，貌似是和git同时提供的，很强大。

```shell
# syntax: gitk -- <file_name>
gitk -- /some_directory/my_interest.c
```

## git报错：remote rejected *** change *** closed

在push到gerrit时出现了这个错误，在overflow查到了原因，我提交的change id和历史提交中的某个id重复了，需要删除change id，然后重新生成（删除后重新打开commit message即可），再Push就没有问题了。

## git创建分支后记得尽快commit：

创建并且切换分支之后，修改了一些代码，如果没有commit，再切回原来的分支，那么这些修改也会影响原来的分支。所以切换回原来的分支之前要commit一下。

## 推送到远程分支
git push origin HEAD:tmp/xxx

虽然推送的标记是只有HEAD，但是实际上会把<=HEAD的节点都给推上去。

## 删除远程分支
git push origin --delete tmp/xxx

## untracked files.
使用了`git reset --hard`或者`git stash --keep-index`，仍然无法去掉untracked files？
尝试使用`git clean -d -f`

## reset 某一个文件
~~~{bash}
git checkout HEAD -- my-file.txt
~~~

# 版本管理工具：git

## permission too open

```bash
chmod 700 ./private_key_file
```

## Change commit editor
```bash
git config --global core.editor "vim"
```

## 设置本地分支track远程分支
```bash
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

```bash
ssh-add ~/.ssh/zk_rsa
```

如果出现了cannot connect to agent 提示，则执行**ssh-agent bash命令**，即

```bash
ssh-agent bash
```

## fetch和pull的区别

fetch命令只是获取远程代码，并不会直接影响到本地代码，自然也不会出现冲突。pull命令实际是可以认为是fetch+merge，会加载到本地代码，并且尝试去合并。在smartgit中，仓库fetch之后会有一个向下的绿色箭头图标，pull之后就会消失。

## git 查看某个文件修改历史

突然有一天，发现自己要用的某个源文件中被修改了！想查看到底是谁修改的，但是手动浏览太慢了，特别是主干提交非常多的情况。使用gitk命令可以直接查看某个文件的修改历史。gitk是一个gui形式的git浏览工具，貌似是和git同时提供的，很强大。

```bash
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

## reset 某一个文件到某个commit
```bash
git checkout HEAD -- my-file.txt
```

## repo sync
repo sync是不会删除已有的git的，即如果把repo的manifest.xml里面有某个git给删除掉后，运行reposync，该git还是会存在的。
repo sync会生成git但是不会删除git，这就会导致来回切repo的时候，遗留的git会越来越多，有可能导致重定义，需要手动删除。

## cherry-pick一个merge commit
merge类型的commit是无法直接被cherry-pick的，因为它有两个父节点。cherry-pick的时候需要特殊处理
```bash
git cherry-pick XXX -m 1
```
这里的数字“1”需要使用者指定，通过git log可以看到merge commit的两个父节点的commit id，两个父节点的顺序就和-m参数后面跟的顺序是一样的。

## 使用rebase进行分支合并
我们通常会建立topic分支进行开发，一开始topic分支和develo是合在一起的，但是由于在开发topic过程中，有别人向develop推送了新的commit，这时topic和develop产生了分歧。topic开发完毕的时候，必须要叠在develop分支上才能进行提交，否则很有可能出现merge conflict。
笨办法：checkout到develop分支，然后cherry-pick topic分支（假设topic分支已经合成一个commit）,然后以develop的身份推送到remote。这种办法会让develop分支发生变动，而且topic分支还需要手动reset到develop分支。
好办法：使用rebase进行合并，站在topic分支直接`git rebase develop`即可。

## 对某个文件撤销改动
对某个文件撤销改动是常常用到的feature。
笨办法：`git reset --mixed HEAD^`,然后再把真正希望提交的内容重新提交一次。两个缺点：（1）需要手动提交每一个真正要提交的内容，如果内容很多，很浪费时间。（2）`--mixed`很容易打成`--hard`，万一哪天喝了酒然后对没有commit的内容使用了`git reset --hard`，就等着哭吧。
好办法：直接在reset 命令后面加上要目标文件即可，`git reset HEAD unstage_file`。

## cherry-pick a merge commit
通常来说我们无法直接cherry-pick一个merge commit，因为merge commit包含两个parent commits。git无法确定到底使用哪一个commit作为parent commit，除非你告诉它。
```bash
git cherry-pick -m 1 <commit_id>
```
默认情况下，1是merge分支，即主干，2是被merge的分支，即topic分支。

## 向gerrit推送一个merge commit
上一个问题我们说过，cherry-pick一个merge commit必须加入-m参数以确定parent commit。
而我们的gerrit同样也是使用cherry-pick来收集我们的提交，但是很多情况下gerrit cherry-pick行为是无法自行加参数的，至少非管理员是不能加的。
这时候，如果不想通过联系管理员解决（管理员最大的可能是让提交者自行解决），用下面的方法。
1.新建一个push分支，站在和develop同一个节点，然后cherry-pick -m merge节点，然后在推送push分支，虽然多了一步，但是目前是最方便的做法了。

## 报错：cannot update paths and switch to branch xxx at the same time
使用git checkout -b local_branch origin/branch的时候，遇到这个错误，字面上的意思是不能同时更改路径和切换分支，实际上该问题的一个根源是orign/branch在本地没有被登记，需要git remote update 或者 git fetch origin/branch来更新，然后再checkout。

## 自动沿用上一次冲突解决策略
解决冲突之后，如果觉得不满意，想要重新解决，会发现再一次cherry-pick\rebase时，已经没有冲突了，并且git会提示
“Resolved xxx using previous resolution”
这就是git的自动沿用上一次冲突解决策略。
可以使用下面的命令关掉。
```bash
git config --global rerere.enabled true
```


# 如何删除submodule

有的时候不知为何搞错了配置，需要删掉submodule然后再重新添加，然而git中删除submodule没有很简单，而是需要一下五步操作。

Step 1: 删除.gitmodule中的submodule相关配置

Step 2: 删除.git/config中的submodule相关配置

Step 3: git rm --cached path_to_submodule

Step 4: rm -rf path_to_submodule

Step 5: rm -rf .git/modules/path_to_submodule.

# git配置代理

前言：有的时候github网络抽风连不上，需要proxy进行稳定连接，设置方式如下：

```
git config --global http.proxy 192.168.1.5:2802
```

IP地址和端口号就是对应的代理服务机器和端口，至于proxy服务器怎么搭建，就不在这里说了。
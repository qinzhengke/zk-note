# git

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
```

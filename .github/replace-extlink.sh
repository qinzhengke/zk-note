BRANCH=$1

find . -name "*.md" | xargs -I {} sed -i "s/@@/https:\/\/github.com\/qinzhengke\/zk-note\/blob\/$1\//g" {}
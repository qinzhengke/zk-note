BRANCH=$1

find . -name "*.md" | xargs -I {} sed -i -E "s/blob\/[a-z]+/blob\/$1/g" {}
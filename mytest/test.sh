#!/bin/bash

# 打印所有参数
echo "全部参数: $*"
echo "参数个数: $#"

# 逐个打印参数
for arg in "$@"
do
    echo "参数: $arg"
done

# 访问特定参数
echo "第一个参数: $1"
echo "第二个参数: $2"

if [ $1 == "install" ]; then
  echo "yes"
else
  echo "no"
fi
# ./test.sh arg1 arg2 arg3
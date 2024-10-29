#!/bin/bash
#将插件打包，使用时直接导入插件

# 切换到build目录
cd build || { echo "build目录不存在"; exit 1; }
cp Sonar_pi-0.0.1.5-ubuntu-x86_64-24.04.xml metadata.xml
tar -zxvf Sonar_pi-0.0.1.5-ubuntu-x86_64-24.04.tar.gz
cp metadata.xml Sonar_pi-0.0.1.5-ubuntu-x86_64-24.04/ 
tar -czvf Sonar_pi.tar.gz Sonar_pi-0.0.1.5-ubuntu-x86_64-24.04/*
rm -rf Sonar_pi-0.0.1.5-ubuntu-x86_64-24.04/
cp Sonar_pi.tar.gz ~/Documents
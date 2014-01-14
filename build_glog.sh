#!/bin/sh
if [ ! -d third_party/local ]
then
    mkdir -p third_party/local
fi

cd third_party/local
PREFIX_PATH=`pwd`
cd ..

if [ ! -d glog ]
then
    svn checkout http://google-glog.googlecode.com/svn/trunk/ glog
    cd glog
    ./configure --prefix=$PREFIX_PATH
    make install
    make clean
fi


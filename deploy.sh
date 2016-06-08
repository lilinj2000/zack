#! /bin/sh

home_app=~/app

home_zack=${home_app}/zack

./configure --prefix=${home_zack}

if test -d ${home_zack}; then
    rm -rf ${home_zack}
fi

make install

make distclean

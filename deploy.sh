#! /bin/sh

home_app=~/app

if test -d /llj/app ; then
   home_app=/llj/app
fi

home_zack=${home_app}/zack

./configure --prefix=${home_zack}

if test -d ${home_zack}; then
    rm -rf ${home_zack}
fi

make install

make distclean

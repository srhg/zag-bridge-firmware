#!/bin/bash
RELDIR=$(cd $(dirname $BASH_SOURCE) && pwd)
#rm -rf sdcc-source
svn co -r 9092 svn://svn.code.sf.net/p/sdcc/code/trunk/sdcc sdcc-source || exit
sed -i 's/^\(MODELS = .*\)$/\1 huge/g' sdcc-source/device/lib/incl.mk
sed -i 's/^\(TARGETS\s*+=\s*models\s*\)small\(-.*\)$/\1model\2/g' sdcc-source/device/lib/Makefile.in
sed -i 's/-Werror//g' sdcc-source/support/sdbinutils/bfd/configure
sed -i 's/-Werror//g' sdcc-source/support/sdbinutils/bfd/warning.m4
sed -i 's/-Werror//g' sdcc-source/support/sdbinutils/binutils/configure
cd sdcc-source
./configure --disable-gbz80-port --disable-z80-port --disable-ds390-port --disable-ds400-port --disable-pic14-port --disable-pic16-port --disable-hc08-port --disable-r2k-port --disable-z180-port --disable-sdcdb --disable-ucsim --prefix=$RELDIR/sdcc
make
make install && cd .. && rm -rf sdcc-source || exit
ln -fs $(which python2) "$RELDIR/sdcc/bin/python"

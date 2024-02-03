#! /bin/bash

autoreconf
automake --add-missing
autoreconf

./configure
make
sudo make install

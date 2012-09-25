# BFR #

## Better Function Replacer based on APD ##

## Installation

Get the source github style and run the following in the source directory:
```
# phpize
```

Output on my system looked like this. May vary depending on system.
```
PHP Api Version: 20100412
Zend Module Api No: 20100525
Zend Extension Api No: 220100525
```

Run the usuall stuff:
```
# ./configure --enable-bfr
```
```
# make && make install
```

Modify your php.ini. Add:
```
zend_extension = /usr/lib/php5/20100525/bfr.so
```

You should see the extension on the output of:
```
php5 --version
```
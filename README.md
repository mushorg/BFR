# BFR #

## Better Function Replacer based on APD ##

APD http://pecl.php.net/package/apd started to have issues with the current Zend API. Is some of my tools strongly rely on APD's function renaming and overwriting features, I was successful in extracting those features and bundling them into a 'new' Zend extension. All credits go to the original developers. Sadly they don't respond to issues with the original version.

### Installation ###

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

Run the usual stuff:
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
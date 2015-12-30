# BFR #

## Better Function Replacer based on APD ##

APD http://pecl.php.net/package/apd started to have issues with the current Zend API. As some of my tools strongly rely on APD's function renaming and overwriting features, I had to get my hands dirty. I finally was successful in extracting those features and bundling them into a 'new' Zend extension. All credits go to the original developers. Sadly they don't respond to issues with the original version.

### Installation Linux ###

Install requirements:
```
# sudo apt install php5-dev
```

Get the source github style and run the following in the source directory:
```
# cd BFR
# phpize
```

Output on my system looked like this. May vary depending on system.
```
Configuring for:
PHP Api Version:         20100412
Zend Module Api No:      20100525
Zend Extension Api No:   220100525
```

Run the usual stuff:
```
# ./configure --enable-bfr
# make && make install
```
```
Build complete.
Don't forget to run 'make test'.

Installing shared extensions:     /usr/lib/php5/20131226/
```

Modify your php.ini accordingly. Add:
```
zend_extension = /usr/lib/php5/20131226/bfr.so
```

You should see the extension on the output of:
```
# php5 --version

PHP 5.4.4-2 (cli) (built: Jun 20 2012 09:52:11)
Copyright (c) 1997-2012 The PHP Group
Zend Engine v2.4.0, Copyright (c) 1998-2012 Zend Technologies
    with Better Function Replacer (BFR) v0.1, , by Lukas Rist
```

### Installation *BSD ###

Install the dependencies:
```
pkg_add -r autoconf automake php git
```

Create symbolic links:
```

# ln -s /usr/local/bin/php-config-5.3 /usr/local/bin/php-config
# ln -s /usr/local/bin/phpize-5.3 /usr/local/bin/phpize
```

Get the source github style and run the following in the source directory:
```
# cd BFR
# phpize
```

Output on my system looked like this. May vary depending on system.
```
Configuring for:
PHP Api Version:         20100412
Zend Module Api No:      20100525
Zend Extension Api No:   220100525
```
Run the following comands. Replace the versions according to your system:
```
# export AUTOCONF_VERSION=2.61
# export AUTOMAKE_VERSION=1.9
```

Run the usual stuff:
```
# ./configure --enable-bfr
# make && make install
```
```
Build complete.
Don't forget to run 'make test'.

Installing shared extensions:     /usr/local/lib/php-5.3/modules/
```

Modify your php.ini accordingly. Add:
```
zend_extension = /usr/local/lib/php-5.3/modules/bfr.so
```

You should see the extension on the output of:
```
# php-5.3 --version

PHP 5.3.14 with Suhosin-Patch (cli) (built: Aug  3 2012 07:43:07)
Copyright (c) 1997-2012 The PHP Group
Zend Engine v2.3.0, Copyright (c) 1998-2012 Zend Technologies
    with Better Function Replacer (BFR) v0.1, , by Lukas Rist
    with Suhosin v0.9.33, Copyright (c) 2007-2012, by SektionEins GmbH
```

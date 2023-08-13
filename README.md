# BFR #

## Better Function Replacer based on APD ##

APD http://pecl.php.net/package/apd started to have issues with the current Zend API. As some of my tools strongly rely on APD's function renaming and overwriting features, I had to get my hands dirty. I finally was successful in extracting those features and bundling them into a 'new' Zend extension. All credits go to the original developers. Sadly they don't respond to issues with the original version.

### Installation Linux ###

Install requirements:
```
# sudo apt install php-dev
```

Get the source github style and run the following in the source directory:
```
# cd BFR
# phpize
```

Output on my system looked like this. May vary depending on system.
```
Configuring for:
PHP Api Version:         20190902
Zend Module Api No:      20190902
Zend Extension Api No:   320190902
```

Run the usual stuff:
```
# ./configure --enable-bfr
# make && make install
```
```
Build complete.
Don't forget to run 'make test'.

Installing shared extensions:     /usr/lib/php/20190902/
```

Modify your php.ini accordingly. 

Find the php.ini file:
```
php --ini | grep Loaded
```

Add the extension to the ini (example):
```
echo "zend_extension = /usr/lib/php/20190902/bfr.so" | sudo tee -a /etc/php/7.4/cli/php.ini
```

You should see the extension on the output of:
```
# php --version

PHP 7.4.3-4ubuntu2.19 (cli) (built: Jun 27 2023 15:49:59) ( NTS )
Copyright (c) The PHP Group
Zend Engine v3.4.0, Copyright (c) Zend Technologies
    with Better Function Replacer (BFR) v0.1, Copyright (C) 2015, by Lukas Rist
    with Zend OPcache v7.4.3-4ubuntu2.19, Copyright (c), by Zend Technologies
```

### Installation *BSD ###

Install the dependencies:
```
pkg_add -r autoconf automake php git
```

Create symbolic links:
```

# ln -s /usr/local/bin/php-config-7.0 /usr/local/bin/php-config
# ln -s /usr/local/bin/phpize-7.0 /usr/local/bin/phpize
```

Get the source github style and run the following in the source directory:
```
# cd BFR
# phpize
```

Output on my system looked like this. May vary depending on system.
```
Configuring for:
PHP Api Version:         20151012
Zend Module Api No:      20151012
Zend Extension Api No:   320151012
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

Installing shared extensions:     /usr/local/lib/php-7.0/modules/
```

Modify your php.ini accordingly. Add:
```
zend_extension = /usr/local/lib/php-7.0/modules/bfr.so
```

You should see the extension on the output of:
```
# php-7.0 --version

PHP 7.0.7 with Suhosin-Patch (cli) (built: Jun  1 2016 00:58:54)
Copyright (c) 1997-2016 The PHP Group
Zend Engine v3.0.0, Copyright (c) 1998-2016 Zend Technologies
    with Better Function Replacer (BFR) v0.1, , by Lukas Rist
    with Suhosin v0.9.33, Copyright (c) 2007-2012, by SektionEins GmbH
```

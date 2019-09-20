# Php example

## Building and running

### php-zmq

See the notes below.

**Install using phpbrew**

If you are using phpbrew, you can install the extension with:

```shell
phpbrew ext install github:zeromq/php-zmq master
```

**Install as a Debian package**

```shell
apt-get install php-zmq  # debian stretch and earlier?
```

**Build and install from the source**

```shell
git clone https://github.com/zeromq/php-zmq
cd php-zmq
./configure
make && sudo -c "make install"
```

Additionally, add "extension=zmq" line to the php.ini file.

### Notes

**php-zmq**

- php-zmq exists in PECL; outdated and has issues w/ php5
    - https://github.com/zeromq/php-zmq/issues/203
    - https://github.com/zeromq/php-zmq/issues/193
    - https://pecl.php.net/package/zmq

- github.com/zeromq/php-zmq:master seems to work (2019-09-20)
    - phpbrew-ext seems to build the extension OK
        - https://github.com/phpbrew/phpbrew/wiki/Extension-Installer
    - I was unable to get composer to do the same
        - https://getcomposer.org/doc/05-repositories.md
        - https://github.com/composer/composer/pull/498

- Debian packages php-zmq
    - Includes the fixes that PECL version does not
    - A virtual package as of buster?

### Now that's out of the way

```shell
../bin/bootstrap -p ../proto/ -l php -d lib/proto/
composer install
./bin/demo [args...]
```

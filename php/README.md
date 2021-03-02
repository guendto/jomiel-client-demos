# Php example

## php-zmq

php-zmq package exists in [PECL] but it is outdated (and has issues with
php7/8). See:

- <https://github.com/zeromq/php-zmq/issues/203>
- <https://github.com/zeromq/php-zmq/issues/193>

There are three possible ways to work around this (choose one):

### Option #1: Install using phpbrew

If you are using phpbrew, you can install the extension with:

```shell
phpbrew ext install github:zeromq/php-zmq master
```

### Option #2: Install as a Debian package

```shell
apt-get install php-zmq
```

### Option #3: Install from the source (recommended)

```shell
git clone https://github.com/zeromq/php-zmq
cd php-zmq
./configure
make && sudo -c "make install"
```

And add `extension=zmq` to the `php.ini` file.

## Once php-zmq is installed

```shell
../bin/bootstrap -p ../proto/ -l php -d lib/proto/
composer install
./bin/demo [args...]
```

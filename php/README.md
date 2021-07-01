# jomiel client demo written in Php

## Notable differences to other examples

- Output defaults to JSON

## php-zmq

php-zmq in [PECL] is outdated and has issues with php7/8. See:
- <https://github.com/zeromq/php-zmq/issues/203>
- <https://github.com/zeromq/php-zmq/issues/193>

[pecl]: https://pecl.php.net/package/zmq

To workaround this, choose one:

### Option #1: Install as a distro package

```shell
apk add php7-pecl-zmq    # alpine
apt-get install php-zmq  # debian (remains untested)
```

`php8-pecl-zmq` isn't available for Alpine v3.13, but can be found in
`edge/testing`:

```shell
apk add php8-pecl-zmq \
  --repository=http://dl-cdn.alpinelinux.org/alpine/edge/testing/
```

### Option #2: Install from the source

```shell
git clone https://github.com/zeromq/php-zmq
cd php-zmq
./configure
make && sudo -c "make install"
```

### Option #3: Install using phpbrew

If you are using [phpbrew], you can install the extension with:

```shell
phpbrew ext install github:zeromq/php-zmq master
```

[phpbrew]: https://phpbrew.github.io/phpbrew/

## Once php-zmq is installed

- Confirm that php is reporting the extension
  - Make sure you add `extension=zmq` to the `php.ini` file
  - Modify `extension_dir` if needed

```shell
php -m | grep zmq  # php hould report the "zmq" extension
```

```shell
../bin/bootstrap -p ../proto/ -l php -d src/build/gen/
composer install
./bin/demo [args...]
```

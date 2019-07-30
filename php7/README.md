# Php example

## Building and running

```shell
$ composer install
$ phpbrew ext install github:zeromq/php-zmq master # See notes
$ ../bin/bootstrap -p ../proto/ -l php -d lib/proto/
$ ./bin/demo [args...]
```

### Notes

- At the time of writing this, the pecl package for php-zmq sits at
  1.1.3 can fails to build (for me, at least)

- As a workaround, grab the code from gh and build that, instead

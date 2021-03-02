# Notes.md

## php-zmq

tldr; build from the cloned [repo]:master.

- Exists in [PECL] but it is outdated (and has issues with php7/8)
  - Related issues:
    - <https://github.com/zeromq/php-zmq/issues/203>
    - <https://github.com/zeromq/php-zmq/issues/193>

[pecl]: https://pecl.php.net/package/zmq

- `phpbrew ext install github:zeromq/php-zmq master`
  - Not ideal because:
    - We want to install everything from `packagist.org` (with composer)
    - Would require the installation of phpbrew

[repo]: https://github.com/zeromq/php-zmq

- Debian has the php-zmq [package]
  - We are not using debian in our docker images (maybe later?)
  - Remains untested

[package]: https://packages.debian.org/buster/php-zmq

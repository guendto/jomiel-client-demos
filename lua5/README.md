# Lua example

## Building and running

```shell
$ for i in {lua-protobuf,argparse,serpent,log4l,lzmq}; do luarocks install $i || break; done
$ ./bin/demo [args...]
```

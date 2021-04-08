# jomiel-examples

`jomiel-examples` is a selection of demo programs written in different
modern programming languages showing how the [jomiel] inquiries are made
and the responses are handled.

![Example](./docs/demo.svg)

The examples are functionally similar but not necessarily identical (in
terms of features). Each example shows how to

- `make an inquiry` and
- `handle the result`.

Some examples have extra features, like additional output options.

|                                                                           | Notes       |
| ------------------------------------------------------------------------- | ----------- |
|  [C](https://github.com/guendto/jomiel-examples/blob/master/c)            |             |
|  [C++](https://github.com/guendto/jomiel-examples/blob/master/cpp)        | C++17       |
|  [C#](https://github.com/guendto/jomiel-examples/blob/master/csharp)      |             |
|  [Go](https://github.com/guendto/jomiel-examples/blob/master/golang)      |             |
|  [Java](https://github.com/guendto/jomiel-examples/blob/master/java)      |             |
|  [Kotlin](https://github.com/guendto/jomiel-examples/blob/master/kotlin)  |             |
|  [Lua](https://github.com/guendto/jomiel-examples/blob/master/lua5)       | version 5   |
|  [Node](https://github.com/guendto/jomiel-examples/blob/master/nodejs)    | Node.js 14  |
|  [Perl](https://github.com/guendto/jomiel-examples/blob/master/perl5)     | version 5   |
|  [Php](https://github.com/guendto/jomiel-examples/blob/master/php)        | version 7/8 |
|  [Python](https://github.com/guendto/jomiel-examples/blob/master/python3) | version 3   |
|  [Qt](https://github.com/guendto/jomiel-examples/blob/master/qt)          |             |
|  [Ruby](https://github.com/guendto/jomiel-examples/blob/master/ruby)      |             |
|  [Rust](https://github.com/guendto/jomiel-examples/blob/master/rust)      |             |
|  [Scala](https://github.com/guendto/jomiel-examples/blob/master/scala)    |             |

## Dockerfiles

You can find a Dockerfile in each `language` specific `docker`
directory. In some cases (e.g. [cpp] or [golang]), there are more than
one Dockerfile (typically one for `alpine-linux` and `debian` image).

To get started, see `jomiel` [HOWTO] for building and running
instructions.

### docker-compose

See also [docker-compose.yaml] file.

## License

`jomiel-examples` is licensed under the [Apache License version
2.0][aplv2].

[howto]: https://github.com/guendto/jomiel/blob/master/docs/HOWTO.md#build-and-run-jomiel-in-a-container
[aplv2]: https://www.tldrlegal.com/l/apache2
[jomiel]: https://github.com/guendto/jomiel
[cpp]: https://github.com/guendto/jomiel-examples/blob/master/cpp/docker
[golang]: https://github.com/guendto/jomiel-examples/blob/master/golang/docker
[docker-compose.yaml]: https://github.com/guendto/jomiel-examples/blob/master/docker-compose.yaml

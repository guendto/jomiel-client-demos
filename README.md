# jomiel-proto

The [Protocol Buffer] declarations for [jomiel] messages.

## License

`jomiel-proto` is licensed under the [Apache License version
2.0][aplv2].

## Installation

You can use the `bootstrap` [Bash] script to compile the [Protocol
Buffer] declarations for the target language. The script is a simple
wrapper script for `protoc` (and `protoc-c`). See the [notes] for a full
description, and run the script with the `-h` switch for more help.

Alternatively, you can call `protoc` (or `protoc-c`) directly to produce
the declarations for the target language.

### Python

[PyPI] package does not exist, you can either git-subtree `jomiel-proto`
into your project, or install the pre-compiled Python bindings provided
by the [jomiel-messages] package.

## Acknowledgements

Linted by [buf.build].

[notes]: https://github.com/guendto/jomiel-proto/blob/master/bin/bootstrap#L13
[protocol buffer]: https://developers.google.com/protocol-buffers/
[jomiel-messages]: https://github.com/guendto/jomiel-messages
[aplv2]: https://www.tldrlegal.com/l/apache2
[buf.build]: https://github.com/bufbuild/buf
[jomiel]: https://github.com/guendto/jomiel/
[bash]: https://www.gnu.org/software/bash/
[pypi]: https://pypi.org

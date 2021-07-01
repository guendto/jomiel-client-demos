# jomiel-proto

The [Protocol Buffer] declarations for [jomiel] messages.

## About

This repository contains the [Protocol Buffer] declaration files
(.proto) that can be compiled for different target languages and used to
communicate with [jomiel].

## bootstrap script

The `./bin/bootstrap` is a [Bash] script for compiling the [Protocol
Buffer] declarations for the target language. It is a mere wrapper
script that calls `protoc` (and `protoc-c`). Many of the examples of
[jomiel-client-demos] use this script.

```text
Usage: bootstrap [-p <protodir>] [-l <langid>] [-d <destdir>]

Notes
    - Make sure you have installed protobuf-compiler (protoc) and/or
      protobuf-c-compiler (protoc-c) if you intend to produce c
      bindings

    - Make sure the compilers can be found in your path

Where
    <protodir> path to the _root_ dir containing the .proto files to
        compile

    <langid> language binding identifier (e.g. "go" or "python"). The
        identifier must be understood by protoc(1); note that if the
        identifier is "c", the script will use protoc-c(1), instead of
        protoc(1).

    <destdir> is the destination dir for the compiled protobuf messages
```

Or, alternatively, call `protoc` (or `protoc-c`) directly to compile the
declarations for the target language.

[jomiel-client-demos]: https://github.com/guendto/jomiel-client-demos
[protocol buffer]: https://developers.google.com/protocol-buffers/
[jomiel]: https://github.com/guendto/jomiel/
[bash]: https://www.gnu.org/software/bash/

### Python

See [jomiel-messages].

[jomiel-messages]: https://github.com/guendto/jomiel-messages

## License

`jomiel-proto` is licensed under the [Apache License version
2.0][aplv2].

[aplv2]: https://www.tldrlegal.com/l/apache2

## Acknowledgements

Linted by [buf.build].

[buf.build]: https://github.com/bufbuild/buf

# Go example

## Building and running

Generate the protobuf bindings for golang:

```shell
go get -v google.golang.org/protobuf/cmd/protoc-gen-go
go generate
```

Build and run the demo:

```shell
go build -o demo
./demo [args...]
```

Alternatively, immediately after generating the bindings:

```shell
go run . [args ...]
```

## Notes

Due to an [issue] in [zeromq/goczmq]:

- `require gopkg.in/zeromq/goczmq.v4 v4.2.0` is commented out in
  `go.mod` file

- `import czmq gopkg.in/zeromq/goczmq.v4` are commented out in sources
  files

Instead:

- `require github.com/zeromq/goczmq v0.0.0-20190622112907-4715d4da6d4b`
  is being used in `go.mod` file

- `import czmq github.com/zeromq/goczmq` are used in source files

This demo will use `zeromq/goczmq@4715d4da6d4` as a workaround until the
upstream fixes the issue. For more details, see the [issue].

[issue]: https://github.com/zeromq/goczmq/issues/279
[zeromq/goczmq]: https://github.com/zeromq/goczmq

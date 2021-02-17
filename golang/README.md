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

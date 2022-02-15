# jomiel client demo written in Go

## Building and running

Generate the protobuf bindings for golang:

```shell
go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
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

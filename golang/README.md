# Go example

## Building and running

```shell
export GOPATH=`pwd -P`
export PATH="$GOPATH/bin:$PATH"
go get github.com/golang/protobuf/protoc-gen-go
../bin/bootstrap -p ../proto/ -l go -d src/jomiel.github.io/demo/examples/proto/
cd src/jomiel.github.io/demo/
go get
go build
./demo [args...]
```

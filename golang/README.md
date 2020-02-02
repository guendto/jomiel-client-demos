# Go example

## Building and running

```shell
export GOPATH=`pwd -P`
export PATH="$GOPATH/bin:$PATH"
go get -v github.com/golang/protobuf/protoc-gen-go
../proto/bin/bootstrap -p ../proto/ -l go -d src/jomiel.github.io/demo/examples/proto/
cd src/jomiel.github.io/demo/
go get -v
go build
./demo [args...]
```

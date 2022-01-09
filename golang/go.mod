module github.com/guendto/jomiel-client-demos/golang

go 1.14

replace internal/messages => ./internal/gen/jomiel/protobuf/v1beta1

require (
	github.com/ghodss/yaml v1.0.0
	github.com/pebbe/zmq4 v1.2.7
	github.com/spf13/pflag v1.0.5
	google.golang.org/protobuf v1.27.1
	gopkg.in/yaml.v2 v2.4.0 // indirect
	internal/messages v0.1.0
)

module github.com/guendto/jomiel-examples/golang

go 1.14

replace internal/gen/messages => ./internal/gen/jomiel/protobuf/v1beta1

require (
	github.com/ghodss/yaml v1.0.0 // indirect
	github.com/golang/protobuf v1.4.3 // indirect
	github.com/spf13/pflag v1.0.5 // indirect
	// gopkg.in/zeromq/goczmq.v4 v4.2.0 see README.md for Notes
	github.com/zeromq/goczmq v0.0.0-20190622112907-4715d4da6d4b // indirect
	gopkg.in/yaml.v2 v2.4.0 // indirect
	internal/gen/messages v0.1.0
)

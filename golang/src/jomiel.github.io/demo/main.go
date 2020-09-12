/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2020 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package main

import (
	"fmt"
	"log"
	"os"

	yaml "github.com/ghodss/yaml"
	flag "github.com/spf13/pflag"
	czmq "gopkg.in/zeromq/goczmq.v4"

	jomiel "jomiel.github.io/demo/golang"
)

var opts *jomiel.Options

func init() {
	log.SetOutput(os.Stderr)
	log.SetFlags(0)
	opts = jomiel.NewOptions()
}

func main() {

	flag.Parse()

	if opts.PrintConfig {
		fmt.Printf("---\n")
		prettyPrintYaml(opts)
		os.Exit(0)
	} else if opts.VersionZmq {
		fmt.Printf("ZeroMQ version %d.%d (czmq version %d.%d)\n",
			czmq.ZMQVersionMajor, czmq.ZMQVersionMinor,
			czmq.CZMQVersionMajor, czmq.CZMQVersionMinor)
		os.Exit(0)
	}

	if flag.NArg() < 1 {
		log.Fatalln("error: no input URI given")
	}

	jomiel := jomiel.NewJomiel(opts)
	defer jomiel.Destroy()

	jomiel.Connect()

	for i := 0; i < flag.NArg(); i++ {
		jomiel.Inquire(flag.Arg(i))
	}
}

func prettyPrintYaml(iface interface{}) {
	yaml, err := yaml.Marshal(iface)
	if err != nil {
		log.Fatalln("error: ", err)
	}
	fmt.Printf(string(yaml))
}

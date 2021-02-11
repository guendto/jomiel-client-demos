/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package app

import (
	"fmt"
	"log"
	"os"

	yaml "github.com/ghodss/yaml"
	flag "github.com/spf13/pflag"

	// czmq "gopkg.in/zeromq/goczmq.v4"  // See README.md for Notes
	czmq "github.com/zeromq/goczmq"
)

var opts *options

func init() {
	log.SetOutput(os.Stderr)
	log.SetFlags(0)
	opts = newOptions()
}

func Main() {

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

	jomiel := newJomiel(opts)
	defer jomiel.Destroy()

	jomiel.connect()

	for i := 0; i < flag.NArg(); i++ {
		jomiel.inquire(flag.Arg(i))
	}
}

func prettyPrintYaml(iface interface{}) {
	yaml, err := yaml.Marshal(iface)
	if err != nil {
		log.Fatalln("error: ", err)
	}
	fmt.Printf(string(yaml))
}

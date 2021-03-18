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
	zmq4 "github.com/pebbe/zmq4"
)

var opts *options

func init() {
	opts = newOptions()
	log.SetOutput(os.Stderr)
	log.SetFlags(0)
}

func Main() {
	if opts.PrintConfig {
		fmt.Printf("---\n")
		prettyPrintYaml(opts)
		os.Exit(0)
	} else if opts.VersionZmq {
		major, minor, patch := zmq4.Version()
		fmt.Printf("ZeroMQ version %d.%d.%d\n", major, minor, patch)
		os.Exit(0)
	}

	jomiel := newJomiel(opts)
	defer jomiel.Destroy()

	jomiel.connect()
}

func prettyPrintYaml(iface interface{}) {
	yaml, err := yaml.Marshal(iface)
	if err != nil {
		log.Fatalln("error: ", err)
	}
	fmt.Printf(string(yaml))
}

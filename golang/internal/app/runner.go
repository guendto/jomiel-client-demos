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

	"github.com/ghodss/yaml"
	zmq4 "github.com/pebbe/zmq4"
)

var opts options

func init() {
	opts = parseOptions()
	log.SetOutput(os.Stderr)
	log.SetFlags(0)
}

func Main() {
	if opts.PrintConfig {
		dumpValues(opts)
	} else if opts.ZmqVersion {
		printZmqVersion()
	} else {
		newJomiel(opts).inquire()
	}
}

func dumpValues(opts options) {
	yaml, err := yaml.Marshal(opts)
	if err != nil {
		log.Fatalln("error: ", err)
	}
	str := string(yaml)
	fmt.Printf(str)
}

func printZmqVersion() {
	major, minor, patch := zmq4.Version()
	fmt.Printf("ZeroMQ version %d.%d.%d\n", major, minor, patch)
}

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

import "github.com/spf13/pflag"

type options struct {
	RouterEndpoint string `json:"router-endpoint"`
	ConnectTimeout int    `json:"connect-timeout"`
	PrintConfig    bool   `json:"print-config"`
	ZmqVersion     bool   `json:"zmq-version"`
	OutputJSON     bool   `json:"output-json"`
	CompactJSON    bool   `json:"compact-json"`
	BeTerse        bool   `json:"be-terse"`
}

func newOptions() *options {
	opts := &options{}

	pflag.BoolVarP(
		&opts.PrintConfig,
		"print-config", "D",
		false,
		"Print configuration values and exit")

	pflag.StringVarP(
		&opts.RouterEndpoint,
		"router-endpoint", "r",
		"tcp://localhost:5514",
		"Specify the router endpoint address")

	pflag.IntVarP(
		&opts.ConnectTimeout,
		"connect-timeout", "t",
		30,
		"Specify maximum time in seconds for the connection allowed to take")

	pflag.BoolVarP(
		&opts.ZmqVersion,
		"version-zmq",
		"V",
		false,
		"Display ZeroMQ version and exit")

	pflag.BoolVarP(
		&opts.OutputJSON,
		"output-json", "j",
		false,
		"Print dumped messages in JSON")

	pflag.BoolVarP(
		&opts.CompactJSON,
		"compact-json", "c",
		false,
		"Use more compact representation of JSON")

	pflag.BoolVarP(
		&opts.BeTerse,
		"be-terse", "q",
		false,
		"Be brief and to the point; dump interesting details only")

	return opts
}

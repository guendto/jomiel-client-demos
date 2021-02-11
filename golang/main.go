/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package main

import "github.com/guendto/jomiel-examples/golang/internal/app"

func main() {
	app.Main()
}

//go:generate ../proto/bin/bootstrap -p ../proto -l go -d internal/gen

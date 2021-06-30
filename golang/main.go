/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package main

import "github.com/guendto/jomiel-client-demos/golang/internal/app"

func main() {
	app.Main()
}

//go:generate ../proto/bin/bootstrap -p ../proto -l go -d internal/gen

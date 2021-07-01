/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2019-2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package jomiel.client.demos;

import picocli.CommandLine;

import static java.lang.System.exit;

public final class App {
    public static void main(final String[] args) {
        exit(new CommandLine(new Runner()).execute(args));
    }
}

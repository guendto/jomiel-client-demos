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

package jomiel.client.demos

import picocli.CommandLine

object App {
  def main(args: Array[String]): Unit = {
    System.exit(new CommandLine(new Runner).execute(args: _*))
  }
}

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

package jomiel.examples

import picocli.CommandLine

object App {
  def main(args: Array[String]): Unit = {
    System.exit(new CommandLine(new Runner).execute(args: _*))
  }
}

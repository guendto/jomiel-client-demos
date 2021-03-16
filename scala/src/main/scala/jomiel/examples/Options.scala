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

import org.tinylog.scala.Logger.info
import org.zeromq.ZMQ.{getFullVersion, getVersionString}
import picocli.CommandLine.{Command, Option, Parameters, usage}

import java.lang.System.out
import java.util.concurrent.Callable

//noinspection VarCouldBeVal
@Command(name = "demo", usageHelpAutoWidth = true)
class Options extends Callable[Int] {

  @Option(names = Array("-h", "--help"),
    description = Array("Display this help message and exit"),
    usageHelp = true
  )
  private var help = false

  @Option(names = Array("-D", "--print-config"),
    description = Array("Print configuration values and exit")
  )
  private var printConfig = false

  @Option(names = Array("-r", "--router-endpoint"),
    paramLabel = "<addr>",
    description =
      Array("Specify the router endpoint address (${DEFAULT-VALUE})")
  )
  var routerEndpoint = "tcp://localhost:5514"

  @Option(names = Array("-t", "--connect-timeout"),
    paramLabel = "<timeout>",
    description =
      Array("Specify maximum time in seconds for the connection "
        + "allowed to take (${DEFAULT-VALUE})")
  )
  var connectTimeout = 30

  @Option(names = Array("-V", "--version-zmq"),
    description = Array("Display ZeroMQ version and exit")
  )
  private var zmqVersion = false

  @Option(names = Array("-j", "--output-json"),
    description = Array("Print dumped messages in JSON")
  )
  var outputJson = false

  @Option(names = Array("-q", "--be-terse"),
    description =
      Array("Be brief and to the point; dump interesting details only")
  )
  var beTerse = false

  @Parameters(arity = "0..*",
    paramLabel = "<uri>",
    description = Array("the URIs to parse")
  )
  var uri = new java.util.ArrayList[String]

  def call(): Int = {
    if (help) {
      usage(this, out)
    } else if (printConfig)
      dumpConfig()
    else if (zmqVersion) {
      printZmqVersion()
    } else {
      new Jomiel(this).run()
    }
    0
  }

  private def dumpConfig(): Unit = {
    info("---")
    for (field <- this.getClass.getDeclaredFields) yield {
      field.setAccessible(true)
      info(s"${field.getName}: ${field.get(this)}")
    }
  }

  private def printZmqVersion(): Unit = {
    info(s"ZeroMQ version $getFullVersion ($getVersionString)")
  }
}

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

import org.tinylog.kotlin.Logger.info
import org.zeromq.ZMQ.getFullVersion
import org.zeromq.ZMQ.getVersionString
import picocli.CommandLine.*
import java.lang.System.out
import java.util.concurrent.Callable
import kotlin.reflect.full.memberProperties
import kotlin.reflect.jvm.isAccessible

@Command(name = "demo", usageHelpAutoWidth = true)

class Options : Callable<Int> {

    @Option(names = ["-h", "--help"],
        description = ["Display this help message and exit"],
        usageHelp = true
    )
    private var help = false

    @Option(names = ["-D", "--print-config"],
        description = ["Print configuration values and exit"]
    )
    private var printConfig = false

    @Option(names = ["-r", "--router-endpoint"],
        paramLabel = "<addr>",
        description =
        ["Specify the router endpoint address (\${DEFAULT-VALUE})"]
    )
    var routerEndpoint = "tcp://localhost:5514"

    @Option(names = ["-t", "--connect-timeout"],
        paramLabel = "<timeout>",
        description = [
            "Specify maximum time in seconds for the connection "
                    + "allowed to take (\${DEFAULT-VALUE})"]
    )
    var connectTimeout: Long = 30

    @Option(names = ["-V", "--version-zmq"],
        description = ["Display ZeroMQ version and exit"]
    )
    private var zmqVersion = false

    @Option(names = ["-j", "--output-json"],
        description = ["Print dumped messages in JSON"]
    )
    var outputJson = false

    @Option(names = ["-c", "--compact-json"],
        description = ["Use more compact representation of JSON"]
    )
    var compactJson = false

    @Option(names = ["-q", "--be-terse"],
        description = [
            "Be brief and to the point; dump interesting details only"]
    )
    var beTerse = false

    @Parameters(arity = "0..*",
        paramLabel = "<uri>",
        description = ["the URIs to parse"]
    )
    var uri: ArrayList<String> = arrayListOf()

    override fun call(): Int {
        when {
            help -> usage(this, out)
            printConfig -> dumpConfig()
            zmqVersion -> printZmqVersion()
            else -> Jomiel(this).run()
        }
        return 0
    }

    private fun dumpConfig() {
        info("---")
        Options::class.memberProperties.forEach {
            it.isAccessible = true
            info("${it.name} = ${it.get(this)}")
        }
    }

    private fun printZmqVersion() {
        info("ZeroMQ version ${getVersionString()} (${getFullVersion()})")
    }
}

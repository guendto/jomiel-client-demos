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

import jomiel.protobuf.v1beta1.*
import jomiel.protobuf.v1beta1.Inquiry.Inquiry.Media
import org.tinylog.kotlin.Logger.error
import org.tinylog.kotlin.Logger.info
import org.zeromq.SocketType.REQ
import org.zeromq.ZContext
import org.zeromq.ZMQ.Poller.POLLIN
import pbandk.ExperimentalProtoJson
import pbandk.Message
import pbandk.decodeFromByteArray
import pbandk.encodeToByteArray
import pbandk.json.JsonConfig.Companion.DEFAULT
import pbandk.json.encodeToJsonString
import kotlin.system.exitProcess

class Jomiel(private val opts: Runner) {
    private val ctx = ZContext()
    private val sck = ctx.createSocket(REQ)
    private val poller = ctx.createPoller(1)

    @OptIn(ExperimentalProtoJson::class)
    private val jsonConfig =
        DEFAULT.copy(compactOutput = opts.compactJson)

    init {
        sck.linger = 0
        poller.register(sck, POLLIN)
    }

    fun run() {
        when {
            !opts.uri.isNullOrEmpty() -> {
                connect()
                opts.uri.forEach { inquire(it) }
            }
            else -> {
                error("error: input URI not given")
                exitProcess(1)
            }
        }
    }

    private fun connect() {
        val re = opts.routerEndpoint
        val to = opts.connectTimeout
        printStatus("<connect> $re (timeout=$to)")
        sck.connect(re)
    }

    private fun inquire(uri: String) {
        sendInquiry(uri)
        receiveResponse()
    }

    private fun sendInquiry(uri: String) {
        val msg = Inquiry(
            inquiry = Media(
                media = MediaInquiry(inputUri = uri)
            )
        )
        if (!opts.beTerse) printMessage("<send>", msg)
        val bytes = msg.encodeToByteArray()
        sck.send(bytes)
    }

    private fun receiveResponse() {
        poller.poll(opts.connectTimeout * 1_000)
        when {
            poller.pollin(0) -> {
                val bytes = sck.recv()
                val msg = Response.decodeFromByteArray(bytes)
                dumpResponse(msg)
            }
            else -> {
                error("error: connection timed out")
                exitProcess(1)
            }
        }
    }

    private fun printStatus(status: String) {
        if (!opts.beTerse) error("status: $status")
    }

    private fun dumpResponse(msg: Response) {
        val status = "<recv>"
        if (msg.status?.code == StatusCode.OK) {
            if (opts.beTerse) dumpTerseResponse(msg.media)
            else printMessage(status, msg.media as Message)
        } else printMessage(status, msg)
    }

    private fun dumpTerseResponse(msg: MediaResponse?) {
        info("---\ntitle: ${msg?.title}\nquality:")
        msg?.stream?.forEach {
            info("  profile: ${it.quality?.profile}\n"
                    + "    width: ${it.quality?.width}\n"
                    + "    height: ${it.quality?.height}"
            )
        }
    }

    @OptIn(ExperimentalProtoJson::class)
    private fun printMessage(status: String, msg: Message) {
        val result =
            if (opts.outputJson) msg.encodeToJsonString(jsonConfig)
            else msg.toString()
        printStatus(status)
        info(result)
    }
}

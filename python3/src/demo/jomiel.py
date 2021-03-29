#
# jomiel-examples
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
from logging import error
from logging import info

from google.protobuf.json_format import MessageToJson
from google.protobuf.text_format import MessageToString
from jomiel_messages.protobuf.v1beta1.message_pb2 import Inquiry
from jomiel_messages.protobuf.v1beta1.message_pb2 import Response
from jomiel_messages.protobuf.v1beta1.status_pb2 import STATUS_CODE_OK
from zmq import Context
from zmq import LINGER
from zmq import Poller
from zmq import POLLIN
from zmq import REQ


class Jomiel:
    __slots__ = ["ctx", "sck", "poller", "opts"]

    def __init__(self, options):
        self.ctx = Context.instance()
        self.sck = self.ctx.socket(REQ)
        self.poller = Poller()
        self.opts = options

        self.sck.setsockopt(LINGER, 0)
        self.poller.register(self.sck, POLLIN)

    def inquire(self):
        if self.opts.uri:
            self.connect()
            for uri in self.opts.uri:
                self.sendInquiry(uri)
                self.receiveResponse()
        else:
            raise RuntimeError("input URI not given")

    def connect(self):
        re = self.opts.router_endpoint
        to = self.opts.connect_timeout
        self.printStatus(f"<connect> {re} (timeout={to})")
        self.sck.connect(re)

    def sendInquiry(self, uri):
        msg = Inquiry()
        msg.media.input_uri = uri

        if not self.opts.be_terse:
            self.printMessage("<send>", msg)

        bytes_ = Inquiry.SerializeToString(msg)
        self.sck.send(bytes_)

    def receiveResponse(self):
        timeout = int(self.opts.connect_timeout) * 1000  # msec
        if self.poller.poll(timeout):
            bytes_ = self.sck.recv()
            msg = Response()
            msg.ParseFromString(bytes_)
            self.dumpResponse(msg)
        else:
            raise RuntimeError("connection timed out")

    def printStatus(self, status):
        if not self.opts.be_terse:
            error(f"status: {status}")

    def dumpResponse(self, msg):
        status = "<recv>"
        if msg.status.code == STATUS_CODE_OK:
            if self.opts.be_terse:
                self.dumpTerseResponse(msg.media)
            else:
                self.printMessage(status, msg.media)
        else:
            self.printMessage(status, msg)

    def dumpTerseResponse(self, msg):
        info(f"---\ntitle: ${msg.title}\nquality")
        for stream in msg.stream:
            info(
                f"  profile: {stream.quality.profile}\n"
                f"    width: {stream.quality.width}\n"
                f"    height: {stream.quality.height}",
            )

    def printMessage(self, status, msg):
        self.printStatus(status)
        result = (
            MessageToJson(msg)
            if self.opts.output_json
            else MessageToString(
                msg,
                pointy_brackets=True,
                as_utf8=True,
            ).rstrip()
        )
        info(result)

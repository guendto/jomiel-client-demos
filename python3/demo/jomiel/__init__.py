#
# jomiel-examples
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
"""TODO."""
import logging as lg
from sys import stdout

from jomiel.protobuf.v1beta1.message_pb2 import Inquiry
from jomiel.protobuf.v1beta1.message_pb2 import Response
from jomiel.protobuf.v1beta1.status_pb2 import STATUS_CODE_OK
from zmq import Context
from zmq import LINGER
from zmq import Poller
from zmq import POLLIN
from zmq import REQ


class Jomiel:
    """Jomiel"""

    __slots__ = ["ctx", "sck", "opts"]

    def __init__(self, options):
        """__init__"""
        self.ctx = Context.instance()
        self.sck = self.ctx.socket(REQ)
        self.sck.setsockopt(LINGER, 0)
        self.opts = options

    def connect(self):
        """connect"""
        addr = self.opts.router_endpoint
        time = self.opts.connect_timeout
        lg.info("<connect> %s (%s)", addr, time)
        self.sck.connect(addr)

    def inquire(self, uri):
        """inquire"""
        self.send(uri)
        self.recv()

    def send(self, uri):
        """send"""

        def inquiry_new():
            """Create a new (serialized) media inquiry message."""
            inquiry = Inquiry()
            inquiry.media.input_uri = uri

            if not self.opts.be_terse:
                self.print_message("<send>", inquiry)

            return Inquiry.SerializeToString(inquiry)

        inquiry = inquiry_new()
        self.sck.send(inquiry)

    def recv(self):
        """recv"""

        def receive_response():
            """Receive a response message from jomiel."""
            data = self.sck.recv()
            resp = Response()
            resp.ParseFromString(data)
            return resp

        poller = Poller()
        poller.register(self.sck, POLLIN)

        timeout = int(self.opts.connect_timeout) * 1000  # to msec

        if poller.poll(timeout):
            response = receive_response()
            self.dump_response(response)
        else:
            raise OSError("connection timed out")

    def dump_response(self, response):
        """Dump a response

        Args:
            response (obj): the response object to dump

        """

        def dump_terse_response(media_response):
            """Dump a terse response

            Args:
                response (obj): the response object to dump

            """
            print("---\ntitle: " + media_response.title)
            print("quality:")

            def get_terse_quality_string():
                """Return terse string for a stream quality."""
                return "  profile: {}\n    width: {}\n    height: {}".format(
                    stream_quality.profile,
                    stream_quality.width,
                    stream_quality.height,
                )

            for stream in media_response.stream:
                stream_quality = stream.quality
                quality_string = get_terse_quality_string()
                print(quality_string)

        if response.status.code == STATUS_CODE_OK:
            if self.opts.be_terse:
                dump_terse_response(response.media)
            else:
                self.print_message("<recv>", response.media)
        else:
            self.print_message("<recv>", response)

    def print_message(self, status, message):
        """Print a message with a status

        Args:
            status (str): the status message to print
            message (obj): the message object to dump

        """
        result = None

        if self.opts.output_json:
            from google.protobuf.json_format import MessageToJson

            result = MessageToJson(message)
        else:
            result = str(message)

        lg.info(status)
        stdout.write(result)


# vim: set ts=4 sw=4 tw=72 expandtab:

#
# jomiel-client-demos
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
from sys import exit

from demo.options import parse_options


class Runner:
    def __init__(self):
        opts = parse_options()

        if opts.print_config:
            self.dumpConfig(opts)
        elif opts.version_zmq:
            self.printZmqVersion()
        else:
            from demo.jomiel import Jomiel
            from demo.log import log_init

            log_init()
            try:
                Jomiel(opts).inquire()
            except RuntimeError as exc:
                self.handleError(exc)

    def dumpConfig(self, opts):
        from ruamel.yaml import YAML, round_trip_dump
        from sys import stdout

        yaml = YAML(typ="safe")
        yaml.default_flow_style = False
        print("---")
        round_trip_dump(opts.__dict__, stdout)
        exit(0)

    def printZmqVersion(self):
        from zmq import zmq_version

        print(f"ZeroMQ version {zmq_version()}")
        exit(0)

    def handleError(self, msg):
        from logging import error

        error(f"error: {msg}")
        exit(1)

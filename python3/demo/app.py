#
# jomiel-examples
#
# Copyright
#  2019-2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
"""TODO."""
import logging as lg
from sys import exit as _exit
from sys import stdout

from demo.jomiel import Jomiel
from demo.options import options_parse


def main():
    """main"""

    def enable_logger(value=True):
        """Enable/disable the logger

        Args:
            value (bool): if True, enables the logger, otherwise the
                logger is disabled

        """
        lg.getLogger().disabled = not value

    def print_error(msg):
        """Print the error message and exit.

        Note:
            Force-enables the logger so that the error messages get printed

        Args:
            msg: the message to be printed

        """
        enable_logger(True)
        lg.error(msg)
        _exit(1)

    opts = options_parse()
    jomiel = Jomiel(opts)

    lg.basicConfig(
        level=lg.INFO,
        format="[%(levelname)s] status: %(message)s",
    )

    enable_logger(not opts.be_terse)

    if opts.print_config:
        print_config(opts.__dict__)
    elif opts.version_zmq:
        print_version_zmq()

    if not opts.uri:
        print_error("no input URI given")

    jomiel.connect()

    try:
        for uri in opts.uri:
            jomiel.inquire(uri)
    except OSError as exc:
        print_error(str(exc))


def print_config(data):
    """Print config values."""
    from ruamel.yaml import YAML, round_trip_dump

    yaml = YAML(typ="safe")
    yaml.default_flow_style = False
    print("---")
    round_trip_dump(data, stdout)
    _exit(0)


def print_version_zmq():
    """Print ZeroMQ version."""
    from zmq import zmq_version

    print("ZeroMQ version %s" % zmq_version())
    _exit(0)


# vim: set ts=4 sw=4 tw=72 expandtab:

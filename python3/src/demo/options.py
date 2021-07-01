#
# jomiel-client-demos
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
from argparse import ArgumentDefaultsHelpFormatter
from argparse import ArgumentParser


def parse_options():
    p = ArgumentParser(
        formatter_class=ArgumentDefaultsHelpFormatter,
        prog="demo",
    )
    p.add_argument(
        "-D",
        "--print-config",
        help="Print configuration and exit",
        action="store_true",
    )
    p.add_argument(
        "-V",
        "--version-zmq",
        help="Display ZeroMQ version and exit",
        action="store_true",
    )
    p.add_argument(
        "-r",
        "--router-endpoint",
        metavar="<addr>",
        help="Specify the router endpoint address",
        default="tcp://localhost:5514",
    )
    p.add_argument(
        "-t",
        "--connect-timeout",
        metavar="<time>",
        help="""Specify maximum time in seconds for the connection
            allowed to take""",
        default=30,
    )
    p.add_argument(
        "-j",
        "--output-json",
        help="Print dumped messages in JSON",
        action="store_true",
    )
    p.add_argument(
        "-q",
        "--be-terse",
        help="Be brief and to the point; dump interesting details only",
        action="store_true",
    )
    p.add_argument(
        "uri",
        metavar="<uri>",
        nargs="*",
        help="the URIs to parse",
    )
    return p.parse_args()

# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
"""TODO."""

from argparse import ArgumentDefaultsHelpFormatter, ArgumentParser


def options_parse():
    """Parse command line args

    Returns:
        obj: the populated namespace

    """
    parser = ArgumentParser(
        prog="demo", formatter_class=ArgumentDefaultsHelpFormatter
    )

    parser.add_argument(
        "-D",
        "--print-config",
        help="Print configuration and exit",
        action="store_true",
    )

    parser.add_argument(
        "-V",
        "--version-zmq",
        help="Display ZeroMQ version and exit",
        action="store_true",
    )

    parser.add_argument(
        "-r",
        "--router-endpoint",
        metavar="<addr>",
        help="Specify the router endpoint address",
        default="tcp://localhost:5514",
    )

    parser.add_argument(
        "-t",
        "--connect-timeout",
        metavar="<time>",
        help="""Specify maximum time in seconds for the connection
            allowed to take""",
        default=30,
    )

    parser.add_argument(
        "-j",
        "--output-json",
        help="Print dumped messages in JSON",
        action="store_true",
    )

    parser.add_argument(
        "-q",
        "--be-terse",
        help="Be brief and to the point; dump interesting details only",
        action="store_true",
    )

    parser.add_argument(
        "uri", metavar="<uri>", nargs="*", help="the URIs to parse"
    )

    return parser.parse_args()


# vim: set ts=4 sw=4 tw=72 expandtab:

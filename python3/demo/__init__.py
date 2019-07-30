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

from sys import exit as _exit
import logging as lg


def main():
    """main"""
    from demo.app import run
    run()


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


def enable_logger(value=True):
    """Enable/disable the logger

    Args:
        value (bool): if True, enables the logger, otherwise the
            logger is disabled

    """
    lg.getLogger().disabled = not value


# vim: set ts=4 sw=4 tw=72 expandtab:

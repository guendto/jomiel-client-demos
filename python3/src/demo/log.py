#
# jomiel-client-demos
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
from logging import basicConfig
from logging import ERROR
from logging import INFO
from logging import StreamHandler
from sys import stderr
from sys import stdout


def log_init():
    out = StreamHandler(stdout)
    out.addFilter(lambda record: record.levelno <= INFO)
    out.setLevel(INFO)

    err = StreamHandler(stderr)
    err.setLevel(ERROR)

    basicConfig(
        level=INFO,
        format="%(message)s",
        handlers=[out, err],
    )

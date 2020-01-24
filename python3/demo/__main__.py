# -*- coding: utf-8 -*-
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

if __name__ == "__main__":
    from sys import path as sys_path
    sys_path.append(".")

    try:
        from demo.app import main
        main()
    except ModuleNotFoundError as error:
        msg = str(error)
        if "jomiel.protobuf" in msg:
            from sys import stderr, exit
            notice = 'Did you run the bootstrap script? See README.md'
            print('error: %s' % msg, file=stderr)
            print('error: %s' % notice, file=stderr)
            exit(1)
        else:
            raise error

# vim: set ts=4 sw=4 tw=72 expandtab:

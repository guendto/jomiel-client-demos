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

import logging as lg
from sys import exit as _exit
from sys import stdout

from demo import print_error, enable_logger
from demo.options import options_parse
from demo.jomiel import Jomiel


def run():
    """run"""
    opts = options_parse()
    jomiel = Jomiel(opts)

    lg.basicConfig(level=lg.INFO,
                   format="[%(levelname)s] status: %(message)s")

    enable_logger(not opts.be_terse)

    if opts.init:
        generate_proto_files()
    elif opts.print_config:
        print_config(opts.__dict__)
    elif opts.version_zmq:
        print_version_zmq()

    if not opts.uri:
        print_error('no input URI given')

    jomiel.connect()

    try:
        for uri in opts.uri:
            jomiel.inquire(uri)
    except IOError as exc:
        print_error(str(exc))


def generate_proto_files():
    """Generates the bindings for the .proto message declarations."""
    def detect_protoc():
        """Try to find te protoc(1) command."""
        from os.path import exists
        from os import environ

        if 'PROTOC' in environ and exists(environ['PROTOC']):
            return environ['PROTOC']

        from distutils.spawn import find_executable  # pylint: disable=E0401,E0611
        return find_executable('protoc')

    protoc = detect_protoc()
    if not protoc:
        print_error('protoc command not found')

    def find_proto_files():
        """Determine paths to .proto files"""
        from os.path import join
        input_path = '../proto'
        output_path = 'demo/proto/'
        proto_files = ['Message.proto', 'Status.proto', 'Media.proto']
        proto_files = [join(input_path, fname) for fname in proto_files]
        return {
            'input_path': input_path,
            'proto_files': proto_files,
            'output_path': output_path,
        }

    proto = find_proto_files()
    lg.info('Compiling the protobuf declarations for jomiel messages')

    from subprocess import call
    from os import EX_OK

    for fname in proto['proto_files']:
        lg.info('Compiling %s...', fname)
        args = [
            protoc, '-I' + proto['input_path'],
            '--python_out=' + proto['output_path'], fname
        ]
        if call(args) != EX_OK:
            _exit(1)

    lg.info('Done')
    _exit(0)


def print_config(data):
    """Print config values."""
    from ruamel.yaml import YAML, round_trip_dump
    yaml = YAML(typ='safe')
    yaml.default_flow_style = False
    print('---')
    round_trip_dump(data, stdout)
    _exit(0)


def print_version_zmq():
    """Print ZeroMQ version."""
    from zmq import zmq_version
    print('ZeroMQ version %s' % zmq_version())
    _exit(0)


# vim: set ts=4 sw=4 tw=72 expandtab:

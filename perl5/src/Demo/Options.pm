#
# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
use strict;
use warnings;
use warnings FATAL => 'all';

package Demo::Options {

  use feature qw(signatures);
  no warnings qw(experimental::signatures);

  use Getopt::Long::Descriptive;

  sub parse() {
    my ($opts, $usage) = describe_options(
      'demo %o [URI ...]',
      ['help|h', "Print this help and exit", { shortcircuit => 1 }],
      [
        'print-config|D',
        "Print configuration values and exit",
        { default => 0 }
      ],
      [
        'version-zmq|V',
        "Display ZeroMQ version and exit",
        { default => 0 }
      ],
      [
        'output-json|j',
        "Print dumped messages in JSON",
        { default => 0 }
      ],
      [
        'compact-json|c',
        "Use more compact representation of JSON",
        { default => 0 }
      ],
      [
        'router-endpoint|r=s',
        "Specify the router endpoint address",
        { default => "tcp://localhost:5514" }
      ],
      [
        'be-terse|q',
        "Be brief and to the point; dump interesting details only",
        { default => 0 }
      ],
      [
        'static-gen|g',
        "Generate static wrapper module for jomiel protobuf messages",
        { default => 0 }
      ],
      [
        'proto-dir|p=s',
        "Specify path to the dir with the .proto files",
        { default => '../proto' }
      ],
    );
    print $usage->text and exit if $opts->help;
    return $opts;
  }

}    # package Demo::Options

1;

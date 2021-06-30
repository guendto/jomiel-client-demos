#
# -*- coding: utf-8 -*-
#
# jomiel-client-demos
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
use v5.20;

package Demo::Runner {

  use feature 'signatures';
  no warnings 'experimental::signatures';

  use Demo::Options;

  sub main() {
    my $opts = Demo::Options::parse;
    if ($opts->static_gen) {
      static_gen($opts);
    } elsif ($opts->print_config) {
      dump_config($opts);
    } elsif ($opts->version_zmq) {
      print_zmq_version();
    } else {
      require Demo::Jomiel;
      Demo::Jomiel->new({ options => $opts })->inquire;
    }
    return;
  }

  sub dump_config ($opts) {
    require YAML;
    print YAML::Dump($opts);
    return;
  }

  sub print_zmq_version() {
    require ZMQ::FFI;
    my $zmq = ZMQ::FFI->new;
    say "ZeroMQ version ", join(".", $zmq->version);
    return;
  }

  sub static_gen ($opts) {
    my $pkg_name   = "Demo.Messages";
    my $pkg_map    = "jomiel.protobuf.v1beta1";
    my $pkg_prefix = "Messages";
    my $dest       = "src";

    my $gpd_out_val =
      "package=$pkg_name,map_package=$pkg_map,prefix=$pkg_prefix:$dest";

    my @args =
      ("-I" . $opts->proto_dir, "--perl-gpd_out=$gpd_out_val",);

    require File::Find::Rule;

    my @proto_files =
      File::Find::Rule->file()->name("*.proto")->in("./proto");

    push @args, @proto_files;

    local $ENV{'PATH'}     = "$ENV{'PATH'}:./local/bin";
    local $ENV{'PERL5LIB'} = "local/lib/perl5";

    print
"Compiling the static protobuf declarations for jomiel messages...";
    my $result = system "protoc", @args;
    say "done." if $result == 0;
    return;
  }

}    # package Demo::Runner

1;

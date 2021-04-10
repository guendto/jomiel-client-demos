#
# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
package Demo::Jomiel {

  use warnings;
  use strict;
  use v5.20;

  use feature qw(signatures);
  no warnings qw(experimental::signatures);

  use ZMQ::FFI qw(ZMQ_REQ);
  use JSON qw(decode_json);
  use YAML qw(Dump);

# use Data::Dumper;

  sub new ($class, $args) {
    my $ctx = ZMQ::FFI->new;
    my $sck = $ctx->socket(ZMQ_REQ);

    $sck->set_linger(0);

    return bless {
      _opts => $args->{options},
      _ctx  => $ctx,
      _sck  => $sck,
    }, $class;
  }

  sub inquire ($self) {
    unless (@ARGV == 0) {
      require Demo::Messages;

      $self->_connect;
      foreach (@ARGV) {
        $self->_send_inquiry($_);
        $self->_receive_response();
      }
    } else {
      say STDERR "error: no input URI given";
      exit 1;
    }
    return;
  }

  sub _connect ($self) {
    my $re = $self->{_opts}->router_endpoint;
    $self->_print_status("<connect> $re (timeout=-1)");
    $self->{_sck}->connect($re);
    return;
  }

  sub _send_inquiry ($self, $uri) {
    my $media = Messages::MediaInquiry->new;
    $media->set_input_uri($uri);

    my $msg = Messages::Inquiry->new;
    $msg->set_media($media);

    $self->_print_message('<send>', $msg)
      unless $self->{_opts}->be_terse;

    my $bytes = Messages::Inquiry->encode($msg);
    $self->{_sck}->send($bytes);
    return;
  }

  sub _receive_response ($self) {
    my $bytes = $self->{_sck}->recv;
    my $msg   = Messages::Response->decode($bytes);
    $self->_dump_response($msg);
    return;
  }

  sub _dump_response ($self, $msg) {
    my $status = "<recv>";
    if ($msg->get_status->get_code ==
      Messages::StatusCode->STATUS_CODE_OK) {
      if ($self->{_opts}->be_terse) {
        $self->_dump_terse_response($msg->get_media);
      } else {
        $self->_print_message($status, $msg->get_media);
      }
    } else {
      $self->_print_message($status, $msg);
    }
    return;
  }

  sub _dump_terse_response ($self, $msg) {
    say sprintf "---\ntitle: %s\nquality:", $msg->get_title;
    for (my $i = 0; $i < $msg->stream_size; $i++) {
      my $stream = $msg->get_stream($i);
      my $qty    = $stream->get_quality;
      say sprintf "  profile: %s\n    width: %d\n    height: %d",
        $qty->get_profile, $qty->get_width, $qty->get_height;
    }
    return;
  }

  sub _print_status ($self, $status) {
    say STDERR $status unless $self->{_opts}->be_terse;
    return;
  }

  sub _print_message ($self, $status, $msg) {
    $self->_print_status($status);

    if ($self->{_opts}->output_json) {
      my $str = $msg->encode_json . "\n";

      $str = JSON->new->utf8->pretty->encode(decode_json $str)
        unless $self->{_opts}->compact_json;

      print $str;
    } else {
      # print Dumper $msg;
      print Dump $msg;
    }
    return;
  }

}    # package Demo::Jomiel

1;

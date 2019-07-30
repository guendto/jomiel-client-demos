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
#
# Notable differences to the other examples:
#   - Messages are dumped in YAML
#   - Timeout is not implemented
#

package Jomiel;

use warnings;
use strict;

use v5.10;

use Google::ProtocolBuffers::Dynamic;
use ZMQ::FFI qw(ZMQ_REQ);
use YAML qw(Dump);

sub new {
    my ($class, $args) = @_;

    my $gpd = Google::ProtocolBuffers::Dynamic->new("../proto");
    $gpd->load_file('Message.proto');

    $gpd->map(
        { message => 'jomiel.Inquiry', to => 'Inquiry'},
        { message => 'jomiel.Response', to => 'Response'},
        { message => 'jomiel.media.MediaInquiry', to => 'MediaInquiry' },
        { message => 'jomiel.media.MediaResponse', to => 'MediaResponse' },
        { message => 'jomiel.status.ResponseStatus', to => 'ResponseStatus' },
        { enum => 'jomiel.status.StatusCode', to => 'StatusCode' }
    );

    my $ctx = ZMQ::FFI->new;
    my $sck = $ctx->socket(ZMQ_REQ);

    $sck->set_linger(0);

    my $self = bless {
        opts => $args->{opts},
        ctx => $ctx,
        sck => $sck,
        gpd => $gpd
    }, $class;
}

sub connect {
    my $self = shift;
    my $addr = $self->{opts}->{'router-endpoint'};
    $self->print_status("<connect> $addr");
    $self->{sck}->connect($addr);
}

sub inquire {
    my ($self, $uri) = @_;
    $self->send($uri);
    $self->recv;
}

sub send {
    my ($self, $uri) = @_;

    my $media = MediaInquiry->new;
    $media->set_input_uri($uri);

    my $inquiry = Inquiry->new;
    $inquiry->set_media($media);

    my $bytes = Inquiry->encode($inquiry);
    $self->{sck}->send($bytes);

    $self->print_message("<send>", $inquiry)
        unless $self->{opts}->{'be-terse'};
}

sub recv {
    my $self = shift;
    my $data = $self->{sck}->recv();
    my $response = Response->decode($data);
    $self->dump_response($response);
}

sub dump_response {
    my ($self, $response) = @_;

    my $status = $response->get_status;
    my $media = $response->get_media;

    my $code = $status->get_code;

    if ($code == StatusCode->OK) {
        if ($self->{opts}->{'be-terse'}) {
            $self->dump_terse_response($media);
        } else {
            $self->print_message("<recv>", $media);
        }
    } else {
        $self->print_message("<recv>", $response);
    }
}

sub dump_terse_response {
    my ($self, $media_response) = @_;
    say "---\ntitle: ", $media_response->get_title;
    say "quality:";

    for (my $i=0; $i<$media_response->stream_size; $i++) {
        my $stream = $media_response->get_stream($i);
        my $quality = $stream->get_quality;
        say sprintf("  profile: %s\n    width: %d\n    height: %d",
            $quality->get_profile,
            $quality->get_width,
            $quality->get_height);
    }
}

sub print_message {
    my ($self, $status, $message) = @_;
    $self->print_status($status);
    print Dump($message);
}

sub print_status {
    my ($self, $message) = @_;
    print STDERR "status: $message\n"
        unless $self->{opts}->{'be-terse'};
}

sub print_zmq_version {
    my $self = shift;
    say "ZeroMQ version ", join(".", $self->{ctx}->version);
    exit(0);
}

sub DESTROY {
    my $self = shift;
    undef $self->{sck};
    undef $self->{ctx};
}

1;

# vim: set ts=4 sw=4 tw=72 expandtab:

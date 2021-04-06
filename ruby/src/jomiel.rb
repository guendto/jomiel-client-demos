# frozen_string_literal: true

#
# jomiel-examples
#
# Copyright
#  2019-2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
require 'ffi-rzmq'
require 'json'
require 'jomiel/protobuf/v1beta1/message_pb'

PB = Jomiel::Protobuf::V1beta1
Status = PB::StatusCode

module Jomiel
  class Jomiel
    def initialize(opts)
      @opts = opts

      @ctx = ZMQ::Context.new
      @sck = @ctx.socket(ZMQ::REQ)
      @poller = ZMQ::Poller.new

      @sck.setsockopt(ZMQ::LINGER, 0)
      @poller.register_readable(@sck)

      # Add finalizer for ffi-rzmq (reason: <https://git.io/JYFf0>).
      ObjectSpace.define_finalizer(self, proc { @sck.close })
    end

    def inquire
      input = @opts['URI']
      Kernel.abort('error: no input URI given') if input.empty?
      connect
      input.each do |uri|
        send_inquiry(uri)
        receive_response
      end
    end

    private

    def connect
      re = @opts['--router-endpoint']
      to = @opts['--connect-timeout']
      print_status("<connect> #{re} (timeout=#{to})")
      @sck.connect(re)
    end

    def send_inquiry(uri)
      msg = PB::Inquiry.new(media: PB::MediaInquiry.new(input_uri: uri))
      str = PB::Inquiry.encode(msg)
      print_message('<send>', msg) unless @opts['--be-terse']
      @sck.send_string(str)
    end

    def receive_response
      timeout = @opts['--connect-timeout'].to_i * 1_000
      if @poller.poll(timeout).positive?
        str = +''
        @sck.recv_string(str)
        msg = PB::Response.decode(str)
        dump_response(msg)
      else
        Kernel.abort('error: connection timed out')
      end
    end

    def print_status(status)
      warn status unless @opts['--be-terse']
    end

    def dump_response(msg)
      status = '<recv>'
      if Status.resolve(msg.status.code) == Status::STATUS_CODE_OK
        media = msg.media
        if @opts['--be-terse']
          dump_terse_response(media)
        else
          print_message(status, media)
        end
      else
        print_message(status, msg)
      end
    end

    def dump_terse_response(msg)
      puts "---\ntitle: #{msg.title}\nquality:"
      msg.stream.each do |stream|
        qty = stream.quality
        puts "  profile: #{qty.profile}\n" \
               "    width: #{qty.width}\n" \
               "    height: #{qty.height}"
      end
    end

    def print_message(status, msg)
      print_status(status)
      puts @opts['--output-json'] ? to_json(msg) : msg
    end

    def to_json(msg)
      str = msg.to_json
      unless @opts['--compact-json']
        obj = JSON.parse(str)
        str = JSON.pretty_generate(obj)
      end
      str
    end
  end
end

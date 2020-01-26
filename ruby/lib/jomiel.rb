# frozen_string_literal: true

# jomiel-examples
#
# Copyright
#  2019-2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

require 'ffi-rzmq'
require 'json'

require 'jomiel/protobuf/v1alpha1/message_pb'

JP = Jomiel::Protobuf::V1alpha1
JPS = JP::StatusCode

module Jomiel
  # :nodoc:
  class Demo
    def initialize(opts, logger)
      @opts = opts
      @logger = logger
      @timeout = opts['--connect-timeout'].to_i
      @ctx = ZMQ::Context.new
      @sck = @ctx.socket ZMQ::REQ
      @sck.setsockopt(ZMQ::LINGER, 0)
    end

    def connect
      addr = @opts['--router-endpoint']
      printStatus("<connect> #{addr} (timeout=#{@timeout})")
      @sck.connect(addr)
    end

    def inquire(uri)
      send(uri)
      recv
    end

    def send(uri)
      inquiry =
        JP::Inquiry.new(media: JP::MediaInquiry.new(input_uri: uri))
      serialized = JP::Inquiry.encode(inquiry)
      @sck.send_string(serialized)
    end

    def recv
      poll = ZMQ::Poller.new
      poll.register_readable(@sck)

      if poll.poll(@timeout * 1_000).positive?
        data = +''
        @sck.recv_string(data)
        response = JP::Response.decode(data)
        dump_response(response)
      else
        Kernel.abort('error: connection timed out')
      end
    end

    def get_quality_string(stream_quality)
      return(
        <<QUALITYSTRING
  profile: #{stream_quality.profile}
    width: #{stream_quality.width}
    height: #{stream_quality.height}
QUALITYSTRING
      )
    end

    def dump_terse_response(media_response)
      puts "---\ntitle: #{media_response.title}"
      puts 'quality:'

      media_response.stream.each do |stream|
        stream_quality = stream.quality
        quality_string = get_quality_string(stream_quality)
        puts(quality_string)
      end
    end

    def dump_response(response)
      status = response.status
      if JPS.resolve(status.code) == JPS::STATUS_CODE_OK
        if @opts['--be-terse']
          dump_terse_response(response.media)
        else
          print_message('<recv>', response.media)
        end
      else
        print_message('<recv>', response)
      end
    end

    def print_message(status, message)
      printStatus(status)
      puts @opts['--output-json'] ? message.to_json : message
    end

    def printStatus(status)
      @logger.info(status) if not @opts['--be-terse']
    end
  end
end

# vim: set ts=4 sw=4 tw=72 expandtab:

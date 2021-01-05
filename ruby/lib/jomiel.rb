# frozen_string_literal: true

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

JP = Jomiel::Protobuf::V1beta1
JPS = JP::StatusCode

# Jomiel module.
module Jomiel
  # Returns the quality string created from the given StreamQuality.
  def self.get_quality_string(stream_quality)
    <<-QUALITYSTRING
  profile: #{stream_quality.profile}
    width: #{stream_quality.width}
    height: #{stream_quality.height}
    QUALITYSTRING
  end

  # A simple demo, that makes an inquiry and dumps the returned
  # metadata.
  class Demo
    def initialize(opts, logger)
      @opts = opts
      @opts[:logger] = logger
      @zmq = {
        timeout: opts['--connect-timeout'].to_i,
        socket: ZMQ::Context.new.socket(ZMQ::REQ)
      }
      @zmq[:socket].setsockopt(ZMQ::LINGER, 0)
    end

    def connect
      addr = @opts['--router-endpoint']
      print_status("<connect> #{addr} (timeout=#{@zmq[:timeout]})")
      @zmq[:socket].connect(addr)
    end

    def inquire(uri)
      send(uri)
      recv
    end

    def send(uri)
      inquiry =
        JP::Inquiry.new(media: JP::MediaInquiry.new(input_uri: uri))
      serialized = JP::Inquiry.encode(inquiry)
      @zmq[:socket].send_string(serialized)
    end

    def recv_data
      data = +''
      @zmq[:socket].recv_string(data)
      dump_response JP::Response.decode(data)
    end

    def recv
      poll = ZMQ::Poller.new
      poll.register_readable(@zmq[:socket])
      if poll.poll(@zmq[:timeout] * 1_000).positive?
        recv_data
      else
        Kernel.abort('error: connection timed out')
      end
    end

    def dump_terse_response(media_response)
      puts "---\ntitle: #{media_response.title}"
      puts 'quality:'

      media_response.stream.each do |stream|
        stream_quality = stream.quality
        puts Jomiel.get_quality_string(stream_quality)
      end
    end

    def dump_response(response)
      if JPS.resolve(response.status.code) == JPS::STATUS_CODE_OK
        media = response.media
        if @opts['--be-terse']
          dump_terse_response(media)
        else
          print_message('<recv>', media)
        end
      else
        print_message('<recv>', response)
      end
    end

    def print_message(status, message)
      print_status(status)
      puts @opts['--output-json'] ? message.to_json : message
    end

    def print_status(status)
      @opts[:logger].info(status) unless @opts['--be-terse']
    end
  end
end

# vim: set ts=4 sw=4 tw=72 expandtab:

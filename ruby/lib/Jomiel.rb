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

require "ffi-rzmq"
require "json"

require "Message_pb"

module Jomiel

    class Demo
        def initialize(opts, lg)
            @opts = opts
            @lg = lg
            @timeout = opts["--connect-timeout"].to_i
            @ctx = ZMQ::Context.new
            @sck = @ctx.socket ZMQ::REQ
            @sck.setsockopt(ZMQ::LINGER, 0)
        end

        def connect
            addr = @opts["--router-endpoint"]
            printStatus("<connect> #{addr} (timeout=#{@timeout})")
            @sck.connect(addr)
        end

        def inquire(uri)
            send(uri)
            recv()
        end

        def send(uri)
            inquiry = Jomiel::Inquiry.new(
                :media => Jomiel::Media::MediaInquiry.new(:input_uri => uri)
            )
            serialized = Jomiel::Inquiry.encode(inquiry)
            @sck.send_string(serialized)
        end

        def recv
            poll = ZMQ::Poller.new
            poll.register_readable(@sck)

            if poll.poll(@timeout * 1000) >0
                data = ''
                @sck.recv_string(data)
                response = Jomiel::Response.decode(data)
                dumpResponse(response)
            else
                Kernel.abort("error: connection timed out")
            end
        end

        def getQualityString(streamQuality)
            return <<QUALITYSTRING
  profile: #{streamQuality.profile}
    width: #{streamQuality.width}
    height: #{streamQuality.height}
QUALITYSTRING
        end

        def dumpTerseResponse(mediaResponse)
            puts "---\ntitle: #{mediaResponse.title}"
            puts "quality:"

            mediaResponse.stream.each do |stream|
                streamQuality = stream.quality
                qualityString = getQualityString(streamQuality)
                puts(qualityString)
            end
        end

        def dumpResponse(response)
            js = Jomiel::Status::StatusCode
            status = response.status
            if js.resolve(status.code) == js::OK
                if @opts["--be-terse"]
                    dumpTerseResponse(response.media)
                else
                    printMessage("<recv>", response.media)
                end
            else
                printMessage("<recv>", response)
            end
        end

        def printMessage(status, message)
            printStatus(status)
            puts @opts["--output-json"] ? message.to_json : message
        end

        def printStatus(status)
            if not @opts["--be-terse"]
                @lg.info(status)
            end
        end

    end
end

# vim: set ts=4 sw=4 tw=72 expandtab:

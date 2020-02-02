--
-- -*- coding: utf-8 -*-
--
-- jomiel-examples
--
-- Copyright
--  2019-2020 Toni Gündoğdu
--
--
-- SPDX-License-Identifier: Apache-2.0
--

local protoc = require 'protoc'
local pb = require 'pb'

local zpoller = require 'lzmq.poller'
local zmq = require 'lzmq'

local serpent = require 'serpent'

local Jomiel = {}
Jomiel.__index = Jomiel

function Jomiel.new(logger, opts)
    local sck, err = zmq.context():socket(zmq.REQ, { linger = 0 })
    zmq.assert(sck,err)

    local p = protoc.new()

    p.include_imports = true
    p:addpath('../proto')

    p:loadfile('jomiel/protobuf/v1alpha1/message.proto')

    assert(pb.type 'jomiel.protobuf.v1alpha1.Inquiry')
    assert(pb.type 'jomiel.protobuf.v1alpha1.MediaInquiry')
    assert(pb.type 'jomiel.protobuf.v1alpha1.Response')
    assert(pb.type 'jomiel.protobuf.v1alpha1.MediaResponse')

    return setmetatable({
        opts = opts,
        lg = logger,
        protoc = p,
        sck = sck,
    }, Jomiel)
end

function Jomiel:connect()
    local addr = self.opts.router_endpoint
    local time = self.opts.connect_timeout
    local msg = string.format('<connect> %s (timeout=%d)', addr, time)
    self:printStatus(msg)
    return self.sck:connect(addr)
end

function Jomiel:inquire(uri)
    self:send(uri)
    self:recv()
end

function Jomiel:send(uri)
    local function inquiry_new(uri)

        local inquiry = {
            media = {
                input_uri = uri
            }
        }

        if not self.opts.be_terse then
            self:printMessage('<send>', inquiry)
        end

        local bytes = pb.encode('jomiel.protobuf.v1alpha1.Inquiry', inquiry)
        self:printSerialized('send', bytes)

        return bytes
    end
    local bytes = inquiry_new(uri)
    return self.sck:send(bytes)
end

function Jomiel:recv()
    local poller = zpoller.new(1)

    poller:add(self.sck, zmq.POLLIN, function()
        local bytes = self.sck:recv()
        self:printSerialized('recv', bytes)
        local response = pb.decode('jomiel.protobuf.v1alpha1.Response', bytes)
        self:dumpResponse(response)
    end)

    local timeout = self.opts.connect_timeout * 1000 -- # to msec

    if poller:poll(timeout) == 0 then
        self.lg:error('connection timed out')
    end

    return rc
end

function Jomiel:printSerialized(prefix, bytes)
    if self.opts.print_serialized then
        local len = string.len(bytes)
        local hex = pb.tohex(bytes)
        local str = string.format('%s: [%d] %s', prefix, len, hex)
        self.lg:info(str)
    end
end

function Jomiel:printf(format, ...)
    return io.write(format:format(...))
end

function Jomiel:dumpTerseResponse(media_response)
    self:printf('---\ntitle: %s\n', media_response.title)
    self:printf('quality:\n')
    for _,stream in pairs(media_response.stream) do
        self:printf('  profile: %s\n    width: %d\n    height: %d\n',
                        stream.quality.profile,
                        stream.quality.width,
                        stream.quality.height)
    end
end

function Jomiel:dumpResponse(response)
    local code = pb.enum('jomiel.protobuf.v1alpha1.StatusCode',
                            response.status.code)
    local STATUS_CODE_OK = pb.enum('jomiel.protobuf.v1alpha1.StatusCode',
                            'STATUS_CODE_OK')
    if code == STATUS_CODE_OK then
        if self.opts.be_terse then
            self:dumpTerseResponse(response.media)
        else
            self:printMessage('<recv>', response.media)
        end
    else
        self:printMessage('<recv>', response)
    end
end

function Jomiel:printMessage(status, message)
    self:printStatus(status)
    print(serpent.block(message))
end

function Jomiel:printStatus(status)
    if not self.opts.be_terse then
        self.lg:info(status)
    end
end

return { Jomiel=Jomiel }

-- vim: set ts=4 sw=4 tw=72 expandtab:

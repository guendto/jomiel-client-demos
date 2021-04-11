--
-- -*- coding: utf-8 -*-
--
-- jomiel-examples
--
-- Copyright
--  2019-2021 Toni Gündoğdu
--
--
-- SPDX-License-Identifier: Apache-2.0
--
local class = require 'middleclass'

local inspect = require 'inspect'
local dkjson = require 'dkjson'

local poller = require 'lzmq.poller'
local zmq = require 'lzmq'

local pkg_path = 'jomiel.protobuf.v1beta1'
local protoc = require 'protoc'
local pb = require 'pb'

Jomiel = class('Jomiel')

local function load_proto_files(options)
  local protoc = protoc.new()
  protoc.include_imports = true
  protoc:addpath(options.proto_dir)
  protoc:loadfile(pkg_path:gsub('%.', '/') .. '/message.proto')
  --[[
  assert(pb.type 'jomiel.protobuf.v1beta1.Inquiry')
  assert(pb.type 'jomiel.protobuf.v1beta1.MediaInquiry')
  assert(pb.type 'jomiel.protobuf.v1beta1.Response')
  assert(pb.type 'jomiel.protobuf.v1beta1.MediaResponse')
  assert(pb.type 'jomiel.protobuf.v1beta1.StatusCode')
  ]]--
end

function Jomiel:initialize(options)
  load_proto_files(options)
  self.sck = zmq.context():socket(zmq.REQ, { linger = 0 })
  self.opts = options
end

function Jomiel:inquire()
  if next(self.opts.URI) then
    self:_connect()
    for _, uri in pairs(self.opts.URI) do
      self:_sendInquiry(uri)
      self:_receiveResponse()
    end
  else
    io.stderr:write("error: no input URI given\n")
    os.exit(1)
  end
end

function Jomiel:_connect()
  local re = self.opts.router_endpoint
  local to = self.opts.connect_timeout
  self:_printStatus("<connect> %s (timeout=%d)", re, to)
  self.sck:connect(re)
end

function Jomiel:_sendInquiry(uri)
  local msg = { media = { input_uri = uri } }
  if self.opts.be_terse == false then
    self:_printMessage('<send>', msg)
  end
  local bytes = pb.encode(pkg_path .. '.Inquiry', msg)
  self.sck:send(bytes)
end

--[[
local function print_serialized(bytes)
  local len = string.len(bytes)
  local hex = pb.tohex(bytes)
  io.stderr:write(string.format('<recv>: [%d] %s\n', len, hex))
end
]]--

function Jomiel:_receiveResponse()
  local poller = poller.new(1)

  poller:add(self.sck, zmq.POLLIN, function()
    local bytes = self.sck:recv()
    -- print_serialized(bytes)
    local msg = pb.decode(pkg_path .. '.Response', bytes)
    self:_dumpResponse(msg)
  end)

  if poller:poll(self.opts.connect_timeout * 1000) == 0 then
    io.stderr:write('error: connection timed out\n')
    os.exit(1)
  end
end

function Jomiel:_dumpResponse(msg)
  local lookup_path = pkg_path .. '.StatusCode'

  local status_code = pb.enum(lookup_path, msg.status.code)
  local status_ok = pb.enum(lookup_path, 'STATUS_CODE_OK')

  local status = '<recv>'

  if status_code == status_ok then
    if self.opts.be_terse then
      self:_dumpTerseResponse(msg.media)
    else
      self:_printMessage(status, msg.media)
    end
  else
    self:_printMessage(status, msg)
  end
end

function Jomiel:_dumpTerseResponse(msg)
  local function printf(fmt, ...)
    io.write(fmt:format(...) .. '\n')
  end
  printf('---\ntitle: %s\nquality:', msg.title)
  for _, stream in pairs(msg.stream) do
    printf('  profile: %s\n    width: %d\n    height: %d',
      stream.quality.profile,
      stream.quality.width,
      stream.quality.height)
  end
end

function Jomiel:_printStatus(fmt, ...)
  if self.opts.be_terse == false then
    io.stderr:write(fmt:format(...) .. '\n')
  end
end

function Jomiel:_printMessage(status, msg)
  self:_printStatus(status)
  local str
  if self.opts.output_json then
    str = dkjson.encode(msg, { indent = not self.opts.compact_json })
  else
    str = inspect(msg)
  end
  print(str)
end

return Jomiel

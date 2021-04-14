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
local Class = require 'middleclass'

local Inspect = require 'inspect'
local DkJSON = require 'dkjson'

local Poller = require 'lzmq.poller'
local ZMQ = require 'lzmq'

local Protoc = require 'protoc'
local PB = require 'pb'

local PkgPath = 'jomiel.protobuf.v1beta1'
Jomiel = Class('Jomiel')

local function loadProtoFiles(options)
  local protoc = Protoc.new()
  protoc.include_imports = true
  protoc:addpath(options.proto_dir)
  protoc:loadfile(PkgPath:gsub('%.', '/') .. '/message.proto')
  --[[
  assert(PB.type 'jomiel.protobuf.v1beta1.Inquiry')
  assert(PB.type 'jomiel.protobuf.v1beta1.MediaInquiry')
  assert(PB.type 'jomiel.protobuf.v1beta1.Response')
  assert(PB.type 'jomiel.protobuf.v1beta1.MediaResponse')
  assert(PB.type 'jomiel.protobuf.v1beta1.StatusCode')
  ]]--
end

function Jomiel:initialize(options)
  loadProtoFiles(options)
  self.sck = ZMQ.context():socket(ZMQ.REQ, { linger = 0 })
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
  local bytes = PB.encode(PkgPath .. '.Inquiry', msg)
  self.sck:send(bytes)
end

--[[
local function printSerialized(bytes)
  local len = string.len(bytes)
  local hex = PB.tohex(bytes)
  io.stderr:write(string.format('<recv>: [%d] %s\n', len, hex))
end
]]--

function Jomiel:_receiveResponse()
  local poller = Poller.new(1)

  poller:add(self.sck, ZMQ.POLLIN, function()
    local bytes = self.sck:recv()
    -- printSerialized(bytes)
    local msg = PB.decode(PkgPath .. '.Response', bytes)
    self:_dumpResponse(msg)
  end)

  if poller:poll(self.opts.connect_timeout * 1000) == 0 then
    io.stderr:write('error: connection timed out\n')
    os.exit(1)
  end
end

function Jomiel:_dumpResponse(msg)
  local lookup_path = PkgPath .. '.StatusCode'

  local status_code = PB.enum(lookup_path, msg.status.code)
  local status_ok = PB.enum(lookup_path, 'STATUS_CODE_OK')

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
    str = DkJSON.encode(msg, { indent = not self.opts.compact_json })
  else
    str = Inspect(msg)
  end
  print(str)
end

return Jomiel

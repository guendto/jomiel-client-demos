--
-- -*- coding: utf-8 -*-
--
-- jomiel-examples
--
-- Copyright
--  2021 Toni Gündoğdu
--
--
-- SPDX-License-Identifier: Apache-2.0
--

local function dumpConfig(opts)
  local YAML = require 'lyaml/init'
  io.write(YAML.dump({ opts }))
end

local function printZMQVersion()
  local ZMQ = require 'lzmq'
  print('ZeroMQ version ' .. table.concat(ZMQ.version(), '.'))
end

local Runner = {}

function Runner.main()
  local opts = require 'demo.options'.parse()
  if opts.print_config then
    dumpConfig(opts)
  elseif opts.version_zmq then
    printZMQVersion()
  else
    local Jomiel = require 'demo.jomiel'
    Jomiel:new(opts):inquire()
  end
end

return Runner

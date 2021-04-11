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

local function dump_config(opts)
  local lyaml = require 'lyaml/init'
  io.write(lyaml.dump({ opts }))
end

local function print_zmq_version()
  local zmq = require 'lzmq'
  print('ZeroMQ version ' .. table.concat(zmq.version(), '.'))
end

local Runner = {}

function Runner.main()
  local opts = require 'demo.options'.parse()
  if opts.print_config then
    dump_config(opts)
  elseif opts.version_zmq then
    print_zmq_version()
  else
    local Jomiel = require 'demo.jomiel'
    Jomiel:new(opts):inquire()
  end
end

return Runner

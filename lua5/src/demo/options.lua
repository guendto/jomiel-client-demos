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

local Options = {}

function Options.parse()
  local parser = require 'argparse' 'demo'

  parser:flag('-D --print-config',
    'Print confuguration values and exit')

  parser:flag('-V --version-zmq',
    'Display ZeroMQ version and exit', false)

  parser:flag('-j --output-json',
    'Print dumped messages in JSON', false)

  parser:flag('-c --compact-json',
    'Use more compact representation of JSON', false)

  parser:option('-r --router-endpoint',
    'Specify the outer endpoint address',
    'tcp://localhost:5514')

  parser:option('-t --connect-timeout',
    'Specify maximum time in seconds for the connection allowed to take',
    30)

  parser:flag('-q --be-terse',
    'Be brief and to the point; dump interesting details only', false)

  parser:option('-p --proto-dir',
    'Specify path to the dir with the .proto files',
    '../proto'
  )

  parser:argument('URI'):args('*')

  return parser:parse()
end

return Options

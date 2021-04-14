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
  local Parser = require 'argparse' 'demo'

  Parser:flag('-D --print-config',
    'Print confuguration values and exit')

  Parser:flag('-V --version-zmq',
    'Display ZeroMQ version and exit', false)

  Parser:flag('-j --output-json',
    'Print dumped messages in JSON', false)

  Parser:flag('-c --compact-json',
    'Use more compact representation of JSON', false)

  Parser:option('-r --router-endpoint',
    'Specify the outer endpoint address',
    'tcp://localhost:5514')

  Parser:option('-t --connect-timeout',
    'Specify maximum time in seconds for the connection allowed to take',
    30)

  Parser:flag('-q --be-terse',
    'Be brief and to the point; dump interesting details only', false)

  Parser:option('-p --proto-dir',
    'Specify path to the dir with the .proto files',
    '../proto'
  )

  Parser:argument('URI'):args('*')

  return Parser:parse()
end

return Options

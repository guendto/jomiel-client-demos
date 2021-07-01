#!/usr/bin/env lua
--
-- -*- coding: utf-8 -*-
--
-- jomiel-client-demos
--
-- Copyright
--  2021 Toni Gündoğdu
--
--
-- SPDX-License-Identifier: Apache-2.0
--
package.path  = package.path  .. ';./src/?.lua'
package.path  = package.path  .. ';local/share/lua/5.3/?.lua'

package.cpath = package.cpath .. ';local/lib/lua/5.3/?.so'
require 'demo.runner'.main()

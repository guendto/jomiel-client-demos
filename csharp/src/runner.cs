/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

using System.Collections.Generic;
using System;

using ZeroMQ.lib;
using DocoptNet;

namespace Demo {

    using Options = IDictionary<string, DocoptNet.ValueObject>;

    internal class Runner {

        private const string _usage =
    @"Usage:
    demo [-hDVjcq] [-r <addr>] [-t <time>] [URI ...]

Options:
     -h --help                      Print this help and exit
     -D --print-config              Print configuration values and exit
     -V --version-zmq               Print ZeroMQ version and exit
     -j --output-json               Print dumped messages in JSON
     -c --compact-json              Use more compact representation of JSON
     -r --router-endpoint <addr>    Specify the router endpoint address
                                     [default: tcp://localhost:5514]
     -t --connect-timeout <time>    Specify maximum time in seconds for
                                     the connection allowed to take
                                     [default: 30]
     -q --be-terse                  Be brief and to the point; dump
                                     interesting details only";

        private Options _opts;

        internal Runner (string [] args)
        {
            _opts = new Docopt ().Apply (_usage, args, exit: true);
        }

        internal int main ()
        {
            if (_opts ["--print-config"].IsTrue) {
                dumpConfig ();
            } else if (_opts ["--version-zmq"].IsTrue) {
                printZmqVersion ();
            } else {
                new Jomiel (_opts).inquire ();
            }
            return 0;
        }

        private void dumpConfig ()
        {
            Console.WriteLine ("---");
            foreach (var opt in _opts) {
                var name = opt.Key.Replace ("--", "");
                Console.WriteLine ("{0}: {1}", name, opt.Value);
            }
        }

        private void printZmqVersion ()
        {
            int mjr, mnr, ptc;
            zmq.version (out mjr, out mnr, out ptc);
            Console.WriteLine ("ZeroMQ version {0}.{1}.{2}", mjr, mnr, ptc);
        }

    } // class Runner

} // namespace Demo

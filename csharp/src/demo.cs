/*
 * -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

using System;

using Google.Protobuf;
using DocoptNet;

using ZeroMQ.lib;
using ZeroMQ;

[assembly: log4net.Config.XmlConfigurator(ConfigFile = "log4net.config")]

namespace Demo {

internal class Program {
    private static readonly log4net.ILog log =
        log4net.LogManager.GetLogger(
            System.Reflection.MethodBase.GetCurrentMethod().DeclaringType
        );
    private const string usage =
        @"Usage:
    demo [-hDVq] [-r <addr>] [-t <time>] [URI ...]

Options:
    -h --help                       Print this help and exit
    -D --print-config               Print configuration values and exit
    -V --version-zmq                Print ZeroMQ version and exit
    -r --router-endpoint <addr>     Specify the router endpoint address
                                     [default: tcp://localhost:5514]
    -t --connect-timeout <time>     Specify maximum time in seconds for
                                     the connection allowed to take
                                     [default: 30]
    -q --be-terse                   Be brief and to the point; dump
                                     interesting details only

";
    private static void Main(string[] args) {
        var opts = new Docopt().Apply(usage, args, exit: true);

        if (opts["--print-config"].IsTrue) {
            Console.WriteLine("---");
            foreach (var argument in opts) {
                Console.WriteLine("{0}: {1}", argument.Key.Replace("--", ""),
                                  argument.Value);
            }
            System.Environment.Exit(1);
        } else if (opts["--version-zmq"].IsTrue) {
            int major, minor, patch;
            zmq.version(out major, out minor, out patch);
            Console.WriteLine("ZeroMQ version {0}.{1}.{2}",
                              major, minor, patch);
            System.Environment.Exit(1);
        }

        var inputUri = opts["URI"].AsList;

        log4net.Config.XmlConfigurator.Configure();
        var log = log4net.LogManager.GetLogger("demo");

        if (inputUri.Count == 0) {
            log.Error("no input URI given");
            System.Environment.Exit(1);
        }

        var jomiel = new Jomiel(opts);
        jomiel.connect();

        foreach (var uri in inputUri) {
            jomiel.inquire(uri.ToString());
        }
    }
}

} // end of namespace

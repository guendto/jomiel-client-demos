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

const char *usage =
    R"(Usage:
    demo [-hDVqj] [-r <addr>] [-t <time>] [URI ...]

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
    -j --output-json                Print dumped messages in JSON

)";

// vim: set ts=2 sw=2 tw=72 expandtab:

<?php
/* -*- coding: utf-8 -*-
 *
 * jomiel-examples
 *
 * Copyright
 *  2019 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

namespace Demo;

use GetOptionKit\OptionPrinter\ConsoleOptionPrinter;
use GetOptionKit\OptionCollection;
use GetOptionKit\OptionParser;

function opts_init($argv) {
    $opts = new OptionCollection;

    $opts->add("h|help", "Print help and exit");

    $opts->add("V|version-zmq", "Display ZeroMQ version and exit");

    $opts->add("r|router-endpoint:",
        "Specify the rounter endpoint address")
        ->isa("string")
        ->defaultValue("tcp://localhost:5570")
        ;

    $opts->add("t|connect-timeout:",
        "Specify maximum time in seconds for the connection allowed to take")
        ->isa("number")
        ->defaultValue(30)
        ;

    $opts->add("q|be-terse",
        "Be brief and to the point; dump interesting details only");

    $parser = new OptionParser($opts);
    try {
        $result = $parser->parse($argv);
        // --help
        if ($result->help) {
            $printer = new ConsoleOptionPrinter;
            echo "Usage:\n";
            echo "    demo [options] <uri> ...\n\n";
            echo $printer->render($opts);
            die;
        // --version-zmq
        } else if ($result->versionZmq) {
            echo "ZeroMQ version " . ZMQ::LIBZMQ_VERSION . " ("
                . ZMQ::LIBZMQ_VERSION_ID . ")\n";
            die;
        }
        $args = $result->getArguments();
    } catch (Exception $e) {
        echo $e->getMessage();
        die;
    }
    return $result;
}

?>

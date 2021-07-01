<?php
/* -*- coding: utf-8 -*-
 *
 * jomiel-client-demos
 *
 * Copyright
 *  2021 Toni Gündoğdu
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

namespace Demo\Runner;

use \ZMQ;
use Docopt\Response as DocoptResponse;
use function Demo\Options\parse;
use function Demo\Util\pretty_dump;
use Demo\Jomiel as Jomiel;

function main(): void
{
    $opts = parse();
    if ($opts["--print-config"]) {
        dump_config($opts);
    } elseif ($opts["--version-zmq"]) {
        print_zmq_version();
    } else {
        (new Jomiel($opts))->inquire();
    }
}

function dump_config(DocoptResponse $opts): void
{
    echo "---\n";
    foreach ($opts as $k => $v) {
        $k = str_replace("--", "", $k);
        $v = pretty_dump($v);
        printf("$k: $v\n");
    }
}

function print_zmq_version(): void
{
    printf("ZeroMQ version %s\n", ZMQ::LIBZMQ_VERSION);
}

?>

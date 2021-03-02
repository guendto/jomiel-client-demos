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

use Monolog\Formatter\LineFormatter;
use Monolog\Handler\StreamHandler;
use Monolog\Logger;

function log_init()
{
    function new_handler()
    {
        function new_formatter()
        {
            $fmt = "[%level_name%] status: %message%\n";
            return new LineFormatter($fmt);
        }
        $hdlr = new StreamHandler("php://stderr", Logger::DEBUG);
        $hdlr->setFormatter(new_formatter());
        return $hdlr;
    }

    $lg = new Logger("demo");
    $lg->pushHandler(new_handler());

    return $lg;
}
?>

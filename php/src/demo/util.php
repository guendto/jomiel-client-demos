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

namespace Demo\Util;

function pretty_dump($value): string
{
    return json_encode(
        $value,
        JSON_INVALID_UTF8_SUBSTITUTE |
            JSON_PARTIAL_OUTPUT_ON_ERROR |
            JSON_UNESCAPED_SLASHES |
            JSON_UNESCAPED_UNICODE |
            JSON_PRETTY_PRINT
    );
}

?>

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

workspace "demo"
    configurations { "release", "debug" }
    location "build"

project "demo"
    kind "ConsoleApp"
    language "C#"

    targetdir "dist"
    clr "On"

    defines {
        "GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE"
    }

    files {
        "src/proto/Message.cs",
        "src/proto/Status.cs",
        "src/proto/Media.cs",
        "src/jomiel.cs",
        "src/runner.cs",
        "src/app.cs"
    }

    filter "configurations:debug"
        defines { "TRACE", "DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"

-- -*- coding: utf-8 -*-
--
-- jomiel-examples
--
-- Copyright
--  2019-2020 Toni Gündoğdu
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

    files {
        "src/proto/Message.cs",
        "src/proto/Status.cs",
        "src/proto/Media.cs",
        "src/Jomiel.cs",
        "src/demo.cs"
    }

    libdirs {
        "./packages/Google.Protobuf/lib/net45/",
        "./packages/log4net/lib/net40-client/",
        "./packages/docopt.net/lib/net40/",
        "./packages/ZeroMQ/lib/net40/"
    }

    links {
        "Google.Protobuf.dll",
        "DocoptNet.dll",
        "log4net.dll",
        "ZeroMQ.dll"
    }

    filter "configurations:debug"
        defines { "TRACE", "DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"

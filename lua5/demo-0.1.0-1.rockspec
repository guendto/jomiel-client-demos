package = "demo"
version = "0.1.0-1"
source = {
  url = "https://github.com/guendto/jomiel-client-demos"
}
description = {
  license = "Apache-2.0"
}
dependencies = {
  "lua >= 5.3, < 5.4",
  "argparse >= 0.7.1-1",
  "dkjson >= 2.5-2",
  "inspect >= 3.1.1-0",
  "lua-protobuf >= 0.3.2-0",
  "lyaml >= 6.2.7-1",
  "lzmq >= 0.4.4-1",
  "middleclass >= 4.1.1-0",
}
build = {
  type = "builtin",
  modules = {
    demo = "src/demo.lua",
    ["demo.options"] = "src/demo/options.lua",
    ["demo.runner"] = "src/demo/runner.lua",
    ["demo.jomiel"] = "src/demo/jomiel.lua"
  }
}

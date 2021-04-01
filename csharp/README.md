# C# example

## Building and running

```shell
../proto/bin/bootstrap -p ../proto/ -l csharp -d src/proto/
premake5 --dotnet=mono vs2019
mono paket.exe install
msbuild build/demo.sln
```

```shell
mono dist/demo.exe [args...]
```

Or:

```shell
mkbundle -o demo --simple demo.exe --no-machine-config --no-config
./demo [args ...]
```

See [paket], [premake]5, [msbuild] and [mkbundle].

[mkbundle]: https://www.mono-project.com/docs/tools+libraries/tools/mkbundle/
[msbuild]: https://github.com/mono/msbuild
[paket]: https://fsprojects.github.io/Paket/
[premake]: https://premake.github.io/

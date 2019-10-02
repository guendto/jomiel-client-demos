# C# example

## Building and running

```shell
mono paket.exe install
../bin/bootstrap -p ../proto/ -l csharp -d src/proto/
premake5 gmake2
make
mono demo.exe [args...]
```

### Resources

- [Premake5][2]
- [Paket][1]

[1]: https://fsprojects.github.io/Paket/
[2]: https://premake.github.io/

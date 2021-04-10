# Perl5 example

## Notable differences to other examples

- Output defaults to YAML

Not implemented:

- `--connect-timeout`

## Building and running

```shell
cpanm --notest --quiet Carton
carton install --without develop
./bin/demo --static-gen --proto-dir ../proto
./bin/demo [args ...]
```

See [carton].

[carton]: https://metacpan.org/pod/Carton

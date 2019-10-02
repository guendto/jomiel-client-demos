# jomiel-examples

`jomiel-examples` is a selection of examples written in a number of
_different modern programming languages_. These examples show how the
[jomiel][1] inquiries are made and responses are handled in different
languages.

**Notes**

- The examples are not necessarily identical to one another

## Dockerfiles

Each language directory contains a dockerfile (some more than one).

```shell
docker build -t YOUR_TAGNAME -f PATH_TO_DOCKERFILE .
docker run --network host --rm -it YOUR_TAGNAME [args...]
```

For example:

```shell
docker build -t tg/jomiel-examples/c-example -f c/docker/alpine/Dockerfile .
docker run --network host --rm -it tg/jomiel-examples/c-example foo
```

## License

`jomiel-examples` is licensed under the [Apache License version 2.0][2]
(APLv2).

[1]: https://github.com/guendto/jomiel
[2]: https://tldrlegal.com/license/apache-license-2.0-(apache-2.0)

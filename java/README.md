# jomiel client demo written in Java

## Building and running

### Gradle

```shell
gradle run -q --args="[args...]"
```

Or:

```shell
gradle build
tar xf build/distributions/demo.tar
./demo/bin/demo [args...]
```

### Maven

```shell
mvn compile exec:java -Dexec.args="[args...]"
```

Or:

```shell
mvn package
java -jar target/<jarfile> [args...]
```

# Java example

## Building and running

### Gradle

```shell
gradle run --args="[args...]"
```

Or:

```shell
gradle shadowJar
java -jar build/libs/<jarfile> [args...]
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

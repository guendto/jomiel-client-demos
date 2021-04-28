# REST-API (Express) example

- Make sure you have installed [jomiel] and that it is running at
  <tcp://localhost:5514>

- Install [httpie] if you haven't, these examples will use it
  - You can use alternatives (not covered here), e.g. `curl`

- Re-configure by editing the `.env` file

[jomiel]: https://github.com/guendto/jomiel
[httpie]: https://httpie.io/

## Building

With npm:

```shell
npm i               # Install deps
npm run gen:static  # Compile protobuf declarations for jomiel messages
```

With [pnpm]:

```shell
pnpm i
pnpm gen:static
```

[pnpm]: https://pnpm.io/

## Running

Start the service.

```shell
node .
```

## POST request for an URL

```shell
http :3001/inquiry url=URL
```

For example:

```shell
http :3001/inquiry url=https://youtu.be/PRdcZSuCpNo
```

## GET request for a Youtube video

```shell
http :3001/inquiry/tube/:id
```

For example:

```shell
http :3001/inquiry/tube/PRdcZSuCpNo
```

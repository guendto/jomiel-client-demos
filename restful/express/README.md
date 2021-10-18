# Express demo

- Install [jomiel], run it at <tcp://localhost:5514>
- Install [httpie] or use an alternative, e.g. `curl`
- Edit `.env` file to configure

[jomiel]: https://github.com/guendto/jomiel
[httpie]: https://httpie.io/

## Building and running

```shell
npm install
npm run dev
```

## POST /api/by-url

```shell
http :3001/api/by-url inputUri=URL
# e.g.:
http :3001/api/by-url inputUri=https://youtu.be/PRdcZSuCpNo
```

## GET /api/yt/by-id specifically for YouTube videos

```shell
http :3001/api/yt/by-id/:id
# e.g.:
http :3001/api/yt/by-id/PRdcZSuCpNo
```

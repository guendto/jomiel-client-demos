# jomiel REST-API demo written in Javascript for Express

- Install [jomiel], run it at <tcp://localhost:5514>
- Install [httpie] or use an alternative, e.g. `curl`
- Configure via the `.env` file

[jomiel]: https://github.com/guendto/jomiel
[httpie]: https://httpie.io/

## Building and running

```shell
npm install
npm run start
```

## POST request for an URL

```shell
http :3001/inquiry url=URL
# e.g.:
http :3001/inquiry url=https://youtu.be/PRdcZSuCpNo
```

## GET request for a Youtube video

```shell
http :3001/inquiry/tube/:id
# e.g.:
http :3001/inquiry/tube/PRdcZSuCpNo
```

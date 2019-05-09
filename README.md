## Demo

![gif](https://media.giphy.com/media/QmKwn3tsLTYbacKes5/giphy.gif)

[Keynote](http://bit.ly/OGalaga)

### Install

```
npm install
```

### If you are using OPAM, run these two commands

```
opam switch create 4.02.3+buckle-master && eval $(opam env)
opam switch 4.02.3+buckle-master && eval $(opam env)
```

### Build

```
npm run build
```

### Start

```
npm start
```

To build to JS run `npm run build:web` and then run a static server, like `python -m SimpleHTTPServer` and go to `localhost:8000`. If you're using safari you can simply open the `index.html` and tick `Develop > Disable Cross-Origin Restrictions`.

To build to native run `npm run build:native` and run `npm run start:native`

The build system used is [bsb-native](https://github.com/bsansouci/bucklescript).

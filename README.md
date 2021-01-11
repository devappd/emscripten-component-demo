# emscripten-component-demo

Demos that show how to use [Emscripten](https://github.com/emscripten-core/emscripten) as a reusable component in modern web frameworks such as React.

See [emscripten#13151](https://github.com/emscripten-core/emscripten/issues/13151) for discussion.

## Demos

1. ~~React~~
2. ~~Vue.js~~
3. [Svelte](https://github.com/devappd/emscripten-component-demo/tree/master/Demo-03-Svelte)

## Install

Clone this project to start experimenting with each demo:

```sh
git clone https://github.com/devappd/emscripten-component-demo
```

Before calling `npm install` on these projects, you must have Python 3.6+ installed on your system. You may download it at [python.org](https://www.python.org/downloads/), or refer to your OS's package manager.

When you call `npm run build`, each project will use the [emscripten-build](https://github.com/devappd/emscripten-build-npm) package to install the Emscripten SDK, CMake, and Ninja prerequisites. You should configure a custom install path for the Emscripten SDK, or else it will
be duplicated into each project. If you already have the SDK installed, you may specify it as the path.

Specify a custom path by
[modifying your NPM config](https://docs.npmjs.com/cli/v6/using-npm/config) via one of the following:

|Method|Command
|------|-------
| Commit the path to your project `.npmrc` | `npm config --userconfig "./.npmrc" set emsdk "/your/install/path"`
| Commit the path to your user `.npmrc` | `npm config set emsdk "/your/install/path"`
| Set an environment variable | `set NPM_CONFIG_EMSDK=/your/install/path`
| Use a config argument to NPM temporarily | `npm [command] --emsdk="/your/install/path"`

## License

The JS project code is released under MIT License.

However, the C++ code is sourced from [erik-larsen/emscripten-sdl2-ogles2](https://github.com/erik-larsen/emscripten-sdl2-ogles2) which does not specify a license.

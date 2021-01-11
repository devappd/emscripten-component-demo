const path = require('path');

const srcPath = path.join(__dirname, 'cpp', 'src');
const buildPath = path.join(__dirname, 'cpp', 'build');
const distPath = path.join(__dirname, 'cpp', 'wasm');

module.exports = {
  default: 'module',

  module: {
    type: 'cmake',

    configure: {
      path: srcPath,
      generator: 'Ninja',
      type: 'Release',

      definitions: {}
    },

    build: {
      path: `${buildPath}-module`
    },

    install: {
      path: `${distPath}-module`
    }
  },

  worker: {
    type: 'cmake',

    configure: {
      path: srcPath,
      generator: 'Ninja',
      type: 'Release',

      definitions: {
        'PROXY_TO_WORKER': true
      }
    },

    build: {
      path: `${buildPath}-worker`
    },

    install: {
      path: `${distPath}-worker`
    }
  }
};

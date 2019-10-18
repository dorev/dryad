const path = require('path');

module.exports = {
  entry: './out/main.js',
  output: {
    filename: 'dryad.js',
    path: path.resolve('dist')
  },
};
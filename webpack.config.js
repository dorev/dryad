const path = require('path');

module.exports = {
  entry: './out/_main.js',
  output: {
    filename: 'dryad.js',
    path: path.resolve('dist')
  },
};
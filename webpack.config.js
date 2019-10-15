const path = require('path');

module.exports = {
  entry: './js/main.js',
  output: {
    filename: 'dryad.js',
    path: path.resolve('dist')
  },
};
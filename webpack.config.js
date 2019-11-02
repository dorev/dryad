const path = require('path');

module.exports = {
  entry: './out/_main.js',
  devtool: 'inline-source-map',
  mode : "development",
  output: {
    filename: 'dryad.js',
    path: path.resolve('dist')
  },
};
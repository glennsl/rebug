const path = require('path');

module.exports = {
  entry: './lib/js/examples/index.js',
  output: {
    path: path.join(__dirname, 'build'),
    filename: 'index.js',
  }
};
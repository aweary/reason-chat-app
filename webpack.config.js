const path = require('path')
const HtmlWebpackPlugin = require('html-webpack-plugin')

module.exports = {
  entry: path.resolve('./lib/js/src/app.js'),
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'bundle.[chunkhash].js',
  },
  module: {
    rules: [
      {
        test: /\.jsx?$/,
        loader: 'babel-loader',
        include: path.resolve('./lib/js'),
        exclude: path.resolve('node_modules'),
      },
      {
        test: /\.css/,
        use: ['style-loader', 'css-loader'],
      },
    ],
  },
  plugins: [new HtmlWebpackPlugin({ template: './index.ejs' })],
}

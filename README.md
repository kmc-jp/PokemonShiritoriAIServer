# ポケモンしりとりAIサーバー

Pepperくんのポケモンしりとりで用いている、 ポケモンしりとりのAIサーバーです
proxy.rbがPepperくんとAIコアの中継をする役割を果たしています。
main.rbがAIの本体です。

# Install and Run

```
git clone https://github.com/kmc-jp/PokemonShiritoriAIServer.git
cd PokemonShiritoriAIServer
make
bundle install --path=vendor/bundle
bundle exec ruby proxy.rb
```

# Algorithm

αβ法を用いて勝敗判定、敗北確定の場合は最短の負け手数が最長になるような手を選びます。

# TODO

コードを整理する
ドキュメントを整備する

# License

MIT License

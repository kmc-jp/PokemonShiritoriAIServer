#include <array>
#include <iostream>
#include <string>

#include <boost/timer.hpp>

constexpr int kana_num = 44;
constexpr int value_max = 100000;

using table = std::array<std::array<int, kana_num>, kana_num>;

table input() {
  table tb;
  for (int i = 0; i < kana_num; ++i)
    for (int j = 0; j < kana_num; ++j)
      std::cin >> tb[i][j];
  return tb;
}

table erase_loop(table tb) {
  for (int i = 0; i < kana_num; ++i) {
    for (int j = 0; j < kana_num; ++j) {
      if (i != j) {
        int diff = std::min(tb[i][j], tb[j][i]);
        tb[i][j] -= diff;
        tb[j][i] -= diff;
      } else {
        tb[i][i] -= (tb[i][i] / 2) * 2;
      }
    }
  }
  return tb;
}

int score(const table &tb, int pos) {
  int sum = 0;
  for (int i = 0; i < kana_num; ++i)
    sum += tb[pos][i];
  return sum ? sum : -value_max;
}

int alphabeta(const table &tb, int pos, int depth, int alpha, int beta) {
  if (depth == 0) return score(tb, pos);
  for (int i = 0; i < kana_num; ++i) {
    if (tb[pos][i] != 0) {
      auto cp = tb;
      --cp[pos][i];
      alpha = std::max(alpha,
          -alphabeta(cp, i, depth - 1, -beta, -alpha));
      if (alpha >= beta) return alpha;
    }
  }
  return alpha;
}

int main(int argc, char **argv) {
  int pos;
  std::cin >> pos;
  auto tb = input();
  auto el = erase_loop(tb);
  int next_pos = 0;
  boost::timer tm;
  for (int depth = 3; depth < 800 && tm.elapsed() < 1.0; ++depth) {
    int alpha = -value_max;
    int beta = value_max;
    for (int i = 0; i < kana_num; ++i) {
      if (tb[pos][i] != 0) {
        auto cp = el;
        if (cp[pos][i])
          --cp[pos][i];
        else
          ++cp[i][pos];
        int s = -alphabeta(cp, i, depth, -beta, -alpha);
        if (s >= alpha) {
          alpha = s;
          next_pos = i;
        }
      }
    }
    std::cerr << "depth: " << depth << ", score: " << alpha << std::endl;
  }
  std::cout << next_pos << std::endl;
  return 0;
}

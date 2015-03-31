#include <array>
#include <iostream>
#include <string>

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

int score(const table &tb, int pos) {
  int sum = 0;
  for (int i = 0; i < kana_num; ++i)
    sum += tb[pos][i];
  return sum ? sum : -value_max;
}

int minimax(const table &tb, int pos, int depth) {
  if (depth == 0) return score(tb, pos);
  int score_max = -value_max;
  for (int i = 0; i < kana_num; ++i) {
    if (tb[pos][i] != 0) {
      auto cp = tb;
      --cp[pos][i];
      score_max = std::max(score_max, -minimax(cp, pos, depth - 1));
    }
  }
  return score_max;
}

int main(int argc, char **argv) {
  int pos;
  std::cin >> pos;
  auto tb = input();
  int score_max = -value_max;
  int next_pos = 0;
  for (int i = 0; i < kana_num; ++i) {
    if (tb[pos][i] != 0) {
      auto cp = tb;
      --cp[pos][i];
      int s = -minimax(cp, pos, 4);
      if (s > score_max) {
        score_max = s;
        next_pos = i;
      }
    }
  }
  std::cerr << score_max << std::endl;
  std::cout << next_pos << std::endl;
  return 0;
}

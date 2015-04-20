#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include <boost/timer.hpp>

constexpr int kana_num = 44;
constexpr int value_max = 100000;

using table = std::array<std::array<int, kana_num>, kana_num>;
using sumt = std::array<int, kana_num>;

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
        tb[i][i] = tb[i][i] % 2;
      }
    }
  }
  return tb;
}

sumt sum_table(table tb) {
  sumt s;
  std::fill(std::begin(s), std::end(s), 0);
  for (int i = 0; i < kana_num; ++i)
    for (int j = 0; j < kana_num; ++j)
      s[i] += tb[i][j];
  return s;
}

int score(const sumt &st, int pos, int) {
  return st[pos] ? 0 : -value_max;
}

int score2(const sumt &st, int pos, int co_depth) {
  return (st[pos] ? 0 : -value_max) * std::pow(0.99, (double)co_depth);
}

template <typename Func>
int alphabeta(const table &tb, const sumt &st, int pos, int depth,
    int alpha, int beta, Func func, int co_depth = 0) {
  if (depth == 0) return func(st, pos, co_depth);
  for (int i = 0; i < kana_num; ++i) {
    if (tb[pos][i] != 0) {
      auto cp = tb;
      auto cps = st;
      --cp[pos][i];
      --cps[pos];
      alpha = std::max(alpha,
          -alphabeta(cp, cps, i, depth - 1, -beta, -alpha, func, co_depth + 1));
      if (alpha >= beta) return alpha;
    }
  }
  return alpha;
}

template <typename Func>
int alphabeta2(const table &tb, const sumt &st, int pos, int depth,
    int alpha, int beta, Func func, int co_depth = 0) {
  if (depth == 0) return func(st, pos, co_depth);
  bool is_lose = true;
  for (int i = 0; i < kana_num; ++i) {
    if (tb[pos][i] != 0) {
      auto cp = tb;
      auto cps = st;
      is_lose = false;
      --cp[pos][i];
      --cps[pos];
      alpha = std::max(alpha,
          -alphabeta2(cp, cps, i, depth - 1, -beta, -alpha,
              func, co_depth + 1));
      if (alpha >= beta) return alpha;
    }
  }
  if (is_lose)
    return std::max(alpha,
        int(-value_max * std::pow(0.99, (double)co_depth)));
  else
    return alpha;
}

std::vector<int> mate_or_escape(const table &tb, const sumt &st, int pos) {
  std::vector<int> next_pos;
  boost::timer tm;
  for (int depth = 0; depth < 800 && tm.elapsed() < 1.0; ++depth) {
    next_pos.clear();
    int alpha = -value_max;
    for (int i = 0; i < kana_num; ++i) {
      if (tb[pos][i] != 0) {
        auto cp = tb;
        auto cps = st;
        --cp[pos][i];
        --cps[pos];
        int s = -alphabeta2(cp, cps, i, depth, -value_max, value_max, score2);
        if (s >= alpha) {
          if (s > alpha) {
            alpha = s;
            next_pos.clear();
          }
          next_pos.push_back(i);
        }
      }
    }
    std::cerr << "depth: " << depth << ", score: " << alpha << std::endl;
  }
  return next_pos;
}

int main(int argc, char **argv) {
  std::random_device rd;
  std::mt19937 mt(rd());
  int pos;
  std::cin >> pos;
  auto tb = input();
  auto st = sum_table(tb);
  auto el = erase_loop(tb);
  auto es = sum_table(el);
  std::vector<int> next_pos;
  boost::timer tm;
  bool to_mate = false;
  for (int depth = 0; depth < 800 && tm.elapsed() < 1.0; ++depth) {
    next_pos.clear();
    int alpha = -value_max;
    for (int i = 0; i < kana_num; ++i) {
      if (tb[pos][i] != 0) {
        auto cp = el;
        auto cps = es;
        if (cp[pos][i]) {
          --cp[pos][i];
          --cps[pos];
        } else {
          ++cp[i][pos];
          ++cps[i];
        }
        int s = -alphabeta(cp, cps, i, depth, -value_max, value_max, score);
        if (s >= alpha) {
          if (s > alpha) {
            alpha = s;
            next_pos.clear();
          }
          next_pos.push_back(i);
        }
      }
    }
    std::cerr << "depth: " << depth << ", score: " << alpha << std::endl;
    if (alpha == -value_max)
      to_mate = true;
  }
  if (to_mate)
    next_pos = mate_or_escape(tb, st, pos);
  std::uniform_int_distribution<> dis(0, next_pos.size() - 1);
  std::cout << next_pos[dis(mt)] << std::endl;
  return 0;
}

#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct {
  std::string dst;
  size_t cost;
} transitions;

typedef struct {
  std::string name;
  std::vector<transitions> edges;
} state;

class Map {
private:
  std::vector<state> states;

  // Para achar os indices de cada estano no vetor em O(1)
  std::unordered_map<std::string, size_t> index;
  size_t find_state(const std::string &state_name) const;

public:
  size_t add_state(const std::string &state_name);
  void add_transitions(const std::string &src, const std::string &dst,
                       size_t cost);
  const state *get_state(const std::string &state_name) const;

  void print_map() const;
};

Map romenia();

#pragma once

#include "map.hpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class Node {
public:
  std::string state;
  std::shared_ptr<Node> parent;
  std::string transition;
  size_t cost;

  Node(const std::string &state, std::shared_ptr<Node> parent = nullptr,
       const std::string &transition = "", size_t cost = 0)
      : state(state), parent(parent), transition(transition), cost(cost) {}

  std::vector<std::string> path() const {
    std::vector<std::string> seq;
    const Node *n = this;

    while (n != nullptr) {
      seq.push_back(n->state);
      n = n->parent.get();
    } // Adiciona os esatados do estado final até o inicial

    std::reverse(seq.begin(), seq.end()); // inverte para a ordem ficar correta
    return seq;
  }
};

// Buscas sem informacao
Node bfs(Map &map, std::string src, std::string dst);
Node dfs(Map &map, std::string src, std::string dst);
Node ucs(Map &map, std::string src, std::string dst);

// Buscas Heurísticas
using heuristic_func = std::function<int(const std::string &)>;

int romenia_heuristic(const std::string &city);
Node greedy_search(Map &map, std::string src, std::string dst, heuristic_func);
Node a_star(Map &map, std::string src, std::string dst, heuristic_func);

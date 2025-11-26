#include "../include/search_algs.hpp"
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

Node bfs(Map &map, std::string src, std::string dst) {
  auto root = std::make_shared<Node>(src, nullptr, "", 0);

  if (src == dst) {
    return *root;
  }

  std::queue<std::shared_ptr<Node>>
      borda; // Pode ser chamada de fronteira tmb, não lembro ao certo qual das
             // duas nomeclaturas a viviane mais usou em sala
  std::set<std::string> explorados;

  borda.push(root);
  explorados.insert(root->state);

  while (!borda.empty()) {
    auto current_node = borda.front();
    borda.pop();

    if (current_node->state == dst) {
      return *current_node;
    }

    if (current_node->state == dst) {
      return *current_node; // retorna uma cópia do nó atual
    }

    const state *current_state = map.get_state(current_node->state);
    if (!current_state) {
      throw std::runtime_error("(bfs) não possivel achar o nó atual");
      exit(1);
    }

    for (const auto &tr : current_state->edges) {
      if (explorados.find(tr.dst) == explorados.end()) {
        explorados.insert(tr.dst);

        auto child = std::make_shared<Node>(tr.dst, current_node, "",
                                            current_node->cost + tr.cost);

        borda.push(child);
      }
    }
  }

  throw std::runtime_error("(bfs) Nenhum caminho encontrado");
}

Node dfs(Map &map, std::string src, std::string dst) {
  auto root = std::make_shared<Node>(src, nullptr, "", 0);

  if (src == dst) {
    return *root;
  }

  std::stack<std::shared_ptr<Node>>
      borda; // Pode ser chamada de fronteira tmb, não lembro ao certo qual das
             // duas nomeclaturas a viviane mais usou em sala
  std::set<std::string> explorados;

  borda.push(root);
  explorados.insert(root->state);

  while (!borda.empty()) {
    auto current_node = borda.top(); // Stack pega o elemento do topo com top()
                                     // e remove com pop()
    borda.pop();

    if (current_node->state == dst) {
      return *current_node; // retorna uma cópia do nó atual
    }

    const state *current_state = map.get_state(current_node->state);
    if (!current_state) {
      throw std::runtime_error("(bfs) não possivel achar o nó atual");
      exit(1);
    }

    for (const auto &tr : current_state->edges) {
      if (explorados.find(tr.dst) == explorados.end()) {
        explorados.insert(tr.dst);

        auto child = std::make_shared<Node>(tr.dst, current_node, "",
                                            current_node->cost + tr.cost);

        borda.push(child);
      }
    }
  }

  throw std::runtime_error("(dfs) Nenhum caminho encontrado");
}

struct CompareNodeCost {
  bool operator()(const std::shared_ptr<Node> &a,
                  const std::shared_ptr<Node> &b) const {
    return a->cost > b->cost;
  }
};

Node ucs(Map &map, std::string src, std::string dst) {
  auto root = std::make_shared<Node>(src, nullptr, "", 0);
  if (src == dst) {
    return *root;
  }

  // Min heap, mas tem que implementar o comparator
  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>,
                      CompareNodeCost>
      borda;
  std::set<std::string> explorados;
  std::unordered_map<std::string, size_t> node_cost;

  node_cost[root->state] = 0;
  explorados.insert(root->state);
  borda.push(root);

  while (!borda.empty()) {
    auto current_node = borda.top();
    borda.pop();

    auto it_best = node_cost.find(current_node->state);
    if (it_best != node_cost.end() && current_node->cost > it_best->second) {
      continue; // Se o cainho do node_cost já for o melhor apenas ignora
    }

    if (current_node->state == dst) {
      return *current_node;
    }

    const state *current_state = map.get_state(current_node->state);
    if (!current_state) {
      throw std::runtime_error(
          "(ucs) Um estado buscado não foi encontrado no mapa");
    }

    for (const auto &tr : current_state->edges) {
      size_t new_cost = current_node->cost + tr.cost;
      size_t old_best;
      auto it = node_cost.find(tr.dst);
      if (it != node_cost.end()) {
        old_best = it->second;
      } else {
        old_best = std::numeric_limits<size_t>::max();
      }

      if (new_cost < old_best) {
        node_cost[tr.dst] = new_cost;
        auto child = std::make_shared<Node>(tr.dst, current_node, "", new_cost);
        borda.push(child);
      }
    }
  }

  throw std::runtime_error("(ucs) Nenhum caminho encontrado");
}

int romenia_heuristic(const std::string &city) {
  static const std::unordered_map<std::string, int> h = {
      {"Arad", 366},    {"Bucharest", 0},   {"Craiova", 160},
      {"Drobeta", 242}, {"Eforie", 161},    {"Fagaras", 176},
      {"Giurgiu", 77},  {"Hirsova", 151},   {"Iasi", 226},
      {"Lugoj", 244},   {"Mehadia", 241},   {"Neamt", 234},
      {"Oradea", 380},  {"Pitesti", 100},   {"Rimnicu Vilcea", 193},
      {"Sibiu", 253},   {"Timisoara", 329}, {"Urziceni", 80},
      {"Vaslui", 199},  {"Zerind", 374}};

  auto it = h.find(city);
  return it != h.end() ? it->second : 0;
}

struct CompareGreedy {
  heuristic_func heuristic;

  explicit CompareGreedy(heuristic_func h) : heuristic(std::move(h)) {}

  bool operator()(const std::shared_ptr<Node> &a,
                  const std::shared_ptr<Node> &b) const {

    // h(n) = heuristic(n)
    return heuristic(a->state) > heuristic(b->state);
  }
};

Node greedy_search(Map &map, std::string src, std::string dst,
                   heuristic_func heuristic) {
  auto root = std::make_shared<Node>(src, nullptr, "", 0); // g(n)=0

  if (src == dst)
    return *root;

  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>,
                      CompareGreedy>
      borda((CompareGreedy(heuristic))); // prioridade = h(n)

  std::set<std::string> visitados;

  borda.push(root);

  while (!borda.empty()) {
    auto atual = borda.top();
    borda.pop();

    if (visitados.count(atual->state))
      continue;
    visitados.insert(atual->state);

    if (atual->state == dst)
      return *atual;

    const state *st = map.get_state(atual->state);

    for (const auto &tr : st->edges) {
      if (visitados.count(tr.dst))
        continue;

      // g(n) = custo real acumulado
      size_t new_g = atual->cost + tr.cost;

      auto filho = std::make_shared<Node>(tr.dst, atual, "", new_g);

      // prioridade = h(n)
      borda.push(filho);

      /**
       * Quando retornar o valor real vai esta no node, mas o valor da heap vai
       * ser o da heuristica
       * */
    }
  }

  throw std::runtime_error("nenhum caminho");
}

struct CompareAStar {
  heuristic_func heuristic;

  explicit CompareAStar(heuristic_func h) : heuristic(std::move(h)) {}

  bool operator()(const std::shared_ptr<Node> &a,
                  const std::shared_ptr<Node> &b) const {

    size_t ga = a->cost;             // g(a) = custo real acumulado
    size_t ha = heuristic(a->state); // h(a) = heurística (linha reta)
    size_t fa = ga + ha;             // f(a) = g(a) + h(a)

    size_t gb = b->cost;             // g(b)
    size_t hb = heuristic(b->state); // h(b)
    size_t fb = gb + hb;             // f(b)

    return fa > fb; // Menor f(n) tem prioridade
  }
};

Node a_star(Map &map, std::string src, std::string dst,
            heuristic_func heuristic) {
  auto root = std::make_shared<Node>(src, nullptr, "", 0); // g(n)=0

  if (src == dst)
    return *root;

  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>,
                      CompareAStar>
      borda((CompareAStar(heuristic))); // prioridade = f(n)=g(n)+h(n)

  // melhor g(n) encontrado até agora
  std::unordered_map<std::string, size_t> best_g;
  best_g[src] = 0;

  std::set<std::string> fechados;

  borda.push(root);

  while (!borda.empty()) {
    auto atual = borda.top();
    borda.pop();

    if (fechados.count(atual->state))
      continue;
    fechados.insert(atual->state);

    if (atual->state == dst)
      return *atual;

    const state *st = map.get_state(atual->state);

    for (const auto &tr : st->edges) {

      // g(n) = custo real acumulado novo
      size_t new_g = atual->cost + tr.cost;

      auto it = best_g.find(tr.dst);

      // mantém somente o menor g(n)
      if (it == best_g.end() || new_g < it->second) {
        best_g[tr.dst] = new_g;

        auto filho = std::make_shared<Node>(tr.dst, atual, "", new_g);

        // prioridade = f(n) = g(n) + h(n)
        borda.push(filho);
      }
    }
  }

  throw std::runtime_error("nenhum caminho");
}

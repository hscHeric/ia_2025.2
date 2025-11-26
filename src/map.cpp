#include "../include/map.hpp"
#include <iostream>
#include <stdexcept>

size_t Map::find_state(const std::string &state_name) const {
  // Verifica se o estado existe usando o algoritmo find da stl, se não
  // encontrar lanca um erro
  auto it = index.find(state_name);
  if (it == index.end()) {
    throw std::runtime_error("Map::(find_state) Estado não encontrado: " +
                             state_name);
  }

  return it->second;
}

size_t Map::add_state(const std::string &state_name) {
  // Verifica se o estado já existe, se existir retorna o indice do estado no
  // vector, se não o adiciona e reotorna o indice
  auto it = index.find(state_name);
  if (it != index.end()) {
    return it->second;
  }

  state s;
  s.name = state_name;
  states.push_back(s); // Cria e adiciona o estado

  std::size_t idx = states.size() - 1;
  index[state_name] = idx;
  return idx; // Adiciona o indice do estado no map o retorna
}
void Map::add_transitions(const std::string &src, const std::string &dst,
                          size_t cost) {
  // Se não existirem o add_state cria os estados
  std::size_t i_src = add_state(src);
  std::size_t i_dst = add_state(dst);

  // Cria e adiciona a transicao, isso é feito inline usando {}
  states[i_src].edges.push_back({dst, cost});
  states[i_dst].edges.push_back({src, cost});
}
const state *Map::get_state(const std::string &state_name) const {
  auto it = index.find(state_name);
  if (it == index.end()) {
    throw std::runtime_error("(Map::get_state) Estado não encontrado: " +
                             state_name);
  }
  return &states[it->second];
}

void Map::print_map() const {
  std::cout << "Map com: " << states.size() << " states\n\n";

  for (const auto &s : states) {
    std::cout << "State: " << s.name << "\n";
    std::cout << "Transitions:\n";

    for (const auto &t : s.edges) {
      std::cout << "  -> " << t.dst << " (cost = " << t.cost << ")\n";
    }

    std::cout << "\n";
  }
}

Map romenia() {
  Map mapa;

  mapa.add_transitions("Arad", "Zerind", 75);
  mapa.add_transitions("Arad", "Sibiu", 140);
  mapa.add_transitions("Arad", "Timisoara", 118);
  mapa.add_transitions("Zerind", "Oradea", 71);
  mapa.add_transitions("Oradea", "Sibiu", 151);
  mapa.add_transitions("Timisoara", "Lugoj", 111);
  mapa.add_transitions("Lugoj", "Mehadia", 70);
  mapa.add_transitions("Mehadia", "Drobeta", 75);
  mapa.add_transitions("Drobeta", "Craiova", 120);
  mapa.add_transitions("Craiova", "Rimnicu Vilcea", 146);
  mapa.add_transitions("Craiova", "Pitesti", 138);
  mapa.add_transitions("Sibiu", "Fagaras", 99);
  mapa.add_transitions("Sibiu", "Rimnicu Vilcea", 80);
  mapa.add_transitions("Rimnicu Vilcea", "Pitesti", 97);
  mapa.add_transitions("Fagaras", "Bucharest", 211);
  mapa.add_transitions("Pitesti", "Bucharest", 101);
  mapa.add_transitions("Bucharest", "Giurgiu", 90);
  mapa.add_transitions("Bucharest", "Urziceni", 85);
  mapa.add_transitions("Urziceni", "Hirsova", 98);
  mapa.add_transitions("Urziceni", "Vaslui", 142);
  mapa.add_transitions("Hirsova", "Eforie", 86);
  mapa.add_transitions("Vaslui", "Iasi", 92);
  mapa.add_transitions("Iasi", "Neamt", 87);

  return mapa;
}

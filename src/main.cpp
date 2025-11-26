#include "../include/map.hpp"
#include "../include/search_algs.hpp"
#include <iostream>

inline void print_path(const std::vector<std::string> &path) {
  if (path.empty()) {
    std::cout << "Caminho vazio\n";
    return;
  }

  // Calcula tamanho total aproximado para evitar realocações
  std::size_t total_len = 0;
  for (const auto &s : path) {
    total_len += s.size() + 4; // " -> " tem 4 chars
  }

  std::string buffer;
  buffer.reserve(total_len);

  buffer.append(path[0]);
  for (std::size_t i = 1; i < path.size(); ++i) {
    buffer.append(" -> ");
    buffer.append(path[i]);
  }
  buffer.push_back('\n');

  std::cout << buffer;
}

int main(int argc, char *argv[]) {
  Map romenia_map = romenia();

  // origem/destino padrão, ou lidos da linha de comando
  const std::string src = (argc > 1) ? argv[1] : "Arad";
  const std::string dst = (argc > 2) ? argv[2] : "Bucharest";

  std::cout << "Origem: " << src << "\nDestino: " << dst << "\n\n";

  // BFS
  auto bfs_node = bfs(romenia_map, src, dst);
  auto bfs_path = bfs_node.path();

  std::cout << "[BFS] Caminho encontrado: ";
  print_path(bfs_path);
  std::cout << "[BFS] Custo total: " << bfs_node.cost << "\n\n";

  // DFS
  auto dfs_node = dfs(romenia_map, src, dst);
  auto dfs_path = dfs_node.path();

  std::cout << "[DFS] Caminho encontrado: ";
  print_path(dfs_path);
  std::cout << "[DFS] Custo total: " << dfs_node.cost << "\n\n";

  // UCS
  auto ucs_node = ucs(romenia_map, src, dst);
  auto ucs_path = ucs_node.path();

  std::cout << "[UCS] Caminho encontrado: ";
  print_path(ucs_path);
  std::cout << "[UCS] Custo total: " << ucs_node.cost << "\n\n";

  auto greedy_node = greedy_search(romenia_map, src, dst, romenia_heuristic);
  auto greedy_path = greedy_node.path();

  std::cout << "[greedy] Caminho encontrado: ";
  print_path(greedy_path);
  std::cout << "[greedy] Custo total: " << greedy_node.cost << "\n\n";

  auto a_node = a_star(romenia_map, src, dst, romenia_heuristic);
  auto a_path = a_node.path();

  std::cout << "[A*] Caminho encontrado: ";
  print_path(a_path);
  std::cout << "[A*] Custo total: " << a_node.cost << "\n\n";

  return 0;
}

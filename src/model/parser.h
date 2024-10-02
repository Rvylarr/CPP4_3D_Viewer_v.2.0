#ifndef PARSER_H
#define PARSER_H

#include <set>
#include <string>
#include <vector>

namespace s21 {
class Parser {
 public:
  // --- error status enum --- //
  enum status { OK, FILE_MISSING, INCORRECT_FILE, EMPTY_FILE };

  // --- constructors --- //
  Parser();
  ~Parser();

  // --- setters --- //
  void set_vertex_cell(int x, int y, double value);

  // --- getters --- //
  std::vector<std::vector<double>> get_vertex_vector();
  std::vector<std::vector<unsigned>> get_edges_vector();
  double get_vertex_cell(int i, int j);
  unsigned get_edge_cell(int i, int j);

  int get_edges_count();
  int get_vertexes_count();
  int get_vertexes_count_in_edge(int i);

  double get_x_max();
  double get_x_min();
  double get_y_max();
  double get_y_min();
  double get_z_max();
  double get_z_min();
  int GetVCount();  // ???

  // --- other methods --- //
  status parse_file(const std::string& file_name);
  void clear_data();

 private:
  // --- parsing methods --- //
  status reserve_data(std::ifstream& file);
  status parse_line(const std::string& line);
  status parse_vertex(const std::string& data);
  status parse_face(const std::string& data);

  // --- values --- //
  std::vector<std::vector<double>> vertexes_;
  std::vector<std::vector<unsigned>> edges_;
  int vertexes_count_;
  int edges_count_;

  double x_max_;
  double x_min_;
  double y_max_;
  double y_min_;
  double z_max_;
  double z_min_;
};
}  // namespace s21
#endif  // PARSER_H

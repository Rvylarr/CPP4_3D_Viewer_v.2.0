#include "parser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

// --- constructors --- //

Parser::Parser() {
  vertexes_count_ = 0;
  edges_count_ = 0;
  x_max_ = -2.22507e-308;
  x_min_ = 2.22507e-308;
  y_max_ = -2.22507e-308;
  y_min_ = 2.22507e-308;
  z_max_ = -2.22507e-308;
  z_min_ = 2.22507e-308;
}

Parser::~Parser() { clear_data(); }

// --- setters --- //

void Parser::set_vertex_cell(int x, int y, double value) {
  vertexes_[x][y] = value;
}

// --- getters --- //

std::vector<std::vector<double>> Parser::get_vertex_vector() {
  return vertexes_;
}

std::vector<std::vector<unsigned>> Parser::get_edges_vector() { return edges_; }

double Parser::get_vertex_cell(int i, int j) { return vertexes_[i][j]; }

unsigned Parser::get_edge_cell(int i, int j) { return edges_[i][j]; }

int Parser::get_edges_count() { return edges_count_; }

int Parser::get_vertexes_count() { return vertexes_count_; }

int Parser::get_vertexes_count_in_edge(int i) { return edges_[i].size(); }

double Parser::get_x_max() { return x_max_; }

double Parser::get_x_min() { return x_min_; }

double Parser::get_y_max() { return y_max_; }

double Parser::get_y_min() { return y_min_; }

double Parser::get_z_max() { return z_max_; }

double Parser::get_z_min() { return z_min_; }

int Parser::GetVCount() { return edges_.size(); }

// --- other methods --- //

s21::Parser::status Parser::parse_file(const std::string &file_name) {
  clear_data();
  std::ifstream file(file_name);
  if (!file.is_open()) return FILE_MISSING;
  status status = OK;
  status = reserve_data(file);
  std::string current_line;
  while (std::getline(file, current_line) && status == OK) {
    status = parse_line(current_line);
  }
  status = vertexes_.empty() || edges_.empty() ? EMPTY_FILE : status;
  if (status != OK) clear_data();
  file.close();
  return status;
}

void Parser::clear_data() {
  if (!vertexes_.empty()) {
    for (auto &inner : vertexes_) inner.clear();
    vertexes_.clear();
    vertexes_.shrink_to_fit();
    vertexes_count_ = 0;
  }

  if (!edges_.empty()) {
    for (auto &inner : edges_) inner.clear();
    edges_.clear();
    edges_.shrink_to_fit();
    edges_count_ = 0;
  }
  vertexes_count_ = 0;
  edges_count_ = 0;
  x_max_ = -2.22507e-308;
  x_min_ = 2.22507e-308;
  y_max_ = -2.22507e-308;
  y_min_ = 2.22507e-308;
  z_max_ = -2.22507e-308;
  z_min_ = 2.22507e-308;
}

/**
 * Reads an OBJ file and reserves memory for the vertex and edge data.
 *
 * @param file The input file stream to read from.
 *
 * @return The status of the operation. Returns kErrorFileEmpty if the file is
 * empty after parsing.
 *
 * @throws None
 */
s21::Parser::status Parser::reserve_data(std::ifstream &file) {
  std::string current_line;
  while (std::getline(file, current_line)) {
    if (current_line.length() <= 2) continue;
    std::istringstream iss(current_line);
    std::string type;
    iss >> type;
    if (type == "v") {
      vertexes_count_++;
    } else if (type == "f") {
      std::string edges_line = current_line.substr(2);
      std::istringstream iss_f(edges_line);
      std::string index_str;
      while (iss_f >> index_str) edges_count_++;
    }
  }
  if (vertexes_count_ > 0 && edges_count_ > 0) {
    vertexes_.reserve(vertexes_count_);
    edges_.reserve(edges_count_);
  } else
    return EMPTY_FILE;
  file.clear();
  file.seekg(0, std::ios::beg);
  return OK;
}

/**
 * Parses a line from an OBJ file and performs the corresponding action based on
 * the type of the line.
 *
 * @param line The line to parse.
 *
 * @return The status of the parsing operation. Returns kOk if the parsing is
 * successful, or kErrorIncorrectFile if there is an error in the file.
 *
 * @throws None
 */
s21::Parser::status Parser::parse_line(const std::string &line) {
  status status = OK;
  if (line.length() > 2) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;
    if (type == "v") {
      std::string data = line.substr(2);
      status = parse_vertex(data);
    } else if (type == "f") {
      std::string data = line.substr(2);
      status = parse_face(data);
    }
  }
  return status;
}

/**
 * Parses a vertex from an OBJ file and adds it to the verteces_ vector.
 *
 * @param data The string containing the vertex coordinates (x, y, z).
 *
 * @return The status of the parsing operation. Returns kErrorIncorrectFile if
 * the vertex data is invalid.
 *
 * @throws None
 */
s21::Parser::status Parser::parse_vertex(const std::string &data) {
  std::istringstream iss(data);
  double x, y, z;
  if (iss >> x >> y >> z) {
    if (x < x_min_) x_min_ = x;
    if (x > x_max_) x_max_ = x;
    if (y < y_min_) y_min_ = y;
    if (y > y_max_) y_max_ = y;
    if (z < z_min_) z_min_ = z;
    if (z > z_max_) z_max_ = z;
    std::vector<double> temp;
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);
    vertexes_.push_back(temp);
    temp.clear();
  } else {
    clear_data();
    return INCORRECT_FILE;
  }
  return OK;
}

/**
 * Parses a face from an OBJ file and inserts the indices into the faces_buff
 * vector.
 *
 * @param data The string containing the face indices.
 *
 * @return The status of the parsing operation. Returns kErrorIncorrectFile if
 * the index is out of range or if the vertex count is incorrect.
 *
 * @throws None
 */
s21::Parser::status Parser::parse_face(const std::string &data) {
  std::istringstream iss(data);
  std::string index_str;
  bool is_first_index = true;
  std::vector<unsigned int> faces_buff;
  while (iss >> index_str) {
    long index;

    try {
      index = std::stoi(index_str);
    } catch (const std::out_of_range &e) {
      return INCORRECT_FILE;
    }
    if (std::abs(index) > static_cast<long>(vertexes_.size()))
      return INCORRECT_FILE;
    if (index < 0)
      index += vertexes_count_;
    else
      index -= 1;
    if (is_first_index) {
      faces_buff.push_back(index);
      is_first_index = false;
    } else {
      faces_buff.push_back(index);
    }
  }
  edges_.push_back(faces_buff);
  return OK;
}
}  // namespace s21

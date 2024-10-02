#ifndef CPP4_3DVIEWER_V2_0_1_SRC_CONTROLLER_CONTROLLER_H_
#define CPP4_3DVIEWER_V2_0_1_SRC_CONTROLLER_CONTROLLER_H_

#include <iostream>
#include <string>

#include "../model/modeldata.h"
#include "../model/parser.h"
#include "../model/preparer.h"

using namespace std;

namespace s21 {
class Controller {
 public:
  static ModelData *get_model_data();

  // --- model interaction --- //
  static void upload_model(int &vertexes, int &edges, string path);
  static void change_vertex_color(int r, int g, int b);
  static void change_vertex_size(short size);
  static void change_vertex_type(short type);
  static void change_edge_color(int r, int g, int b);
  static void change_edge_width(short width);
  static void change_edge_type(short type);
  static void change_background_color(int r, int g, int b);
  static void change_projection(bool type);
  static void move_model(double x, double y);
  static void rotate_model(double x, double y);
  static void scale_in_model();
  static void scale_out_model();

  // --- memento --- //
  static void save_memento();
  static void load_memento();
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_1_SRC_CONTROLLER_CONTROLLER_H_

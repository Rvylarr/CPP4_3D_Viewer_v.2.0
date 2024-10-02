#ifndef MODELDATA_H
#define MODELDATA_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "observable.h"
#include "observer.h"
#include "parser.h"

using namespace std;

namespace s21 {
class ModelData : public Observable {
 public:
  struct color_t {
    double r, g, b;
  };

  // --- setters --- //
  void set_obj_data(string path);
  void set_projection(bool type);
  void set_vertex_size(short size);
  void set_vertex_type(short type);
  void set_edge_width(short width);
  void set_edge_type(short type);
  void set_vertex_color(int r, int g, int b);
  void set_edge_color(int r, int g, int b);
  void set_background_color(int r, int g, int b);

  // --- getters --- //
  static ModelData* get_instance();
  Parser& get_obj_data();
  bool get_projection_type();
  short get_vertex_size();
  short get_vertex_type();
  short get_edge_width();
  short get_edge_type();
  double get_move_x();
  double get_move_y();
  double get_rotate_x();
  double get_rotate_y();
  float get_scale_modifier();
  color_t get_vertex_color();
  color_t get_edge_color();
  color_t get_background_color();

  // --- model interaction --- //
  void move_model(double x, double y);
  void rotate_model(double x, double y);
  void scale_in_model();
  void scale_out_model();

  // --- observable methods --- //
  void register_observer(Observer* observer) override;
  void remove_observer(Observer* observer) override;
  void notify_observers() override;

  // --- memento methods --- //
  void create_memento();
  void restore_memento();
  void delete_memento();

 private:
  class Memento;

  // --- constructors and operators --- //
  ModelData();
  ~ModelData();
  ModelData(const ModelData&) = delete;
  ModelData& operator=(const ModelData&) = delete;

  // --- support methods --- //
  void convert_color_for_GL(int r, int g, int b, color_t* color);

  // --- values --- //
  static ModelData* instance;
  vector<Observer*> observers;
  Memento* memento;

  // --- model properties --- //
  Parser obj_data;
  bool projection_type;
  short vertex_size, vertex_type;
  short edge_width, edge_type;
  double x_move, y_move;
  double x_rotate, y_rotate;
  float scale_modifier;
  color_t vertex_color, edge_color, background_color;

  class Memento {
   public:
    Memento();
    ~Memento() = default;

    // --- getters --- //
    Parser get_obj_data();
    bool get_projection_type();
    short get_vertex_size();
    short get_vertex_type();
    short get_edge_width();
    short get_edge_type();
    double get_move_x();
    double get_move_y();
    double get_rotate_x();
    double get_rotate_y();
    float get_scale_modifier();
    color_t get_vertex_color();
    color_t get_edge_color();
    color_t get_background_color();

   private:
    Parser obj_data;
    bool projection_type;
    short vertex_size, vertex_type;
    short edge_width, edge_type;
    double x_move, y_move;
    double x_rotate, y_rotate;
    float scale_modifier;
    color_t vertex_color, edge_color, background_color;
  };
};
}  // namespace s21

#endif  // MODELDATA_H

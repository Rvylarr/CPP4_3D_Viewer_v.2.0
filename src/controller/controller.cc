#include "controller.h"

namespace s21 {
ModelData *Controller::get_model_data() {
  return s21::ModelData::get_instance();
}

// --- model interaction --- //
void Controller::upload_model(int &vertexes, int &edges, string path) {
  try {
    s21::ModelData::get_instance()->set_obj_data(path);
    vertexes =
        s21::ModelData::get_instance()->get_obj_data().get_vertexes_count();
    edges = s21::ModelData::get_instance()->get_obj_data().get_edges_count();
    Preparer::move_center();
    Preparer::scale_start();
  } catch (string exception) {
    cout << "[Error] " + exception + "\n";
  }
}

void Controller::change_vertex_color(int r, int g, int b) {
  s21::ModelData::get_instance()->set_vertex_color(r, g, b);
}

void Controller::change_vertex_size(short size) {
  s21::ModelData::get_instance()->set_vertex_size(size);
}

void Controller::change_vertex_type(short type) {
  s21::ModelData::get_instance()->set_vertex_type(type);
}

void Controller::change_edge_color(int r, int g, int b) {
  s21::ModelData::get_instance()->set_edge_color(r, g, b);
}

void Controller::change_edge_width(short width) {
  s21::ModelData::get_instance()->set_edge_width(width);
}

void Controller::change_edge_type(short type) {
  s21::ModelData::get_instance()->set_edge_type(type);
}

void Controller::change_background_color(int r, int g, int b) {
  s21::ModelData::get_instance()->set_background_color(r, g, b);
}

void Controller::change_projection(bool type) {
  s21::ModelData::get_instance()->set_projection(type);
}

void Controller::move_model(double x, double y) {
  s21::ModelData::get_instance()->move_model(x, y);
}

void Controller::rotate_model(double x, double y) {
  s21::ModelData::get_instance()->rotate_model(x, y);
}

void Controller::scale_in_model() {
  s21::ModelData::get_instance()->scale_in_model();
}

void Controller::scale_out_model() {
  s21::ModelData::get_instance()->scale_out_model();
}

void Controller::save_memento() {
  try {
    s21::ModelData::get_instance()->create_memento();
  } catch (string exception) {
    cout << "[Error] " + exception + "\n";
  }
}

void Controller::load_memento() {
  try {
    s21::ModelData::get_instance()->restore_memento();
  } catch (string exception) {
    cout << "[Error] " + exception + "\n";
  }
}
}  // namespace s21

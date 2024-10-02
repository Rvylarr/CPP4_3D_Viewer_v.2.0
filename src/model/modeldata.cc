#include "modeldata.h"

namespace s21 {
ModelData* ModelData::instance = nullptr;

// --- setters --- //
void ModelData::set_obj_data(string path) {
  obj_data.parse_file(path);
  notify_observers();
}

void ModelData::set_projection(bool type) {
  projection_type = type;
  notify_observers();
}

void ModelData::set_vertex_size(short size) {
  vertex_size = size;
  notify_observers();
}

void ModelData::set_vertex_type(short type) {
  vertex_type = type;
  notify_observers();
}

void ModelData::set_edge_width(short width) {
  edge_width = width;
  notify_observers();
}

void ModelData::set_edge_type(short type) {
  edge_type = type;
  notify_observers();
}

void ModelData::set_vertex_color(int r, int g, int b) {
  convert_color_for_GL(r, g, b, &vertex_color);
  notify_observers();
}

void ModelData::set_edge_color(int r, int g, int b) {
  convert_color_for_GL(r, g, b, &edge_color);
  notify_observers();
}

void ModelData::set_background_color(int r, int g, int b) {
  convert_color_for_GL(r, g, b, &background_color);
  notify_observers();
}

// --- getters --- //
bool ModelData::get_projection_type() { return projection_type; }

short ModelData::get_vertex_size() { return vertex_size; }

short ModelData::get_vertex_type() { return vertex_type; }

short ModelData::get_edge_width() { return edge_width; }

short ModelData::get_edge_type() { return edge_type; }

double ModelData::get_move_x() { return x_move; }

double ModelData::get_move_y() { return y_move; }

double ModelData::get_rotate_x() { return x_rotate; }

double ModelData::get_rotate_y() { return y_rotate; }

float ModelData::get_scale_modifier() { return scale_modifier; }

ModelData::color_t ModelData::get_vertex_color() { return vertex_color; }

ModelData::color_t ModelData::get_edge_color() { return edge_color; }

ModelData::color_t ModelData::get_background_color() {
  return background_color;
}

Parser& ModelData::get_obj_data() { return obj_data; }

// --- model interaction --- //
void ModelData::move_model(double x, double y) {
  x_move = x;
  y_move = y;
  notify_observers();
}

void ModelData::rotate_model(double x, double y) {
  x_rotate = x;
  y_rotate = y;
  notify_observers();
}

void ModelData::scale_in_model() {
  scale_modifier *= 0.9;
  notify_observers();
}

void ModelData::scale_out_model() {
  scale_modifier *= 1.1;
  notify_observers();
}

// --- observable methods --- //
void ModelData::register_observer(Observer* observer) {
  observers.push_back(observer);
}

void ModelData::remove_observer(Observer* observer) {
  auto it = find(observers.begin(), observers.end(), observer);
  if (it != observers.end()) {
    observers.erase(it);
  }
}

void ModelData::notify_observers() {
  for (Observer* observer : observers) observer->update_state();
}

// --- memento methods --- //
void ModelData::create_memento() {
  if (memento != nullptr) delete memento;
  memento = new Memento();
}

void ModelData::restore_memento() {
  if (memento != nullptr) {
    obj_data = memento->get_obj_data();
    projection_type = memento->get_projection_type();
    vertex_size = memento->get_vertex_size();
    vertex_type = memento->get_vertex_type();
    edge_width = memento->get_edge_width();
    edge_type = memento->get_edge_type();
    x_move = memento->get_move_x();
    y_move = memento->get_move_y();
    x_rotate = memento->get_rotate_x();
    y_rotate = memento->get_rotate_y();
    scale_modifier = memento->get_scale_modifier();
    vertex_color = memento->get_vertex_color();
    edge_color = memento->get_edge_color();
    background_color = memento->get_background_color();
    notify_observers();
  }
}

void ModelData::delete_memento() {
  if (memento != nullptr) delete memento;
}

// --- constructors and operators --- //
ModelData::ModelData() {
  projection_type = true;  // центральная, параллельная
  vertex_size = 20;
  vertex_type = 1;  // 0 - нет, 1 - круг, 2 - квадрат
  vertex_color.r = 0;
  vertex_color.g = 0;
  vertex_color.b = 0;

  edge_width = 5;
  edge_type = 1;  // 0 - нет, 1 - обычная, 2 - пунктир
  edge_color.r = 0;
  edge_color.g = 0;
  edge_color.b = 0;

  x_move = 0, y_move = 0;
  x_rotate = 0, y_rotate = 0;
  scale_modifier = 1;
  memento = nullptr;
}

ModelData::~ModelData() {
  instance = nullptr;
  if (memento != nullptr) delete memento;
}

ModelData* ModelData::get_instance() {
  if (instance == nullptr) instance = new ModelData();
  return instance;
}

// --- support methods --- //
void ModelData::convert_color_for_GL(int r, int g, int b, color_t* color) {
  color->r = (double)r / 255;
  color->g = (double)g / 255;
  color->b = (double)b / 255;
}

// === memento class === //

// --- constructor --- //
ModelData::Memento::Memento() {
  ModelData* model_data = ModelData::get_instance();

  obj_data = model_data->get_obj_data();
  projection_type = model_data->get_projection_type();
  vertex_size = model_data->get_vertex_size();
  vertex_type = model_data->get_vertex_type();
  edge_width = model_data->get_edge_width();
  edge_type = model_data->get_edge_type();
  x_move = model_data->get_move_x();
  y_move = model_data->get_move_y();
  x_rotate = model_data->get_rotate_x();
  y_rotate = model_data->get_rotate_y();
  scale_modifier = model_data->get_scale_modifier();
  vertex_color = model_data->get_vertex_color();
  edge_color = model_data->get_edge_color();
  background_color = model_data->get_background_color();
}

// --- getters --- //
Parser ModelData::Memento::get_obj_data() { return obj_data; }

bool ModelData::Memento::get_projection_type() { return projection_type; }

short ModelData::Memento::get_vertex_size() { return vertex_size; }

short ModelData::Memento::get_vertex_type() { return vertex_type; }

short ModelData::Memento::get_edge_width() { return edge_width; }

short ModelData::Memento::get_edge_type() { return edge_type; }

double ModelData::Memento::get_move_x() { return x_move; }

double ModelData::Memento::get_move_y() { return y_move; }

double ModelData::Memento::get_rotate_x() { return x_rotate; }

double ModelData::Memento::get_rotate_y() { return y_rotate; }

float ModelData::Memento::get_scale_modifier() { return scale_modifier; }

ModelData::color_t ModelData::Memento::get_vertex_color() {
  return vertex_color;
}

ModelData::color_t ModelData::Memento::get_edge_color() { return edge_color; }

ModelData::color_t ModelData::Memento::get_background_color() {
  return background_color;
}
}  // namespace s21

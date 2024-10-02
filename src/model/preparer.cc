#include "preparer.h"

namespace s21 {
// --- constructor --- //
Preparer::Preparer() {}

// --- calibration methods --- //
void Preparer::move_center() {  // отцентровка фигура
  ModelData *model_data = s21::ModelData::get_instance();

  double centerX = 0, centerY = 0, centerZ = 0;
  centerX = model_data->get_obj_data().get_x_min() +
            (model_data->get_obj_data().get_x_max() -
             model_data->get_obj_data().get_x_min()) /
                2;
  centerY = model_data->get_obj_data().get_y_min() +
            (model_data->get_obj_data().get_y_max() -
             model_data->get_obj_data().get_y_min()) /
                2;
  centerZ = model_data->get_obj_data().get_z_min() +
            (model_data->get_obj_data().get_z_max() -
             model_data->get_obj_data().get_z_min()) /
                2;
  if (centerX != 0) {
    for (int i = 0; i < model_data->get_obj_data().get_vertexes_count(); i++) {
      model_data->get_obj_data().set_vertex_cell(
          i, 0, model_data->get_obj_data().get_vertex_cell(i, 0) - centerX);
    }
  }
  if (centerY != 0) {
    for (int i = 0; i < model_data->get_obj_data().get_vertexes_count(); i++) {
      model_data->get_obj_data().set_vertex_cell(
          i, 1, model_data->get_obj_data().get_vertex_cell(i, 1) - centerX);
    }
  }
  if (centerZ != 0) {
    for (int i = 0; i < model_data->get_obj_data().get_vertexes_count(); i++) {
      model_data->get_obj_data().set_vertex_cell(
          i, 2, model_data->get_obj_data().get_vertex_cell(i, 2) - centerX);
    }
  }
}

// приводит фигуру в нужный диапазон
void Preparer::scale_start() {
  ModelData *model_data = s21::ModelData::get_instance();
  double x = 0, y = 0, z = 0;

  x = model_data->get_obj_data().get_x_max() -
      model_data->get_obj_data().get_x_min();
  y = model_data->get_obj_data().get_y_max() -
      model_data->get_obj_data().get_y_min();
  z = model_data->get_obj_data().get_z_max() -
      model_data->get_obj_data().get_z_min();
  double max = find_max(x, y, z);
  double scale = (0.8 - (0.8 * (-1))) / max;

  for (int i = 0; i < model_data->get_obj_data().get_vertexes_count(); i++) {
    model_data->get_obj_data().set_vertex_cell(
        i, 0, model_data->get_obj_data().get_vertex_cell(i, 0) * scale);
    model_data->get_obj_data().set_vertex_cell(
        i, 1, model_data->get_obj_data().get_vertex_cell(i, 1) * scale);
    model_data->get_obj_data().set_vertex_cell(
        i, 2, model_data->get_obj_data().get_vertex_cell(i, 2) * scale);
  }
}

// ф-ия для помощи в просчете  скейла
double Preparer::find_max(double x, double y, double z) {
  double max = 0;
  if (x > y && x > z) {
    max = x;
  } else if (y > x && y > z) {
    max = y;
  } else {
    max = z;
  }
  return max;
}
}  // namespace s21

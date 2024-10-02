#ifndef PREPARER_H
#define PREPARER_H

#include "modeldata.h"

namespace s21 {
class Preparer {
 public:
  // --- constructor --- //
  Preparer();
  ~Preparer() = default;

  // --- calibration methods --- //
  static void move_center();
  static void scale_start();
  static double find_max(double x, double y, double z);

 private:
  //        ModelData* model_data;
};
}  // namespace s21

#endif  // PREPARER_H

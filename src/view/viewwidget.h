#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include <QtOpenGL>
#include <QtOpenGLWidgets>
#include <QtWidgets/QWidget>

#include "../controller/controller.h"
#include "../model/observer.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class ViewWidget;
}
QT_END_NAMESPACE

namespace s21 {
class ViewWidget : public QOpenGLWidget, public Observer {
  Q_OBJECT

 public:
  // --- constructors --- //
  explicit ViewWidget(QWidget *parent = nullptr);
  ~ViewWidget();

  void change_pos_coeficent();

 private:
  // --- interaction methods --- //
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *) override;

  // --- draw methods --- //
  void draw_model();
  void pick_edges_type();
  void pick_vertexes_type();

  // --- values --- //
  Ui::ViewWidget *ui;
  s21::ModelData *model_data;
  QElapsedTimer delay;
  QPoint mouse_pos;
  bool move_or_rotate;

 protected:
  // --- GL methods --- //
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  // --- observer methods --- //
  void update_state() override;
};
}  // namespace s21

#endif  // VIEWWIDGET_H

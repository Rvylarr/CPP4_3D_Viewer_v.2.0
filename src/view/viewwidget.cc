#include "viewwidget.h"

namespace s21 {
// --- constructors --- //
ViewWidget::ViewWidget(QWidget *parent) : QOpenGLWidget{parent} {
  model_data = s21::Controller::get_model_data();
  model_data->register_observer(this);
  model_data->get_obj_data().clear_data();
  delay.start();
}

ViewWidget::~ViewWidget() { model_data->remove_observer(this); }

// --- interaction methods --- //
void ViewWidget::mousePressEvent(QMouseEvent *event) {
  if (delay.elapsed() >= 25) {
    if (event->button() == Qt::LeftButton) {
      mouse_pos = event->pos();
      move_or_rotate = true;
    } else if (event->button() == Qt::RightButton) {
      mouse_pos = event->pos();
      move_or_rotate = false;
    }
    delay.restart();
  }
}

void ViewWidget::mouseMoveEvent(QMouseEvent *event) {
  move_or_rotate
      ? s21::Controller::rotate_model(
            1 / M_PI * (event->position().y() - mouse_pos.y()),
            1 / M_PI * (event->position().x() - mouse_pos.x()))
      : s21::Controller::move_model(
            0.008 / M_PI * (event->position().x() - mouse_pos.x()),
            (-0.008) / M_PI * (event->position().y() - mouse_pos.y()));
}

void ViewWidget::wheelEvent(QWheelEvent *event) {
  event->angleDelta().y() > 0 ? s21::Controller::scale_out_model()
                              : s21::Controller::scale_in_model();
}

// --- draw methods --- //
void ViewWidget::draw_model() {
  ModelData::color_t edge_color = model_data->get_edge_color();
  short edge_type = model_data->get_edge_type();
  short vertex_type = model_data->get_vertex_type();
  double x = 0, y = 0, z = 0;

  for (int i = 0; i < model_data->get_obj_data().GetVCount(); i++) {
    for (int j = 0;
         j < model_data->get_obj_data().get_vertexes_count_in_edge(i); j++) {
      x = model_data->get_obj_data().get_vertex_cell(
          model_data->get_obj_data().get_edge_cell(i, j), 0);
      y = model_data->get_obj_data().get_vertex_cell(
          model_data->get_obj_data().get_edge_cell(i, j), 1);
      z = model_data->get_obj_data().get_vertex_cell(
          model_data->get_obj_data().get_edge_cell(i, j), 2);
      glColor3d(edge_color.r, edge_color.g, edge_color.b);
      glPointSize(model_data->get_vertex_size());
      if (edge_type != 0) glBegin(GL_LINE_LOOP);
      if (vertex_type != 0) glBegin(GL_POINTS);
      glVertex3d(x, y, z);
    }
    glEnd();
  }
}

void ViewWidget::pick_edges_type() {
  short edge_type = model_data->get_edge_type();

  if (edge_type != 0) glLineWidth(model_data->get_edge_width() / 10.0f);
  if (edge_type != 2)
    glDisable(GL_LINE_STIPPLE);
  else if (edge_type == 2) {
    glLineStipple(3, 0x00F0);
    glEnable(GL_LINE_STIPPLE);
  }
}

void ViewWidget::pick_vertexes_type() {
  short vertex_type = model_data->get_vertex_type();
  ModelData::color_t vertex_color = model_data->get_vertex_color();

  glPointSize(model_data->get_vertex_size());
  if (vertex_type == 1)
    glEnable(GL_POINT_SMOOTH);
  else if (vertex_type == 2)
    glDisable(GL_POINT_SMOOTH);
  for (int i = 0; i < model_data->get_obj_data().get_vertexes_count(); i++) {
    double x = model_data->get_obj_data().get_vertex_cell(i, 0),
           y = model_data->get_obj_data().get_vertex_cell(i, 1),
           z = model_data->get_obj_data().get_vertex_cell(i, 2);
    if (vertex_type != 0) glBegin(GL_POINTS);
    glColor3d(vertex_color.r, vertex_color.g, vertex_color.b);
    glVertex3d(x, y, z);
    glEnd();
  }
}

// --- GL methods --- //
void ViewWidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_DOUBLEBUFFER);
}

void ViewWidget::paintGL() {
  glClearColor(model_data->get_background_color().r,
               model_data->get_background_color().g,
               model_data->get_background_color().b, 1.0f);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  if (model_data->get_projection_type()) {
    glFrustum(model_data->get_obj_data().get_x_min(),
              model_data->get_obj_data().get_x_max(),
              model_data->get_obj_data().get_y_min(),
              model_data->get_obj_data().get_y_max(),
              model_data->get_obj_data().get_z_min(),
              model_data->get_obj_data().get_z_max());
  } else {
    glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glTranslated(model_data->get_move_x(), model_data->get_move_y(), 0);
  glRotated(model_data->get_rotate_x(), 1, 0, 0);
  glRotated(model_data->get_rotate_y(), 0, 1, 0);
  glScaled(model_data->get_scale_modifier(), model_data->get_scale_modifier(),
           model_data->get_scale_modifier());

  pick_vertexes_type();
  pick_edges_type();
  draw_model();
}

void ViewWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

// --- observer methods --- //
void ViewWidget::update_state() { update(); }
}  // namespace s21

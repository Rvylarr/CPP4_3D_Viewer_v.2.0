#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QColor>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QSettings>
#include <QTimer>
#include <iostream>

#include "../QtGifImage-master/src/gifimage/qgifimage.h"
#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

using namespace std;

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  // --- constructors --- //
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void resizeEvent(QResizeEvent *);

  // --- main panel --- //
  void on_load_file_clicked();
  void on_change_main_color_clicked();
  void on_open_editor_clicked();
  void on_change_projection_clicked();
  void on_info_clicked();
  void on_screenshot_pressed();
  void on_screenshot_released();
  void on_record_video_clicked();
  void on_start_gif_record_clicked();

  // --- editor panel --- //
  void on_edge_color_clicked();
  void on_vertexes_color_clicked();
  void on_edge_solid_clicked();
  void on_edge_dotted_clicked();
  void on_edge_none_clicked();
  void on_vertexes_circle_clicked();
  void on_vertexes_square_clicked();
  void on_vertexes_none_clicked();
  void on_edge_width_slider_sliderMoved();
  void on_vertex_size_slider_sliderMoved();
  void on_edge_width_spin_valueChanged();
  void on_vertex_size_spin_valueChanged();
  void on_save_memento_clicked();
  void on_load_memento_clicked();

 private:
  Ui::MainWindow *ui;

  struct gif_t {
    QGifImage *gif_image;
    QString gif_name;
    QTimer *timer;
    short loops, competed_loops;
  };

  // --- support methods --- //
  QString color_to_string(int red, int green, int blue);
  void change_main_color(int r, int g, int b);
  void change_vertex_color(int r, int g, int b);
  void change_edge_color(int r, int g, int b);
  void change_cubic_vertexes(QColor color, short type);
  void load_config();
  void save_config();
  void record_gif();

  // --- values --- //
  QSettings *config;
  QString main_color;
  bool projection_mode;

  // --- gif values --- //
  gif_t *gif;
};
}  // namespace s21

#endif  // MAINWINDOW_H

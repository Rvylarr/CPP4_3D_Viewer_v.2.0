#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {

// --- constructors --- //
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  s21::Controller::get_model_data()->get_instance();

  gif = nullptr;
  config = new QSettings("School_21", "3D_viewer_v2_0", this);
  load_config();

  connect(ui->edge_width_spin, SIGNAL(valueChanged(int)), ui->edge_width_slider,
          SLOT(setValue(int)));
  connect(ui->edge_width_slider, SIGNAL(valueChanged(int)), ui->edge_width_spin,
          SLOT(setValue(int)));
  connect(ui->vertex_size_spin, SIGNAL(valueChanged(int)),
          ui->vertex_size_slider, SLOT(setValue(int)));
  connect(ui->vertex_size_slider, SIGNAL(valueChanged(int)),
          ui->vertex_size_spin, SLOT(setValue(int)));

  ui->editor_panel->setVisible(false);
  ui->info_panel->setVisible(false);
  ui->gif_panel->setVisible(false);
  ui->open_editor_no_color->setVisible(false);
}

MainWindow::~MainWindow() {
  if (gif != nullptr) {
    disconnect(gif->timer, SIGNAL(timeout()), this, SLOT(record_gif()));
    if (gif->gif_image != nullptr) delete gif->gif_image;
    if (gif->timer != nullptr) delete gif->timer;
    delete gif;
    gif = nullptr;
  }
  save_config();
  delete config;
  delete ui;
}

// === private slots === //

// --- main panel --- //

void MainWindow::resizeEvent(QResizeEvent *) {
  QSize size = this->size();
  int height = size.height();
  int width = size.width();

  ui->main_panel->setGeometry(width / 2 - 180, 20, 361, 70);
  ui->view_widget->setGeometry(0, (height - width) / 2, width, width);
  ui->gif_panel->setGeometry(width / 2 - 0, 100, 181, 111);
}

void MainWindow::on_load_file_clicked() {
  ui->load_file_color->setStyleSheet(main_color);
  QString filter = tr("OBJ (*.obj)");
  QString abs_path =
      QFileDialog::getOpenFileName(this, "\0", "\0", filter, &filter);
  QFile file(abs_path);

  if (file.open(QIODevice::ReadOnly)) {
    int vertex_count = 0, facet_count = 0;

    s21::Controller::upload_model(vertex_count, facet_count,
                                  abs_path.toStdString());
    while (abs_path.contains('/') || abs_path.contains('\''))
      abs_path.remove(0, 1);
    if (!ui->info_panel->isVisible()) on_info_clicked();
    ui->model_name->setText(abs_path);
    ui->vertex_count->setText("Vertexes: " + QString::number(vertex_count));
    ui->edge_count->setText("Edges: " + QString::number(facet_count));
    file.close();
  }
  ui->load_file_color->setStyleSheet("background-color: #ffffff;");
}

void MainWindow::on_change_main_color_clicked() {
  int r = 0, g = 0, b = 0;
  QColor qcolor_background =
      QColorDialog::getColor(Qt::gray, this, tr("Выберите цвет фона"));
  qcolor_background.getRgb(&r, &g, &b);
  change_main_color(r, g, b);
}

void MainWindow::on_open_editor_clicked() {
  ui->editor_panel->isVisible() ? ui->editor_panel->setVisible(false)
                                : ui->editor_panel->setVisible(true);
  ui->editor_panel->isVisible() ? ui->info_panel->setGeometry(15, 490, 191, 101)
                                : ui->info_panel->setGeometry(15, 20, 191, 101);
}

void MainWindow::on_change_projection_clicked() {
  if (s21::Controller::get_model_data()->get_projection_type()) {
    s21::Controller::change_projection(false);
    ui->change_projection->setToolTip("change to central projection");
    ui->change_projection->setIcon(
        QIcon(":/icons/icons/parallel_template.png"));
  } else {
    s21::Controller::change_projection(true);
    ui->change_projection->setToolTip("change to parallel projection");
    ui->change_projection->setIcon(QIcon(":/icons/icons/central_template.png"));
  }
}

void MainWindow::on_info_clicked() {
  if (!ui->info_panel->isVisible()) {
    ui->info_panel->setVisible(true);
    ui->info_color->setStyleSheet(main_color);
  } else {
    ui->info_panel->setVisible(false);
    ui->info_color->setStyleSheet("background-color: #ffffff;");
  }

  ui->editor_panel->isVisible() ? ui->info_panel->setGeometry(15, 490, 191, 101)
                                : ui->info_panel->setGeometry(15, 20, 191, 101);
}

void MainWindow::on_screenshot_pressed() {
  ui->screenshot->setIcon(QIcon(":/icons/icons/screenshot_pressed.png"));
}

void MainWindow::on_screenshot_released() {
  ui->screenshot->setIcon(QIcon(":/icons/icons/screenshot.png"));
  QString file_name = QFileDialog::getSaveFileName(
      0, "Saving screenshot...", "", tr("PNG (*.png);;BMP (*.bmp)"));

  if (file_name.isEmpty())
    return;
  else
    ui->view_widget->grabFramebuffer().save(file_name);
}

void MainWindow::on_record_video_clicked() {
  ui->gif_panel->isVisible() ? ui->gif_panel->setVisible(false)
                             : ui->gif_panel->setVisible(true);
}

void MainWindow::on_start_gif_record_clicked() {
  QString file_name =
      QFileDialog::getSaveFileName(this, "Save file", nullptr, "Image(*.gif)") +
      ".gif";

  if (!file_name.isNull()) {
    gif = new gif_t;
    gif->gif_name = file_name;
    gif->gif_image = new QGifImage;
    gif->timer = new QTimer;
    gif->timer->setInterval(1000 / ui->gif_fps->value());
    gif->competed_loops = 0;
    gif->loops = ui->gif_fps->value() * ui->gif_duration->value();

    connect(gif->timer, SIGNAL(timeout()), this, SLOT(record_gif()));
    gif->timer->start(1000 / ui->gif_fps->value());
    ui->gif_indicator->setStyleSheet("background-color: #e12222;");
  }
}

// --- editor panel --- //

void MainWindow::on_edge_color_clicked() {
  int r = 0, g = 0, b = 0;
  QColor qcolor_lines =
      QColorDialog::getColor(Qt::lightGray, this, tr("Выберите цвет линий"));
  qcolor_lines.getRgb(&r, &g, &b);
  change_edge_color(r, g, b);
}

void MainWindow::on_vertexes_color_clicked() {
  int r = 0, g = 0, b = 0;
  QColor qcolor_vertexes =
      QColorDialog::getColor(Qt::lightGray, this, tr("Выберите цвет точек"));
  qcolor_vertexes.getRgb(&r, &g, &b);
  change_vertex_color(r, g, b);
}

void MainWindow::on_edge_solid_clicked() {
  QColor color = ui->edge_color->palette().color(QPalette::Button).toRgb().Rgb;
  int r = 0, g = 0, b = 0;
  color.getRgb(&r, &g, &b);

  ui->edge_solid_color->setStyleSheet(main_color);
  ui->edge_dotted_color->setStyleSheet("background-color: #FFFFFF");
  ui->edge_none_color->setStyleSheet("background-color: #FFFFFF");
  ui->open_editor_no_color->setVisible(false);
  s21::Controller::change_edge_type(1);
}

void MainWindow::on_edge_dotted_clicked() {
  QColor color =
      ui->edge_color->palette().color(QPalette::Button).toExtendedRgb();
  int r = 0, g = 0, b = 0;
  color.getRgb(&r, &g, &b);

  ui->edge_solid_color->setStyleSheet("background-color: #FFFFFF");
  ui->edge_dotted_color->setStyleSheet(main_color);
  ui->edge_none_color->setStyleSheet("background-color: #FFFFFF");
  ui->open_editor_no_color->setVisible(false);
  s21::Controller::change_edge_type(2);
}

void MainWindow::on_edge_none_clicked() {
  ui->edge_solid_color->setStyleSheet("background-color: #FFFFFF");
  ui->edge_dotted_color->setStyleSheet("background-color: #FFFFFF");
  ui->edge_none_color->setStyleSheet(main_color);
  ui->open_editor_no_color->setVisible(true);
  s21::Controller::change_edge_type(0);
}

void MainWindow::on_vertexes_circle_clicked() {
  ui->vertexes_circle_color->setStyleSheet(main_color);
  ui->vertexes_square_color->setStyleSheet("background-color: #FFFFFF");
  ui->vertexes_none_color->setStyleSheet("background-color: #FFFFFF");
  s21::Controller::change_vertex_type(1);
  change_cubic_vertexes(ui->vertexes_color->palette().color(QPalette::Base), 1);
}

void MainWindow::on_vertexes_square_clicked() {
  ui->vertexes_circle_color->setStyleSheet("background-color: #FFFFFF");
  ui->vertexes_square_color->setStyleSheet(main_color);
  ui->vertexes_none_color->setStyleSheet("background-color: #FFFFFF");
  s21::Controller::change_vertex_type(2);
  change_cubic_vertexes(ui->vertexes_color->palette().color(QPalette::Base), 2);
}

void MainWindow::on_vertexes_none_clicked() {
  ui->vertexes_circle_color->setStyleSheet("background-color: #FFFFFF");
  ui->vertexes_square_color->setStyleSheet("background-color: #FFFFFF");
  ui->vertexes_none_color->setStyleSheet(main_color);
  s21::Controller::change_vertex_type(0);
  change_cubic_vertexes(ui->vertexes_color->palette().color(QPalette::Base), 0);
}

void MainWindow::on_edge_width_slider_sliderMoved() {
  s21::Controller::change_edge_width(ui->edge_width_slider->value());
}

void MainWindow::on_vertex_size_slider_sliderMoved() {
  s21::Controller::change_vertex_size(ui->vertex_size_slider->value());
}

void MainWindow::on_edge_width_spin_valueChanged() {
  s21::Controller::change_edge_width(ui->edge_width_spin->value());
}

void MainWindow::on_vertex_size_spin_valueChanged() {
  s21::Controller::change_vertex_size(ui->vertex_size_spin->value());
}

void MainWindow::on_save_memento_clicked() { s21::Controller::save_memento(); }

void MainWindow::on_load_memento_clicked() {
  s21::Controller::load_memento();
  save_config();
  load_config();
}

// --- support methods --- //

QString MainWindow::color_to_string(int red, int green, int blue) {
  QString r = QString::number(red);
  QString g = QString::number(green);
  QString b = QString::number(blue);
  return ":rgba(" + r + ", " + g + ", " + b + ", 100%);";
}

void MainWindow::change_main_color(int r, int g, int b) {
  QString color = color_to_string(r, g, b);
  QString font = "color" + color;
  QString background = "background-color" + color;
  main_color = background;

  ui->main_color->setStyleSheet(background);
  ui->left_stick->setStyleSheet(background);
  ui->right_stick->setStyleSheet(background);
  ui->projection_color->setStyleSheet(background);
  ui->model_name->setStyleSheet(font);
  ui->vertex_count->setStyleSheet(font);
  ui->edge_count->setStyleSheet(font);
  ui->vertex_size_spin->setStyleSheet(font);
  ui->edge_width_spin->setStyleSheet(font);
  ui->vertex_size_slider->setStyleSheet("QSlider::handle:horizontal {" +
                                        background + "}");
  ui->edge_width_slider->setStyleSheet("QSlider::handle:horizontal {" +
                                       background + "}");
  ui->gif_fps->setStyleSheet(font);
  ui->gif_duration->setStyleSheet(font);

  if (ui->info_panel->isVisible()) ui->info_color->setStyleSheet(background);

  if (s21::Controller::get_model_data()->get_edge_type() == 0)
    on_edge_none_clicked();
  else if (s21::Controller::get_model_data()->get_edge_type() == 1)
    on_edge_dotted_clicked();
  else if (s21::Controller::get_model_data()->get_edge_type() == 2)
    on_edge_solid_clicked();

  if (s21::Controller::get_model_data()->get_vertex_type() == 0)
    on_vertexes_none_clicked();
  else if (s21::Controller::get_model_data()->get_vertex_type() == 1)
    on_vertexes_circle_clicked();
  else if (s21::Controller::get_model_data()->get_vertex_type() == 2)
    on_vertexes_square_clicked();

  MainWindow::setStyleSheet("QMainWindow{" + background + "}");
  s21::Controller::change_background_color(r, g, b);
}

void MainWindow::change_vertex_color(int r, int g, int b) {
  QString color = color_to_string(r, g, b);
  QString background = "background-color" + color;

  ui->vertexes_color->setStyleSheet(background);
  s21::Controller::change_vertex_color(r, g, b);
  change_cubic_vertexes(ui->vertexes_color->palette().color(QPalette::Base),
                        s21::Controller::get_model_data()->get_edge_type());
}

void MainWindow::change_edge_color(int r, int g, int b) {
  QString color = color_to_string(r, g, b);
  QString background = "background-color" + color;

  ui->edge_color->setStyleSheet(background);
  if (s21::Controller::get_model_data()->get_edge_type() != 0)
    ui->open_editor_color->setStyleSheet(background);
  s21::Controller::change_edge_color(r, g, b);
}

void MainWindow::change_cubic_vertexes(QColor color, short type) {
  QString background =
      "background-color" +
      color_to_string(color.red(), color.green(), color.blue());

  if (type == 0) {
    ui->vertex_1->setVisible(false);
    ui->vertex_2->setVisible(false);
    ui->vertex_3->setVisible(false);
    ui->vertex_4->setVisible(false);
    ui->vertex_5->setVisible(false);
    ui->vertex_6->setVisible(false);
    ui->vertex_7->setVisible(false);
  } else {
    ui->vertex_1->setVisible(true);
    ui->vertex_2->setVisible(true);
    ui->vertex_3->setVisible(true);
    ui->vertex_4->setVisible(true);
    ui->vertex_5->setVisible(true);
    ui->vertex_6->setVisible(true);
    ui->vertex_7->setVisible(true);
  }

  if (type == 1) {
    QString with_vertex = background + "border-radius: 3px;";
    ui->vertex_1->setStyleSheet(with_vertex);
    ui->vertex_2->setStyleSheet(with_vertex);
    ui->vertex_3->setStyleSheet(with_vertex);
    ui->vertex_4->setStyleSheet(with_vertex);
    ui->vertex_5->setStyleSheet(with_vertex);
    ui->vertex_6->setStyleSheet(with_vertex);
    ui->vertex_7->setStyleSheet(with_vertex);
  } else {
    ui->vertex_1->setStyleSheet(background);
    ui->vertex_2->setStyleSheet(background);
    ui->vertex_3->setStyleSheet(background);
    ui->vertex_4->setStyleSheet(background);
    ui->vertex_5->setStyleSheet(background);
    ui->vertex_6->setStyleSheet(background);
    ui->vertex_7->setStyleSheet(background);
  }
}

void MainWindow::load_config() {
  int r = 0, g = 0, b = 0;
  QColor loaded_color;

  loaded_color = QColor(config->value("main_color").toString());
  loaded_color.getRgb(&r, &g, &b);
  change_main_color(r, g, b);

  loaded_color = QColor(config->value("vertex_color").toString());
  loaded_color.getRgb(&r, &g, &b);
  change_vertex_color(r, g, b);

  if (config->value("vertex_type").toInt() == 0)
    on_vertexes_none_clicked();
  else if (config->value("vertex_type").toInt() == 1) {
    on_vertexes_circle_clicked();
    change_cubic_vertexes(loaded_color, 1);
  } else if (config->value("vertex_type").toInt() == 2) {
    on_vertexes_square_clicked();
    change_cubic_vertexes(loaded_color, 2);
  }

  loaded_color = QColor(config->value("edge_color").toString());
  loaded_color.getRgb(&r, &g, &b);
  change_edge_color(r, g, b);

  if (config->value("edge_type").toInt() == 0)
    on_edge_none_clicked();
  else if (config->value("edge_type").toInt() == 1)
    on_edge_solid_clicked();
  else if (config->value("edge_type").toInt() == 2)
    on_edge_dotted_clicked();

  ui->vertex_size_spin->setValue(config->value("vertex_size").toInt());
  ui->vertex_size_slider->setValue(config->value("vertex_size").toInt());
  ui->edge_width_spin->setValue(config->value("edge_width").toInt());
  ui->edge_width_slider->setValue(config->value("edge_width").toInt());

  if (config->value("projection_type").toBool()) on_change_projection_clicked();
}

void MainWindow::save_config() {
  config->setValue("main_color", ui->left_stick->palette()
                                     .color(ui->left_stick->backgroundRole())
                                     .toRgb());
  config->setValue(
      "vertex_color",
      ui->vertexes_color->palette().color(QPalette::Button).toRgb());
  config->setValue("edge_color",
                   ui->edge_color->palette().color(QPalette::Button).toRgb());
  config->setValue("vertex_size", ui->vertex_size_spin->value());
  config->setValue("edge_width", ui->edge_width_spin->value());
  config->setValue("vertex_type",
                   s21::Controller::get_model_data()->get_vertex_type());
  config->setValue("edge_type",
                   s21::Controller::get_model_data()->get_edge_type());
  config->setValue("projection_type",
                   s21::Controller::get_model_data()->get_projection_type());
}

void MainWindow::record_gif() {
  if (gif->competed_loops != gif->loops) {
    QImage frame(ui->view_widget->grabFramebuffer());
    gif->gif_image->addFrame(frame, 1000 / ui->gif_fps->value());
    gif->competed_loops++;
  } else {
    gif->gif_image->save(gif->gif_name);
    gif->timer->stop();
    ui->gif_indicator->setStyleSheet("background-color: #b1b1b1;");
    disconnect(gif->timer, SIGNAL(timeout()), this, SLOT(record_gif()));
    delete gif->gif_image;
    delete gif->timer;
    delete gif;
    gif = nullptr;
  }
}
}  // namespace s21

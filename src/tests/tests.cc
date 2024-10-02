#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "../controller/controller.h"
#include "../model/parser.h"

using namespace s21;

int main() {
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}

TEST(PTest, ParseFile) {
  Parser parser;
  std::vector<std::vector<double>> vertecesTest = {
      {1.000000, -1.000000, -1.000000}, {1.000000, -1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000}, {-1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, -0.999999},  {0.999999, 1.000000, 1.000001},
      {-1.000000, 1.000000, 1.000000},  {-1.000000, 1.000000, -1.000000}};
  std::vector<std::vector<unsigned>> edgesTest = {
      {1, 2, 3}, {7, 6, 5}, {4, 5, 1}, {5, 6, 2}, {2, 6, 7}, {0, 3, 7},
      {0, 1, 3}, {4, 7, 5}, {0, 4, 1}, {1, 5, 2}, {3, 2, 7}, {4, 0, 7}};
  parser.parse_file("tests/figure/test_copy.obj");
  std::vector<std::vector<double>> verteces = parser.get_vertex_vector();
  std::vector<std::vector<unsigned>> edges = parser.get_edges_vector();
  ASSERT_EQ(vertecesTest, verteces);
  ASSERT_EQ(edgesTest, edges);
  ASSERT_EQ(parser.get_vertexes_count(), 8);
  ASSERT_EQ(parser.get_edges_count(), 36);
};

TEST(PTest, NormisGigaSmallParser) {
  std::string path = "tests/figure/test_copy_2.obj";
  Parser parser;
  std::vector<std::vector<double>> vertecesTest = {
      {{0.000001, -0.000001, -0.000001},
       {0.000001, -0.000001, 0.000001},
       {-0.000001, -0.000001, 0.000001},
       {-0.000001, -0.000001, -0.000001},
       {0.000001, 0.000001, -0.0000009},
       {0.0000009, 0.000001, 0.000001},
       {-0.000001, 0.000001, 0.000001},
       {-0.000001, 0.000001, -0.000001}}};
  std::vector<std::vector<unsigned>> edgesTest = {
      {1, 2, 3}, {7, 6, 5}, {4, 5, 1}, {5, 6, 2}, {2, 6, 7}, {0, 3, 7},
      {0, 1, 3}, {4, 7, 5}, {0, 4, 1}, {1, 5, 2}, {3, 2, 7}, {4, 0, 7}};

  parser.parse_file(path);
  ASSERT_EQ(vertecesTest, parser.get_vertex_vector());
  ASSERT_EQ(edgesTest, parser.get_edges_vector());
  ASSERT_EQ(parser.get_vertexes_count(), 8);
  ASSERT_EQ(parser.get_edges_count(), 36);
};

TEST(PTest, NoFile) {
  Parser parser;
  ASSERT_EQ(parser.parse_file("../nofile.obj"), 1);
  ASSERT_EQ(parser.get_vertexes_count(), 0);
  ASSERT_EQ(parser.get_edges_count(), 0);
};

TEST(PTest, EmptyFile) {
  Parser parser;
  ASSERT_EQ(parser.parse_file("tests/figure/empty.obj"), 3);
  ASSERT_EQ(parser.get_vertexes_count(), 0);
  ASSERT_EQ(parser.get_edges_count(), 0);
}

TEST(CTest, Normis) {
  int vertexes = 0;
  int edges = 0;
  std::string path = "tests/figure/test_copy.obj";
  Controller::upload_model(vertexes, edges, path);
  ASSERT_EQ(vertexes, 8);
  ASSERT_EQ(edges, 36);
  std::vector<std::vector<double>> vertecesTest = {{{0.8, -0.8, -0.8},
                                                    {0.8, -0.8, 0.8},
                                                    {-0.8, -0.8, 0.8},
                                                    {-0.8, -0.8, -0.8},
                                                    {0.8, 0.8, -0.799999},
                                                    {0.799999, 0.8, 0.8},
                                                    {-0.8, 0.8, 0.8},
                                                    {-0.8, 0.8, -0.8}}};
  std::vector<std::vector<unsigned>> edgesTest = {
      {1, 2, 3}, {7, 6, 5}, {4, 5, 1}, {5, 6, 2}, {2, 6, 7}, {0, 3, 7},
      {0, 1, 3}, {4, 7, 5}, {0, 4, 1}, {1, 5, 2}, {3, 2, 7}, {4, 0, 7}};

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      ASSERT_NEAR(vertecesTest[i][j],
                  Controller::get_model_data()
                      ->get_obj_data()
                      .get_vertex_vector()[i][j],
                  0.01);
    }
  }
  ASSERT_EQ(edgesTest,
            Controller::get_model_data()->get_obj_data().get_edges_vector());
  Controller::get_model_data()->get_obj_data().clear_data();
};

TEST(CTest, NormisBig) {
  int vertexes = 0;
  int edges = 0;
  std::string path = "tests/figure/test.obj";
  Controller::upload_model(vertexes, edges, path);
  ASSERT_EQ(vertexes, 8);
  std::vector<std::vector<double>> vertecesTest = {{{0.8, -0.8, -0.8},
                                                    {0.8, -0.8, 0.8},
                                                    {-0.8, -0.8, 0.8},
                                                    {-0.8, -0.8, -0.8},
                                                    {0.8, 0.8, -0.799999},
                                                    {0.799999, 0.8, 0.8},
                                                    {-0.8, 0.8, 0.8},
                                                    {-0.8, 0.8, -0.8}}};
  std::vector<std::vector<unsigned>> edgesTest = {
      {1, 2, 3}, {7, 6, 5}, {4, 5, 1}, {5, 6, 2}, {2, 6, 7}, {0, 3, 7},
      {0, 1, 3}, {4, 7, 5}, {0, 4, 1}, {1, 5, 2}, {3, 2, 7}, {4, 0, 7}};

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      ASSERT_NEAR(vertecesTest[i][j],
                  Controller::get_model_data()
                      ->get_obj_data()
                      .get_vertex_vector()[i][j],
                  0.01);
    }
  }
  ASSERT_EQ(edgesTest,
            Controller::get_model_data()->get_obj_data().get_edges_vector());
  Controller::get_model_data()->get_obj_data().clear_data();
};

// TEST(CTest, NormisGigaSmall) {
//     int vertexes = 0;
//     int edges = 0;
//     std::string path = "tests/figure/test copy 2.obj";
//     Controller::upload_model(vertexes, edges, path);
//     ASSERT_EQ(vertexes, 8);
//     std::vector<std::vector<double>> vertecesTest ={
//     { { 0.000001, -0.000001, -0.000001 },
//       { 0.000001, -0.000001, 0.000001 },
//       { -0.000001, -0.000001, 0.000001 },
//       { -0.000001, -0.000001, -0.000001 },
//       { 0.000001, 0.000001, -0.0000009 },
//       { 0.0000009, 0.000001, 0.000001 },
//       { -0.000001, 0.000001, 0.000001},
//       { -0.000001, 0.000001, -0.000001 } }};
//     std::vector<std::vector<unsigned>> edgesTest ={{1,2,3},
//     {7,6,5},
//     {4,5,1},
//     {5,6,2},
//     {2,6,7},
//     {0,3,7},
//     {0,1,3},
//     {4,7,5},
//     {0,4,1},
//     {1,5,2},
//     {3,2,7},
//     {4,0,7}};

//     double x = Controller::get_model_data()->get_obj_data().get_x_max()
//     - Controller::get_model_data()->get_obj_data().get_x_min();
//     double y = Controller::get_model_data()->get_obj_data().get_y_max()
//     - Controller::get_model_data()->get_obj_data().get_y_min();
//     double z = Controller::get_model_data()->get_obj_data().get_z_max()
//     - Controller::get_model_data()->get_obj_data().get_z_min();

//     double max = 0;
//       if (x > y && x > z) {
//         max = x;
//       } else if (y > x && y > z) {
//         max = y;
//       } else {
//         max = z;
//       }
//     double scale = (0.8 - (0.8 * (-1))) / max;
//     for (unsigned int i = 0; i < vertecesTest.size(); i++) {
//           vertecesTest[i][0] = vertecesTest[i][0] * scale;
//           vertecesTest[i][1] = vertecesTest[i][1] * scale;
//           vertecesTest[i][2] = vertecesTest[i][2] * scale;
//     }
//     ASSERT_EQ(vertecesTest,
//     Controller::get_model_data()->get_obj_data().get_vertex_vector());
//     ASSERT_EQ(edgesTest,
//     Controller::get_model_data()->get_obj_data().get_edges_vector());
//     Controller::get_model_data()->get_obj_data().clear_data();
// };

TEST(Controller, c1) {
  int vertexes = 0, edges = 0;
  string path = "tests/figure/test.obj";

  EXPECT_NO_THROW(Controller::upload_model(vertexes, edges, path));
}

TEST(Controller, c2) {
  EXPECT_NO_THROW(Controller::change_vertex_color(0, 0, 0));
}

TEST(Controller, c3) { EXPECT_NO_THROW(Controller::change_vertex_size(1)); }

TEST(Controller, c4) { EXPECT_NO_THROW(Controller::change_vertex_type(0)); }

TEST(Controller, c5) {
  EXPECT_NO_THROW(Controller::change_edge_color(0, 0, 0));
}

TEST(Controller, c6) { EXPECT_NO_THROW(Controller::change_edge_width(1)); }

TEST(Controller, c7) { EXPECT_NO_THROW(Controller::change_edge_type(0)); }

TEST(Controller, c8) {
  EXPECT_NO_THROW(Controller::change_background_color(0, 0, 0));
}

TEST(Controller, c9) { EXPECT_NO_THROW(Controller::change_projection(false)); }

TEST(Controller, c10) { EXPECT_NO_THROW(Controller::move_model(0.1, 0.1)); }

TEST(Controller, c11) { EXPECT_NO_THROW(Controller::rotate_model(0.1, 0.1)); }

TEST(Controller, c12) { EXPECT_NO_THROW(Controller::scale_in_model()); }

TEST(Controller, c13) { EXPECT_NO_THROW(Controller::scale_out_model()); }

TEST(Controller, c14) {
  EXPECT_NO_THROW(Controller::save_memento());
  EXPECT_NO_THROW(Controller::load_memento());
}

TEST(Controller, c15) {
  int vertexes = 0, edges = 0;
  string path = "tests/figure/empty.obj";

  EXPECT_NO_THROW(Controller::upload_model(vertexes, edges, path));
}

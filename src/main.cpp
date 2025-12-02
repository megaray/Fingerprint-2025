#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "fingerprint.hpp"
#include "helper.hpp"
#include "test_helper.hpp"

void test_get_neighbours_1();
void test_get_neighbours_2();
void test_black_neighbours();
void test_transitions();
void test_identical_true();
void test_identical_false();
void test_thinning_step_single_pixel();
void test_connected_pixels_1();
void test_connected_pixels_2();
void test_connected_pixels_3();
void test_orientation();
void test_compute_slope_horizontal();
void test_compute_angle_horizontal();
void test_extract_single_minutia();
void test_apply_rotation();
void test_apply_translation();
void test_thin();
void test_with_skeleton();
void test_apply_transformation_single();
void test_apply_transformation_list();
void test_matching_minutiae_count_basic();
void test_match_succeeds_with_translation();
void test_match_insufficient_minutiae();
void test_draw_skeleton(const std::string &name);
void test_draw_minutiae(const std::string &name);
void test_compare_fingerprints(const std::string &name_1, const std::string &name_2, bool expected_result);
void test_compare_all_fingerprints(const std::string &name, size_t finger, bool expected_result);

void test_part_1();
void test_part_2();
void test_part_3();

int main(int argc, char *argv[]) {
  // set verbose if "-v" is passed as an argument
  verbose = false; // verbose is declared in test_helper.hpp
  for (size_t i(0); i < static_cast<size_t>(argc); ++i) {
    if (std::string(argv[i]) == "-v") verbose = true;
  }

  std::cout << "Uncomment the function calls to test your implementation." << std::endl;
  std::cout << "The provided tests are not complete. You have to write your own tests." << std::endl;

  // TODO: uncomment when ready to test
  test_part_1();
  test_part_2();
  //test_part_3();

  // TODO: write more tests


  return 0;
}

void test_part_1() {
  print_part_header(1);
  /* Warning: the tests test_thin and test_draw_skeleton
     can be slow if the code is faulty.
  */
  // TODO: uncomment when ready to test

   test_get_neighbours_1();
   test_get_neighbours_2();
   test_black_neighbours();
   test_transitions();
   test_identical_true();
   test_identical_false();
   test_thinning_step_single_pixel();
   test_thin();
   test_draw_skeleton("1_1");
   test_draw_skeleton("1_2");
   test_draw_skeleton("2_1");
}

void test_part_2() {
  print_part_header(2);

  /* Warning: the test test_draw_minutiae
     can be slow if the code is faulty.
  */

  // TODO: uncomment when ready to test

   test_connected_pixels_1();
   test_connected_pixels_2();
   test_connected_pixels_3();
  // test_compute_slope_horizontal();
  // test_compute_angle_horizontal();

  // test_orientation();
  // test_extract_single_minutia();

  // test_with_skeleton();

  // print_test_header("draw_minutiae"); // header for all three following tests
  // test_draw_minutiae("1_1");
  // test_draw_minutiae("1_2");
  // test_draw_minutiae("2_1");
}

void test_part_3() {
  print_part_header(3);
  /* Warning: the test test_compare_fingerprints is slow even if
     the code is correct because it does many comparisons.
  */

  // TODO: uncomment when ready to test

  // test_apply_rotation();
  // test_apply_translation();
  // test_apply_transformation_single();
  // test_apply_transformation_list();

  // print_test_header("compare fingerprints"); // header for all following compare fingerprints tests

  // test_compare_fingerprints("1_1", "1_2", true);
  // test_compare_fingerprints("1_1", "2_1", false);

  // test_compare_all_fingerprints("1_1", 1, true);
  // test_compare_all_fingerprints("1_1", 2, false);

  // for (size_t f(3); f <= 16; ++f) {
  //  test_compare_all_fingerprints("1_1", f, false);
  // }

  // test_matching_minutiae_count_basic();
  // test_match_succeeds_with_translation();
  // test_match_insufficient_minutiae();
}

void test_get_neighbours_1() {
  print_test_header(__func__); // __func__ gives the current function name

  BinaryImage image({{true}});

  std::vector<bool> neighbours(get_neighbours(image, 0, 0));
  std::vector<bool> expected({false, false, false, false, false, false, false, false});

  print_assertion_result(neighbours == expected, vector_to_string(expected), vector_to_string(neighbours));
}

void test_get_neighbours_2() {
  print_test_header(__func__);

  BinaryImage image_2({{true, true}});

  std::vector<bool> neighbours(get_neighbours(image_2, 0, 0));
  std::vector<bool> expected({false, false, true, false, false, false, false, false});

  print_assertion_result(neighbours == expected, vector_to_string(expected), vector_to_string(neighbours));
}

void test_black_neighbours() {
  print_test_header(__func__);

  std::vector<bool> neighbours({true, false, true, false, false, false, true, false});
  unsigned int expected(3);
  unsigned int result(black_neighbours(neighbours));

  print_assertion_result(result == expected, std::to_string(expected), std::to_string(result));
}

void test_transitions() {
  print_test_header(__func__);

  std::vector<bool> neighbours({false, false, true, false, false, false, false, false});
  unsigned int expected(1);
  unsigned int result(transitions(neighbours));

  print_assertion_result(result == expected, std::to_string(expected), std::to_string(result));
}

void test_identical_true() {
  print_test_header(__func__);

  BinaryImage image({{true, false}, {false, true}});
  bool result(identical(image, image));

  print_assertion_result(result, bool_to_string(true), bool_to_string(result));
}

void test_identical_false() {
  print_test_header(__func__);

  BinaryImage image_1({{true, false}, {false, true}});
  BinaryImage image_2({{true, true}, {false, true}});
  bool result(identical(image_1, image_2));

  print_assertion_result(!result, bool_to_string(false), bool_to_string(result));
}

void test_thinning_step_single_pixel() {
  print_test_header(__func__);

  BinaryImage image({{true}});
  BinaryImage expected(image);

  BinaryImage step0(thinning_step(image, 0));
  print_assertion_result(step0 == expected, vector_to_string(expected), vector_to_string(step0));

  BinaryImage step1(thinning_step(image, 1));
  print_assertion_result(step1 == expected, vector_to_string(expected), vector_to_string(step1));
}

void test_connected_pixels_1() {
  print_test_header(__func__);

  BinaryImage image({
    {true, false, false, true},
    {false, false, true, true},
    {false, true, true, false},
    {false, false, false, false}
  });

  BinaryImage expected({
    {false, false, false, true},
    {false, false, true, true},
    {false, true, true, false},
    {false, false, false, false}
  });

  BinaryImage connected(connected_pixels(image, 2, 1, 10));

  print_assertion_result(connected == expected, vector_to_string(expected), vector_to_string(connected));
}

void test_connected_pixels_2() {
  print_test_header(__func__);

  BinaryImage image({
    {true, false, false, true},
    {false, false, true, true},
    {false, true, true, false},
    {false, false, false, false}
  });

  BinaryImage expected({
    {false, false, false, false},
    {false, false, true, false},
    {false, true, true, false},
    {false, false, false, false}
  });

  BinaryImage connected(connected_pixels(image, 2, 1, 1));

  print_assertion_result(connected == expected, vector_to_string(expected), vector_to_string(connected));
}

void test_connected_pixels_3() {
  print_test_header(__func__);

  BinaryImage image({
    {true, false, false, true, true},
    {true, false, true, true, false},
    {true, true, false, false, false},
    {false, true, false, true, false}
  });

  BinaryImage expected({
    {true, false, false, true, false},
    {true, false, true, true, false},
    {true, true, false, false, false},
    {false, true, false, false, false}
  });

  BinaryImage connected(connected_pixels(image, 2, 1, 2));

  print_assertion_result(connected == expected, vector_to_string(expected), vector_to_string(connected));
}

void test_compute_slope_horizontal() {
  print_test_header(__func__);

  BinaryImage connected({
    {false, false, false},
    {false, true, true},
    {false, false, false}
  });

  double expected(0.0);
  double result(compute_slope(connected, 1, 1));

  print_assertion_result(result == expected, std::to_string(expected), std::to_string(result));
}

void test_compute_angle_horizontal() {
  print_test_header(__func__);

  BinaryImage connected({
    {false, false, false},
    {false, true, true},
    {false, false, false}
  });

  double slope(compute_slope(connected, 1, 1));
  double expected(0.0);
  double result(compute_angle(connected, 1, 1, slope));

  print_assertion_result(result == expected, std::to_string(expected), std::to_string(result));
}

void test_orientation() {
  print_test_header(__func__);

  BinaryImage image({
    {true, false, false, true},
    {false, false, true, true},
    {false, true, true, false},
    {false, false, false, false}
  });

  int expected(35);
  int angle(compute_orientation(image, 2, 1, 3));

  print_assertion_result(angle == expected, std::to_string(expected), std::to_string(angle));
}

void test_extract_single_minutia() {
  print_test_header(__func__);

  BinaryImage skeleton({
    {false, false, false},
    {false, true, true},
    {false, false, false}
  });

  std::vector<Minutia> minutiae(extract(skeleton));
  std::vector<Minutia> expected({{1, 1, 0}});

  bool passes(minutiae.size() == expected.size() && minutiae[0] == expected[0]);

  print_assertion_result(passes, minutiae_to_string(expected), minutiae_to_string(minutiae));
}

void test_apply_rotation() {
  print_test_header(__func__);

  Minutia minutia({1, 3, 10});

  Minutia expected;
  Minutia result;

  result = apply_rotation(minutia, 0, 0, 0);
  expected = minutia;

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));

  result = apply_rotation(minutia, 10, 5, 0);
  expected = minutia;

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));

  result = apply_rotation(minutia, 0, 0, 90);
  expected = {-3, 1, 100};

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));

  result = apply_rotation({0, 3, 10}, 0, 0, 90);
  expected = {-3, 0, 100};

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));

  result = apply_rotation({3, 0, 10}, 0, 0, 90);
  expected = {0, 3, 100};

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));
}

void test_apply_translation() {
  print_test_header(__func__);

  Minutia minutia({1, 3, 10});

  Minutia expected;
  Minutia result;

  result = apply_translation(minutia, 0, 0);
  expected = minutia;

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));

  result = apply_translation(minutia, 10, 5);
  expected = {-9, -2, 10};

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));
}

void test_apply_transformation_single() {
  print_test_header(__func__);

  Minutia minutia({1, 3, 10});
  Minutia expected({-13, -4, 100});

  Minutia result(apply_transformation(minutia, 0, 0, 10, 5, 90));

  print_assertion_result(result == expected, minutia_to_string(expected), minutia_to_string(result));
}

void test_apply_transformation_list() {
  print_test_header(__func__);

  std::vector<Minutia> minutiae({{1, 2, 10}, {3, 4, 20}});
  std::vector<Minutia> expected({{0, 1, 190}, {-2, -1, 200}});

  std::vector<Minutia> result(apply_transformation(minutiae, 1, 2, 1, 1, 180));

  bool passes(result.size() == expected.size());
  for (size_t i(0); passes && i < result.size(); ++i) {
    passes = passes && (result[i] == expected[i]);
  }

  print_assertion_result(passes, minutiae_to_string(expected), minutiae_to_string(result));
}

void test_thin() {
    print_test_header(__func__);

    std::string fingerprint_filename("resources/test_inputs/1_1_small.png");
    BinaryImage image(read_binary(fingerprint_filename));

    std::string expected_thin_filename("resources/test_outputs/skeleton_1_1_small.png");
    std::string actual_thin_filename("skeleton_1_1_small.png");

    BinaryImage skeleton(thin(image));

    BinaryImage expected_skeleton(argb_to_binary(read_argb(expected_thin_filename)));

    print_file_comparison_result(skeleton == expected_skeleton, expected_thin_filename, actual_thin_filename);
    write_argb(binary_diff_to_argb(skeleton, expected_skeleton), actual_thin_filename);

}

void test_draw_skeleton(const std::string &name){
    print_test_header(__func__);

    std::string fingerprint_filename("resources/fingerprints/" + name + ".png");
    std::string expected_thin_filename("resources/test_outputs/skeleton_" + name + ".png");
    std::string actual_thin_filename("skeleton_" + name + ".png");

    BinaryImage image(read_binary(fingerprint_filename));
    BinaryImage skeleton(thin(image));
    BinaryImage expected_skeleton(argb_to_binary(read_argb(expected_thin_filename)));

    print_file_comparison_result(skeleton == expected_skeleton, expected_thin_filename, actual_thin_filename);
    write_argb(binary_diff_to_argb(skeleton, expected_skeleton), actual_thin_filename);
}

void test_draw_minutiae(const std::string &name) {
  BinaryImage image(read_binary("resources/fingerprints/" + name + ".png"));
  BinaryImage skeleton(thin(image));
  std::vector<Minutia> minutiae(extract(skeleton));

  // draw the minutiae on top of the thinned image
  ARGBImage color_image_skeleton(binary_to_argb(skeleton));
  draw_minutia(color_image_skeleton, minutiae);
  write_argb(color_image_skeleton, "minutiae_" + name + ".png");
  std::cout << "Visual test: open the file minutiae_"
                   + name + ".png and check that the minutiae (red circles) are placed correctly"  << std::endl;
  std::cout << "The image should be identical to the one in the resources/test_outputs/minutiae_" + name + ".png file"  << std::endl;
}

void test_with_skeleton() {
  print_test_header(__func__);

  BinaryImage skeleton(read_binary("resources/test_inputs/skeleton_test.png"));
  std::vector<Minutia> minutiae(extract(skeleton));
  std::vector<Minutia> expected({{39, 21, 264}, {53, 33, 270}});

  bool passes(
    minutiae.size() == expected.size()
      && (
        (minutiae[0] == expected[0] && minutiae[1] == expected[1])
          || (minutiae[0] == expected[1] && minutiae[1] == expected[0])
      )
  );
  print_assertion_result(passes, minutiae_to_string(expected), minutiae_to_string(minutiae));

  // draw the minutiae on top of the thinned image
  ARGBImage color_image_skeleton(binary_to_argb(skeleton));
  draw_minutia(color_image_skeleton, minutiae);
  write_argb(color_image_skeleton, "minutiae_skeleton_test.png");
}

void test_compare_fingerprints(const std::string &name_1, const std::string &name_2, bool expected_result) {
  BinaryImage image_1(read_binary("resources/fingerprints/" + name_1 + ".png"));
  BinaryImage skeleton_1(thin(image_1));
  std::vector<Minutia> minutiae_1(extract(skeleton_1));

  BinaryImage image_2(read_binary("resources/fingerprints/" + name_2 + ".png"));
  BinaryImage skeleton_2(thin(image_2));
  std::vector<Minutia> minutiae_2(extract(skeleton_2));

  // debug like this:

  //write_binary(skeleton_1, "skeleton_" + name_1 + ".png");
  //std::cout << minutiae_to_string(minutiae_1);

  //ARGBImage color_image_skeleton_1(binary_to_argb(skeleton_1));
  //draw_minutia(color_image_skeleton_1, minutiae_1);
  //write_argb(color_image_skeleton_1, "./minutiae_" + name_1 + ".png");

  //ARGBImage color_image_skeleton_2(binary_to_argb(skeleton_2));
  //draw_minutia(color_image_skeleton_2, minutiae_2);
  //write_argb(color_image_skeleton_2, "./minutiae_" + name_2 + ".png");

  std::cout << "Compare " << name_1 << " with " << name_2 << std::endl;

  bool is_match(match(minutiae_1, minutiae_2));

  print_assertion_result(is_match == expected_result, std::to_string(expected_result), std::to_string(is_match));
}

void test_compare_all_fingerprints(const std::string &name, size_t finger, bool expected_result) {
  for (size_t i(1); i <= 8; ++i) {
    std::ostringstream ss;
    ss << finger << "_" << i;
    test_compare_fingerprints(name, ss.str(), expected_result);
  }
}

void test_matching_minutiae_count_basic() {
  print_test_header(__func__);

  std::vector<Minutia> minutiae_1({{0, 0, 0}, {5, 5, 20}});
  std::vector<Minutia> minutiae_2({{1, 1, 5}, {10, 10, 40}});

  unsigned int expected(1);
  unsigned int result(matching_minutiae_count(minutiae_1, minutiae_2, 2, 10));

  print_assertion_result(result == expected, std::to_string(expected), std::to_string(result));
}

void test_match_succeeds_with_translation() {
  print_test_header(__func__);

  std::vector<Minutia> minutiae_1;
  std::vector<Minutia> minutiae_2;

  for (int i(0); i < 20; ++i) {
    minutiae_1.push_back({i, i, 90});
    minutiae_2.push_back({i + 2, i + 2, 90});
  }

  bool result(match(minutiae_1, minutiae_2));

  print_assertion_result(result, bool_to_string(true), bool_to_string(result));
}

void test_match_insufficient_minutiae() {
  print_test_header(__func__);

  std::vector<Minutia> minutiae_1({{0, 0, 0}, {1, 1, 0}});
  std::vector<Minutia> minutiae_2({{0, 0, 0}, {1, 1, 0}});

  bool result(match(minutiae_1, minutiae_2));

  print_assertion_result(!result, bool_to_string(false), bool_to_string(result));
}

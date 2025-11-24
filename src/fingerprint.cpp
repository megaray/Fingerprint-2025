#include "fingerprint.hpp"

#include <stdexcept> // for std::invalid_argument

#include "helper.hpp"
#include "utils.hpp" // for NotImplemented()

// provided constants
constexpr unsigned int ORIENTATION_DISTANCE(16);
constexpr int MATCH_ANGLE_OFFSET(2);
constexpr unsigned int DISTANCE_THRESHOLD(5);
constexpr unsigned int ORIENTATION_THRESHOLD(20);
constexpr unsigned int FOUND_THRESHOLD(20);

std::vector<bool> get_neighbours(const BinaryImage &binary_image, size_t row, size_t column) {
  NotImplemented(); // TODO: implement and remove this line
}

unsigned int black_neighbours(std::vector<bool> neighbours) {
  NotImplemented(); // TODO: implement and remove this line
}

unsigned int transitions(std::vector<bool> neighbours) {
  NotImplemented(); // TODO: implement and remove this line
}

bool identical(const BinaryImage &binary_image_1, const BinaryImage &binary_image_2) {
  NotImplemented(); // TODO: implement and remove this line
}

BinaryImage thinning_step(const BinaryImage &binary_image, int step) {
  NotImplemented(); // TODO: implement and remove this line
}

BinaryImage thin(const BinaryImage &binary_image) {
  NotImplemented(); // TODO: implement and remove this line
}

BinaryImage connected_pixels(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {
  NotImplemented(); // TODO: implement and remove this line
}

double compute_slope(const BinaryImage &connected_pixels, size_t row, size_t column) {
  NotImplemented(); // TODO: implement and remove this line
}

double compute_angle(const BinaryImage &connected_pixels, size_t row, size_t column, double slope) {
  NotImplemented(); // TODO: implement and remove this line
}

int compute_orientation(const BinaryImage &binary_image, size_t row, size_t column, unsigned int distance) {
  NotImplemented(); // TODO: implement and remove this line
}

std::vector<Minutia> extract(const BinaryImage &binary_image) {
  NotImplemented(); // TODO: implement and remove this line
}

Minutia apply_rotation(const Minutia &minutia, int center_row, int center_column, int rotation_in_degrees) {
  NotImplemented(); // TODO: implement and remove this line
}

Minutia apply_translation(const Minutia &minutia, int row_translation, int column_translation) {
  NotImplemented(); // TODO: implement and remove this line
}

Minutia apply_transformation(const Minutia &minutia, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
  NotImplemented(); // TODO: implement and remove this line
}

std::vector<Minutia> apply_transformation(const std::vector<Minutia> &minutiae, int center_row, int center_column, int row_translation, int column_translation, int rotation_in_degrees) {
  NotImplemented(); // TODO: implement and remove this line
}

unsigned int matching_minutiae_count(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2, unsigned int max_distance, unsigned int max_orientation) {
  NotImplemented(); // TODO: implement and remove this line
}

bool match(const std::vector<Minutia> &minutiae_1, const std::vector<Minutia> &minutiae_2) {
  NotImplemented(); // TODO: implement and remove this line
}


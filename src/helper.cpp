#include "helper.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "fingerprint.hpp"

constexpr int CHANNEL_NUM_ARGB(STBI_rgb_alpha);

constexpr double LUMA_COEFFICIENT_RED(0.299);
constexpr double LUMA_COEFFICIENT_GREEN(0.587);
constexpr double LUMA_COEFFICIENT_BLUE(0.114);
const std::string FILE_PREFIX("../");

bool exists(const std::string &filename) {
    std::ifstream f(filename.c_str());
    return f.good();
}

ARGBImage read_argb(const std::string &filename_) {
    std::string filename(FILE_PREFIX+filename_);

    if (!exists(filename)) throw std::runtime_error("File does not exist: " + filename);

    //std::cout << "Info: reading file " << filename << std::endl;

    int width_signed, height_signed, bpp;

    uint8_t *rgba_image(stbi_load(filename.c_str(), &width_signed, &height_signed, &bpp, CHANNEL_NUM_ARGB));

    size_t width(static_cast<size_t>(width_signed));
    size_t height(static_cast<size_t>(height_signed));

    ARGBImage argb_image(std::vector<std::vector<uint32_t>>(height, std::vector<uint32_t>(width)));

    uint8_t *iterator(rgba_image);
    for (size_t i(0); i < height; ++i) {
        for (size_t j(0); j < width; ++j) {
            uint32_t rgba(0);
            for (int c(CHANNEL_NUM_ARGB - 1); c >= 0; --c) {
                uint8_t value(*iterator);
                rgba = rgba + (static_cast<uint32_t>(value) << (8 * c));
                iterator++;
            }

            // convert rgba to argb
            // NOTE: if using int instead of uint32_t, this does arithmetic shift
            uint32_t argb((rgba >> 8) | ((rgba & 0xFF) << 8 * 3));

            argb_image[i][j] = argb;
        }
    }

    stbi_image_free(rgba_image);
    return argb_image;
}

BinaryImage read_binary(const std::string &filename) {
  return argb_to_binary(read_argb(filename));
}

void write_argb(const ARGBImage &argb_image, const std::string &filename) {
    //std::cout << "Info: writing file " << filename << std::endl;

    size_t height(argb_image.size());
    size_t width(argb_image[0].size());

    uint8_t *rgba_image((uint8_t *)malloc(width * height * CHANNEL_NUM_ARGB));

    uint8_t *iterator(rgba_image);
    for (size_t i(0); i < height; ++i) {
        for (size_t j(0); j < width; ++j) {
            uint32_t argb(argb_image[i][j]);

            // convert argb to rgba
            // NOTE: if using int instead of uint32_t, this does arithmetic shift
            uint32_t rgba((argb << 8) | (argb >> 8 * 3));

            for (int c(CHANNEL_NUM_ARGB - 1); c >= 0; --c) {
                uint8_t value((rgba >> (8 * c)) & 0xFF);
                *iterator = value;
                iterator++;
            }
        }
    }

    stbi_write_png(filename.c_str(), static_cast<int>(width), static_cast<int>(height), CHANNEL_NUM_ARGB, rgba_image, static_cast<int>(width) * CHANNEL_NUM_ARGB);
    stbi_image_free(rgba_image);
}

void write_binary(const BinaryImage &binary_image, const std::string &filename) {
    write_argb(binary_to_argb(binary_image), filename);
}

uint8_t luminance(uint32_t argb) {
  uint8_t red((argb >> 16) & 0xFF);
  uint8_t green((argb >> 8) & 0xFF);
  uint8_t blue(argb & 0xFF);
  return static_cast<uint8_t>(std::round(LUMA_COEFFICIENT_RED * red + LUMA_COEFFICIENT_GREEN * green + LUMA_COEFFICIENT_BLUE * blue));
}

BinaryImage argb_to_binary(const ARGBImage &argb_image) {
    size_t height(argb_image.size());
    size_t width(argb_image[0].size());

    BinaryImage binary_image(std::vector<std::vector<bool>>(height, std::vector<bool>(width)));

    for (size_t i(0); i < height; i++) {
      for (size_t j(0); j < width; j++) {
        binary_image[i][j] = luminance(argb_image[i][j]) < 128;
      }
    }

    return binary_image;
}

ARGBImage binary_diff_to_argb(const BinaryImage &binary_image1, const BinaryImage &binary_image2) {
    size_t height(binary_image1.size());
    size_t width(binary_image1[0].size());

    ARGBImage argb_image(std::vector<std::vector<uint32_t>>(height, std::vector<uint32_t>(width)));

    for (size_t i(0); i < height; i++) {
        for (size_t j(0); j < width; j++) {
            if (binary_image1[i][j] != binary_image2[i][j]) argb_image[i][j] = 0xFFFF0000;
            else
                argb_image[i][j] = binary_image1[i][j] ? 0xFF000000 : 0xFFFFFFFF;
        }
    }

    return argb_image;
}

ARGBImage binary_to_argb(const BinaryImage &binary_image) {
    size_t height(binary_image.size());
    size_t width(binary_image[0].size());

    ARGBImage argb_image(std::vector<std::vector<uint32_t>>(height, std::vector<uint32_t>(width)));

    for (size_t i(0); i < height; i++) {
        for (size_t j(0); j < width; j++) {
            argb_image[i][j] = binary_image[i][j] ? 0xFF000000 : 0xFFFFFFFF;
        }
    }

    return argb_image;
}

double degrees_to_radians(double degrees) {
  return degrees * PI / 180.0;
}

double radians_to_degrees(double radians) {
  return radians * 180.0 / PI;
}

void add_line(ARGBImage &argb_image, int row_start, int column_start, double orientation, unsigned int length, uint32_t color) {
  size_t height(argb_image.size());
  size_t width(argb_image[0].size());

  int row_end(static_cast<int>(std::round(row_start - static_cast<int>(length) * std::sin(orientation))));
  int column_end(static_cast<int>(std::round(column_start + static_cast<int>(length) * std::cos(orientation))));

  int d_column(column_end - column_start);
  int d_row(row_end - row_start);

  int s_column(d_column < 0 ? -1 : 1);
  int s_row(d_row < 0 ? -1 : 1);

  int row(row_start);
  int column(column_start);

  if (std::abs(d_row) <= std::abs(d_column)) {
    double slope(static_cast<double>(d_row) / d_column);
    double pitch(row_start - slope * column_start);

    while (column != column_end) {
      int r(static_cast<int>(std::round(slope * column + pitch)));

      if (0 <= r && r < static_cast<int>(height) && 0 <= column && column < static_cast<int>(width))
        argb_image[static_cast<size_t>(r)][static_cast<size_t>(column)] = color;

      column += s_column;
    }
  } else {
    double slope(static_cast<double>(d_column) / d_row);
    double pitch(column_start - slope * row_start);

    while (row != row_end) {
      int c(static_cast<int>(std::round(slope * row + pitch)));

      if (0 <= row && row < static_cast<int>(height) && 0 <= c && c < static_cast<int>(width))
        argb_image[static_cast<size_t>(row)][static_cast<size_t>(c)] = color;

      row += s_row;
    }
  }

  if (0 <= row_end && row_end < static_cast<int>(height) && 0 <= column_end && column_end < static_cast<int>(width))
    argb_image[static_cast<size_t>(row_end)][static_cast<size_t>(column_end)] = color;
}

void add_circle(ARGBImage &argb_image, int row_center, int column_center, unsigned int radius, uint32_t color) {
  size_t height(argb_image.size());
  size_t width(argb_image[0].size());

  for (unsigned int i(0); i < 360; ++i) {
    int row = row_center + static_cast<int>(std::round(static_cast<int>(radius) * std::sin(degrees_to_radians(i))));
    int column = column_center + static_cast<int>(std::round(static_cast<int>(radius) * std::cos(degrees_to_radians(i))));

    if (0 <= row && row < static_cast<int>(height) && 0 <= column && column < static_cast<int>(width))
      argb_image[static_cast<size_t>(row)][static_cast<size_t>(column)] = color;
  }
}

void draw_minutia(ARGBImage &argb_image, const std::vector<Minutia> &minutiae) {
  for (const Minutia &minutia : minutiae) {
    add_circle(argb_image, minutia.row, minutia.column, 5, 0xFFFF0000);
    add_line(argb_image, minutia.row, minutia.column, degrees_to_radians(minutia.angle_in_degrees), 8, 0xFFFF0000);
  }
}


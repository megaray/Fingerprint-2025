#include "test_helper.hpp"

#include <algorithm> // for std::max
#include <iostream>
#include <sstream> // for std::ostringstream
#include <stdexcept>

bool verbose(false);

// define equality for Minutia
bool operator==(const Minutia &minutia_1, const Minutia &minutia_2)
{
    return minutia_1.row == minutia_2.row && minutia_1.column == minutia_2.column
           && minutia_1.angle_in_degrees == minutia_2.angle_in_degrees;
}

bool operator==(const BinaryImage &image_1, const BinaryImage &image_2)
{
    if (image_1.size() != image_2.size())
        return false;
    for (size_t row(0); row < image_1.size(); ++row) {
        if (image_1[row].size() != image_2[row].size())
            return false;
        for (size_t col(0); col < image_1[row].size(); ++col) {
            if (image_1[row][col] != image_2[row][col])
                return false;
        }
    }
    return true;
}

std::string bool_to_string(bool boolean)
{
    return boolean ? "true" : "false";
}

std::string vector_to_string(const std::vector<std::vector<bool>> &vec)
{
    std::ostringstream ss;
    ss << "{";

    for (size_t i(0); i < vec.size(); ++i) {
        if (i != 0)
            ss << ",";

        ss << std::endl << "  " << vector_to_string(vec[i]);
    }

    ss << std::endl << "}";
    return ss.str();
}

std::string vector_to_string(const std::vector<bool> &vec)
{
    std::ostringstream ss;
    ss << "{";

    for (size_t i(0); i < vec.size(); ++i) {
        if (i != 0)
            ss << ", ";
        ss << vec[i];
    }

    ss << "}";
    return ss.str();
}

std::string minutia_to_string(const Minutia &minutia)
{
    std::ostringstream ss;
    ss << "{row: " << minutia.row << ", column: " << minutia.column
       << ", angle_in_degrees: " << minutia.angle_in_degrees << "}" << std::endl;
    return ss.str();
}

std::string minutiae_to_string(const std::vector<Minutia> &minutiae, const std::string &prefix)
{
    std::ostringstream ss;

    for (const Minutia &minutia : minutiae) {
        ss << prefix;
        ss << minutia_to_string(minutia);
    }
    return ss.str();
}

void print_box(const std::string &text,
               const std::string &top_left,
               const std::string &top_right,
               const std::string &bottom_left,
               const std::string &bottom_right,
               const std::string &horizontal,
               const std::string &vertical,
               size_t min_width = 50)
{
    size_t unpadded_width(text.size() + 2); // text with single spaces around the text

    size_t width(std::max(min_width, unpadded_width));
    size_t padding(width - unpadded_width);

    // top
    std::cout << top_left;
    for (size_t i(0); i < width; ++i)
        std::cout << horizontal;
    std::cout << top_right << std::endl;

    // text line with padding
    std::cout << vertical;
    std::cout << " " << text << " ";
    for (size_t i(0); i < padding; ++i)
        std::cout << " ";
    std::cout << vertical << std::endl;

    // bottom
    std::cout << bottom_left;
    for (size_t i(0); i < width; ++i)
        std::cout << horizontal;
    std::cout << bottom_right << std::endl;
}

void print_part_header(unsigned int part)
{
    if (part < 1 || part > 3)
        throw std::invalid_argument("Invalid part number");
    print_box("Testing Part " + std::to_string(part), "╔", "╗", "╚", "╝", "═", "║");
}

void print_test_header(const std::string &name)
{
    print_box("Running Test: " + name, "┌", "┐", "└", "┘", "─", "│");
}

void print_assertion_result(bool passes, const std::string &expected, const std::string &actual)
{
    if (passes)
        std::cout << "--- OK ---" << std::endl;
    else
        std::cout << "--- FAIL ---" << std::endl;

    if (!passes || verbose) {
        std::cout << std::endl;

        std::cout << "Expected:" << std::endl;
        std::cout << expected << std::endl;

        std::cout << std::endl;

        std::cout << "Actual:" << std::endl;
        std::cout << actual << std::endl;

        std::cout << std::endl;
    }
}

void print_file_comparison_result(bool passes,
                                  const std::string &expected_filename,
                                  const std::string &actual_filename)
{
    print_assertion_result(passes,
                           "the contents of image " + expected_filename,
                           "the contents of image " + actual_filename);
}

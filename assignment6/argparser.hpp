#ifndef ASSIGNMENT1_ARGPARSER_HPP
#define ASSIGNMENT1_ARGPARSER_HPP
struct Options {
    const char *input_file = nullptr;
    int width = 100;
    int height = 100;
    const char *output_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    const char *depth_file = nullptr;
    const char *normal_file = nullptr;
    bool shade_back = false;
    bool gui = false;
    bool gouraud = false;
    int tessellation[2]{100, 100};
    bool shadows = false;
    int max_bounces = 0;
    float cutoff_weight = 0.0f;
    bool visualize_grid = false;
    int grid_size[3];
    bool stats = false;
};
void parse_args(int argc, const char **argv);
const Options &getOptions();
#endif /* ASSIGNMENT1_ARGPARSER_HPP */

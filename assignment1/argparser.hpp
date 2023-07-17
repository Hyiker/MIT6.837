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
};
Options parse_args(int argc, const char **argv);
#endif /* ASSIGNMENT1_ARGPARSER_HPP */

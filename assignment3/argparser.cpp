#include "argparser.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>

static Options options{};
void parse_args(int argc, const char **argv) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert(i < argc);
            options.input_file = argv[i];
            // for convenience, change pwd to input file's directory
            std::filesystem::current_path(
                std::filesystem::path(options.input_file).parent_path());
        } else if (!strcmp(argv[i], "-size")) {
            i++;
            assert(i < argc);
            options.width = atoi(argv[i]);
            i++;
            assert(i < argc);
            options.height = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-output")) {
            i++;
            assert(i < argc);
            options.output_file = argv[i];
        } else if (!strcmp(argv[i], "-depth")) {
            i++;
            assert(i < argc);
            options.depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            options.depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            options.depth_file = argv[i];
        } else if (!strcmp(argv[i], "-normals")) {
            i++;
            assert(i < argc);
            options.normal_file = argv[i];
        } else if (!strcmp(argv[i], "-shade_back")) {
            options.shade_back = true;
        } else if (!strcmp(argv[i], "-gui")) {
            options.gui = true;
        } else if (!strcmp(argv[i], "-tessellation")) {
            i++;
            assert(i < argc);
            options.tessellation[0] = atoi(argv[i]);
            i++;
            assert(i < argc);
            options.tessellation[1] = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            options.gouraud = true;
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i,
                   argv[i]);
            assert(0);
        }
    }
}
const Options &getOptions() { return options; }
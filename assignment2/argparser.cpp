#include "argparser.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
Options parse_args(int argc, const char **argv) {
    Options options{};
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert(i < argc);
            options.input_file = argv[i];
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
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i,
                   argv[i]);
            assert(0);
        }
    }
    return options;
}
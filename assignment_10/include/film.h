#ifndef _FILM_H_
#define _FILM_H_

#include <assert.h>

#include <memory>

#include "filter.hpp"
#include "sample.h"

class Filter;

// ==================================================================
// A helper class for super-sampling.  Stores a set of samples for
// each pixel of the final image.

class Film {
   public:
    // CONSTRUCTOR & DESTRUCTOR
    Film(int _width, int _height, int _num_samples,
         std::unique_ptr<Filter> filter)
        : m_filter(std::move(filter)) {
        width = _width;
        height = _height;
        num_samples = _num_samples;
        assert(width > 0 && height > 0 && num_samples > 0);
        samples = new Sample[width * height * num_samples];
    }
    ~Film() { delete[] samples; }

    // ACCESSORS
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getNumSamples() { return num_samples; }
    Sample getSample(int i, int j, int n) const {
        return samples[getIndex(i, j, n)];
    }

    // MODIFIERS
    void setSample(int x, int y, int i, Vec2f offset, Vec3f color) {
        samples[getIndex(x, y, i)].set(offset, color);
    }

    int getIndex(int i, int j, int n) const {
        assert(i >= 0 && i < width);
        assert(j >= 0 && j < height);
        assert(n >= 0 && n < num_samples);
        return i * height * num_samples + j * num_samples + n;
    }

    // VISUALIZATION
    void renderSamples(char *samples_file, int sample_zoom);
    void renderFilter(char *filter_file, int filter_zoom, Filter *filter);

    // OUTPUT IMAGE
    void saveImage(const char *filename);

   private:
    Film() { assert(0); }  // don't use this constructor

    // REPRESENTATION
    int width;
    int height;
    int num_samples;
    Sample *samples;
    std::unique_ptr<Filter> m_filter;
};

// ==================================================================

#endif
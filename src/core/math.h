/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

TR_NAMESPACE_BEGIN

/**
 * Computes barycentric coordinates.
 */
template<class T>
inline T barycentric(const T& a, const T& b, const T& c, const float u, const float v) {
    return a * (1 - u - v) + b * u + c * v;
}

/**
 * Restricts a value to a given interval.
 */
template<class T>
inline T clamp(T v, T min, T max) {
    return std::min(std::max(v, min), max);
}

/**
 * Checks if vector is zero.
 */
inline bool isZero(const v3f v) {
    return glm::dot(v, v) < Epsilon;
}

/**
 * Generates coordinate system.
 */
inline void coordinateSystem(const v3f& a, v3f& b, v3f& c) {
    if (std::abs(a.x) > std::abs(a.y)) {
        float invLen = 1.f / std::sqrt(a.x * a.x + a.z * a.z);
        c = v3f(a.z * invLen, 0.f, -a.x * invLen);
    } else {
        float invLen = 1.f / std::sqrt(a.y * a.y + a.z * a.z);
        c = v3f(0.f, a.z * invLen, -a.y * invLen);
    }
    b = glm::cross(c, a);
}

/**
 * Converts RGB value to luminance.
 */
inline float getLuminance(const v3f& rgb) {
    return glm::dot(rgb, v3f(0.212671f, 0.715160f, 0.072169f));
}

/**
 * Pseudo-random sampler (Mersenne Twister 19937) structure.
 */
struct Sampler {
    std::mt19937 g;
    std::uniform_real_distribution<float> d;
    explicit Sampler(int seed) {
        g = std::mt19937(seed);
        d = std::uniform_real_distribution<float>(0.f, 1.f);
    }
    float next() { return d(g); }
    p2f next2D() { return {d(g), d(g)}; }
    void setSeed(int seed) {
        g.seed(seed);
        d.reset();
    }
};

/**
 * 1D discrete distribution.
 */
struct Distribution1D {
    std::vector<float> cdf{0};
    bool isNormalized = false;

    inline void add(float pdfVal) {
        cdf.push_back(cdf.back() + pdfVal);
    }

    size_t size() {
        return cdf.size() - 1;
    }

    float normalize() {
        float sum = cdf.back();
        for (float& v : cdf) {
            v /= sum;
        }
        isNormalized = true;
        return sum;
    }

    inline float pdf(size_t i) const {
        assert(isNormalized);
        return cdf[i + 1] - cdf[i];
    }

    int sample(float sample) const {
        assert(isNormalized);
        const auto it = std::upper_bound(cdf.begin(), cdf.end(), sample);
        return clamp(int(distance(cdf.begin(), it)) - 1, 0, int(cdf.size()) - 2);
    }
};


/**
 * Warping functions.
 */
namespace Warp {


inline v3f squareToUniformSphere(const p2f& sample) {
    v3f v(0.f);
	// TODO: Implement this - DONE
	float z = 1 - 2 * sample.x;
	float r = std::sqrt(std::max((float) 0, (float) 1 - z * z));
	float phi = 2 * M_PI * sample.y;
    return v3f(r * std::cos(phi), r * std::sin(phi), z);
}

inline float squareToUniformSpherePdf() {
	// TODO: Implement this - DONE
    float pdf = INV_FOURPI;
    return pdf;
}

inline v3f squareToUniformHemisphere(const p2f& sample) {
	// TODO: Implement this - DONE
	float z = sample.x;
	float r = std::sqrt(std::max((float) 0, (float) 1. - z * z));
	float phi = 2 * M_PI * sample.y;
	v3f v(r * std::cos(phi), r * std::sin(phi), z);
    return v;
}

inline float squareToUniformHemispherePdf(const v3f& v) {
	// TODO: Implement this - DONE
	float pdf = INV_TWOPI;
    return pdf;
}

inline v2f squareToUniformDiskConcentric(const p2f& sample) {
	// TODO: Implement this (optional) - DONE
	//Map uniform random numbers to [-1, 1]^2
	p2f uOffset = 2.f * sample - v2f(1.0f, 1.0f);
	//Handle degeneracy at the origin
	if(uOffset.x == 0 && uOffset.y == 0) {
	    return v2f(0, 0);
	}
	//Apply concentric mapping to point
    float piOver4 = M_PI * (0.25f);
	float piOver2 = M_PI * (0.5f);
	float theta, r;
	if(std::abs(uOffset.x) > std::abs(uOffset.y)) {
	    r = uOffset.x;
	    theta = piOver4 * (uOffset.y / uOffset.x);
	} else {
	    r = uOffset.y;
	    theta = piOver2 - piOver4 * (uOffset.x / uOffset.y);
	}
	v2f v(r * std::cos(theta), r * std::sin(theta));
    return v;
}

inline v3f squareToCosineHemisphere(const p2f& sample) {
	// TODO: Implement this - DONE
	p2f d = squareToUniformDiskConcentric(sample);
	float z = std::sqrt(std::max((float) 0, 1 - d.x * d.x - d.y * d.y));
	v3f v(d.x, d.y, z);
    return v;
}

inline float squareToCosineHemispherePdf(const v3f& v) {
    float pdf = 0.f;
	// TODO: Implement this - DONE
    float cosTheta = cos(v.y);
    pdf = cosTheta * INV_PI;
    return pdf;
}

inline v3f squareToPhongLobe(const p2f& sample, int n) {
    v3f v(0.f);
	// TODO: Implement this - DONE
	float xP = sample.x;
	float yP = sample.y;
    float theta = acos(pow((1 - xP), 1.0f / (n + 2)));
    float phi = 2 * M_PI * yP;
    v = v3f(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    return v;
}

inline float squareToPhongLobePdf(const v3f& v) {
    float pdf = 0.f;
	// TODO: Implement this
    return pdf;
}

}

TR_NAMESPACE_END
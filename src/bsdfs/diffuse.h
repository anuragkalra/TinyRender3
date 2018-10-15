/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

#include "core/core.h"

TR_NAMESPACE_BEGIN

/**
 * Perfectly diffuse, Lambertian reflectance model
 */
struct DiffuseBSDF : BSDF {
    std::unique_ptr<Texture < v3f>> albedo;

    DiffuseBSDF(const WorldData& scene, const Config& config, const size_t& matID) : BSDF(scene, config, matID) {
        const tinyobj::material_t& mat = scene.materials[matID];

        if (mat.diffuse_texname.empty())
            albedo = std::unique_ptr<Texture<v3f>>(new ConstantTexture3f(glm::make_vec3(mat.diffuse)));
        else
            albedo = std::unique_ptr<Texture<v3f>>(new BitmapTexture3f(config, mat.diffuse_texname));

        components.push_back(EDiffuseReflection);

        combinedType = 0;
        for (size_t i = 0; i < components.size(); ++i)
            combinedType |= components[i];
    }

    v3f eval(const SurfaceInteraction& i) const override {
        v3f val(0.f);
	    // TODO: Add previous assignment code (if needed)
	    // In TinyRender, the albedo rho of a diffuse BRDF is casted as a constant 2D texture
	    // which allows for better integration with Bitmap textures.
	    // albedo->eval(worldData, i) returns the albedo at surface interaction point i
	    // 1. Check that the incoming and outgoing rays are headed in the correct
	    //    directions, if not return black
	    // 2. Otherwise return the evaluated albedo dived by pi, and multiplied
	    //    by the cosine factor cos theta_i

	    // Use the Frame::cosTheta() function on i.wo and i.wi to perform checks
	    if(Frame::cosTheta(i.wo) <= 0 || Frame::cosTheta(i.wi) <= 0) {
	        return val;
	    }

	    return albedo->eval(worldData, i) * INV_PI * Frame::cosTheta(i.wo);
    }

    float pdf(const SurfaceInteraction& i) const override {
        float pdf = 0.f;

        return pdf;
    }

    v3f sample(SurfaceInteraction& i, const v2f& sample, float* pdf) const override {
        v3f val(0.f);

        return val;
    }

    std::string toString() const override { return "Diffuse"; }
};

TR_NAMESPACE_END
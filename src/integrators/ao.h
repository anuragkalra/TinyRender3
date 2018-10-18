/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

TR_NAMESPACE_BEGIN

/**
 * Ambient occlusion integrator
 */
struct AOIntegrator : Integrator {
    explicit AOIntegrator(const Scene& scene) : Integrator(scene) { }

    v3f render(const Ray& ray, Sampler& sampler) const override {
        v3f Li(0.0f);
        //TODO: build logic to change sampling method call depending on importanceSamplingScheme value
        //0 => Uniform Spherical Direction Sampling
        //1 => Uniform Hemispherical Directional Sampling
        //2 => Cosine-weighted Hemispherical Direction Sampling
        int importanceSamplingScheme = 2;

        int sampleCount = 16;
        float maxShadowRayLength = scene.aabb.getBSphere().radius * (0.5f);

        SurfaceInteraction si_initial = SurfaceInteraction();

        bool hit = scene.bvh->intersect(ray, si_initial);   //does ray intersect the scene
        v2f sample = sampler.next2D(); //2d sample from [0, 1] ^ 2

        if(hit){
            v3f direction;
            if(importanceSamplingScheme == 0) { //Uniform Spherical Direction Sampling
                direction = Warp::squareToUniformSphere(sample);
            }
            if (importanceSamplingScheme == 1) {    //Uniform Hemisphere Direction Sampling
                direction = Warp::squareToUniformHemisphere(sample);
            }
            if (importanceSamplingScheme == 2){     //Cosine Hemisphere Sampling
                direction = Warp::squareToCosineHemisphere(sample);
            }

            direction = si_initial.frameNs.toWorld(direction);  //transform direction from local frame to world frame

            //build ray originating at current surface interaction and in direction from sampler
            Ray ray2 = Ray(si_initial.p, direction, Epsilon, maxShadowRayLength);
            if(!scene.bvh->intersect(ray2, si_initial)) {
                float dot = glm::dot(direction, si_initial.frameNs.n);   //cos(theta_i)
                float pdf;
                if(importanceSamplingScheme == 0) { //Uniform Spherical PDF
                    pdf = Warp::squareToUniformSpherePdf();
                }
                if(importanceSamplingScheme == 1) { //Uniform Hemisphere PDF
                    v3f dummy;
                    pdf = Warp::squareToUniformHemispherePdf(dummy);
                }
                if(importanceSamplingScheme == 2) {   //Cosine Hemisphere PDF
                    v3f dummy2;
                    pdf = Warp::squareToCosineHemispherePdf(dummy2);
                }
                Li += glm::max(0.f, dot/pdf);
            }
        }
        return (Li * INV_PI);

    }
};

TR_NAMESPACE_END
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
        int importanceSamplingScheme = 0;

        int sampleCount = 16;
        float maxShadowRayLength = scene.aabb.getBSphere().radius * (0.5f);
        float albedo = 1.0f;
	    // TODO: Implement this

	    //1. Intersect your eye rays with the scene geometry
        SurfaceInteraction si_initial = SurfaceInteraction();

        //2. If there's an intersection i, solve for the appropriate Monte Carlo
	    //   AO estimate at this shade point: you can sample directions in your
	    //   MC estimator using the sampling routines you developed earlier.

        bool hit = scene.bvh->intersect(ray, si_initial);   //does ray intersect the scene
        v2f sample = sampler.next2D(); //2d sample from [0, 1] ^ 2
        //uniform sphere assumed
        //only runs integration loop if there is a hit.
        if(hit){
            v3f direction = Warp::squareToUniformSphere(sample);    //3d direction vector on a sphere
            direction = si_initial.frameNs.toWorld(direction);  //transform direction from local frame to world frame
            Ray ray2 = Ray(si_initial.p, direction, Epsilon, maxShadowRayLength); //build ray originating at current surface interaction and in direction from sphere
            if(!scene.bvh->intersect(ray2, si_initial)) {
                float dot = glm::dot(direction, si_initial.frameNs.n);   //cos(theta_i)
                float pdf = Warp::squareToUniformSpherePdf();
                Li += glm::max(0.f, dot/pdf);
            }
        }
        return (Li * INV_PI);

    }
};

TR_NAMESPACE_END
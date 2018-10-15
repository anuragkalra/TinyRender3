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
        v3f Li(0.f);
	    // TODO: Implement this
	    //1. Intersect your eye rays with the scene geometry
	    //2. If there's an intersection i, solve for the appropriate Monte Carlo
	    //   AO estimate at this shade point: you can sample directions in your
	    //   MC estimator using the sampling routines you developed earlier.
	    //3. When evaluating the visibility in the AO integrand of your MC estimator,
	    //   take care when specifying relevant positions and directions; remember, all
	    //   intersection routines expect world-space coordinates. Here, you will need
	    //   to compute the parameters of a shadow ray based on i.p and i.wi
	    //4. When computing the contribution of each MC sample to the final integral
	    //   estimate, don't forget to evaluate all the integrand terms and to divide
	    //   by the appropriate PDF evaluation
        return Li;
    }
};

TR_NAMESPACE_END
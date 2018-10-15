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

	    //NOTES
	    //->To avoid shadow ray problem (described in handout), set the maximum shadow ray length
	    //  to half of the bounding sphere radius. Use scene.aabb.getBSphere() to retrieve the
	    //  sphere and its corresponding radius.

	    //->The albedo is commonly set to 1.0 for AO (and RO).

	    //->Visually benchmark using cbox scene using each of the three sampling methods. Use 16 spp.
	    //->Can control the MC sampling rate as a parameter of the integrator, looping and averaging
	    //  over your samples directly in the integrator.
        return Li;
    }
};

TR_NAMESPACE_END
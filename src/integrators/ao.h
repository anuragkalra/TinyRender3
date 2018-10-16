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
        SurfaceInteraction si = SurfaceInteraction();

        //2. If there's an intersection i, solve for the appropriate Monte Carlo
	    //   AO estimate at this shade point: you can sample directions in your
	    //   MC estimator using the sampling routines you developed earlier.

        bool hit = scene.bvh->intersect(ray, si);   //does ray intersect the scene

        //uniform sphere assumed
        //only runs integration loop if there is a hit.
        if(hit){
            float radiance;
            int i;
            for(i = 0; i < sampleCount; i++){
                v2f sample = sampler.next2D(); //2d sample from [0, 1] ^ 2
                v3f direction = Warp::squareToUniformSphere(sample);    //3d direction vector on a sphere
                direction = si.frameNs.toWorld(direction);  //transform direction from local frame to world frame
                Ray ray2 = Ray(si.p, direction, Epsilon, maxShadowRayLength); //build ray originating at current surface interaction and in direction from sphere
                float element;
                if(!scene.bvh->intersect(ray2, si)) {     //if there's no hit...V(x, w_i) == 1)
                    float cosValue = Frame::cosTheta(direction);
                    float pdf = Warp::squareToUniformSpherePdf();
                    element = cosValue / pdf;
                }
                radiance += element;
            }
            float finalRadiance = (albedo * (INV_PI) * (1 / sampleCount)) * radiance;
            return v3f(finalRadiance);
        } else {
            return Li;
        }




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

    }
};

TR_NAMESPACE_END
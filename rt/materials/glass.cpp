#include <rt/materials/glass.h>
#include <core/random.h>
#include <core/scalar.h>
#include <cmath>

namespace rt {

    GlassMaterial::GlassMaterial(float eta) :
        m_Eta(eta)
    {
        /* TODO */
    }

    RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {

        return RGBColor::rep(0);
    }

    RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */
        return RGBColor::rep(0.0f);
    }

    float fresnel(float eta,float cosine,float& refrCosine, bool& total) {
        const float sine = sqrt(1.0f-sqrt(cosine));
        // Snell's law
        const float refrSine = sine/eta;

        // Total Internal Reflection
        total = (refrSine>=1.0f);
        if(total){ return 1.0f;}

        //The cosine of the possible refraction
        refrCosine = sqrt(1.0f-sqr(refrSine));

        //parallel Polarization
        const float para = (eta*cosine - refrCosine) / (eta*cosine + refrCosine);

        // perpendicular polarization
        const float perp = (cosine- eta * refrCosine) / (cosine + eta * refrCosine);

        // We do not have polarization , so take the average
        return (sqr(para) + sqr(perp)) / 2.0f ;

    }

    Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */
        Vector norm = normal;
        float cosine = dot(outDir,norm);
        float eta = m_Eta;
        // Flip if below
        if (cosine<0.0f){
            cosine = -cosine;
            norm = -normal;
            eta = 1.0f/m_Eta;
        }

        cosine = fmin(1.0f,cosine);
        bool total;
        float refrCosine = 0;
        const float reflectance = fresnel(eta,cosine,refrCosine,total);

        // Early drop out if total interneal reflection
        if (total){
            SampleReflectance reflection;
            reflection.direction = -outDir + 2 * cosine * norm;
            reflection.reflectance = RGBColor::rep(1.0f);
            return reflection;
        }

        assert (reflectance<=1.0f && reflectance >=0.0f)<<reflectance<<"is beyond the allowed range";

        #ifndef FRESNEL_SAMPLING
            const float prob = 0.5f;
        #else
            const float prob = reflectance;
        #endif

            if (random()<prob){
                SampleReflectance reflection;
                reflection.direction = -outDir + 2 * cosine*norm;
                reflection.reflectance = RGBColor::rep(1.0f)*(reflectance/prob);

                return reflection;
            }else{
                SampleReflectance refraction;
                const Vector outPara = -outDir + cosine*norm;
                const Vector refrPara = outPara/eta;
                const Vector refrParp = -norm * refrCosine;
                refraction.direction = (refrPara + refrParp).normalize();

                const float transmittance = 1 - reflectance;
                const float squeezeFactor = 1/sqr(eta);

                refraction.reflectance = RGBColor::rep(1.0f)* (transmittance*squeezeFactor/(1-prob));

                return refraction;
            }


    }

    Material::Sampling GlassMaterial::useSampling() const {
        /* TODO */
        return(Material::SAMPLING_ALL);
    }

}

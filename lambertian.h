#ifndef LAMBERTIANH
#define LAMBERTIANH
#include "material.h"
#include <random>
#include <ctime>

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0, 1.0);


class lambertian : public material{
    public:
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const{
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }
    

        vec3 albedo;
};


vec3 random_in_unit_sphere(){
    vec3 p;
    do{
        p = 2.0*vec3(dis(gen),dis(gen),dis(gen))  - vec3(1,1,1);
    } while(p.squared_length() >= 1.0);
    return p;
}

#endif

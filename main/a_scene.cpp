#include <chrono> 
#include <core/image.h>
#include <core/color.h>
#include <core/random.h>
#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/textures/constant.h>
#include <rt/renderer.h>
#include <rt/integrators/recraytrace.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/casting.h>


#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/solids/quad.h>

#include <rt/solids/infiniteplane.h>
#include <rt/solids/sphere.h>
#include <rt/materials/lambertian.h>

#include <rt/materials/flatmaterial.h>
#include <rt/materials/glass.h>
#include <rt/materials/cooktor.h>
#include <rt/materials/fuzzymirror.h>
#include <rt/lights/arealight.h>
#include <rt/lights/pointlight.h>
#include <rt/coordmappers/environmental.h>
#include <rt/solids/environmental.h>

#include <rt/textures/perlin.h>

using namespace rt;
namespace{
        TexLib* gettex() {
        TexLib* texlib = new TexLib;
        ImageTexture* bumptex_mountain = new ImageTexture("models/tex_mountain_bump.png");
        ImageTexture* bumptex_stone = new ImageTexture("models/tex_stone_bump.png");
        ImageTexture* bumptex_boatinside = new ImageTexture("models/tex_boat_inside_bump.png");
        ImageTexture* bumptex_boatout = new ImageTexture("models/tex_boat_out_bump.png");
        ImageTexture* bumptex_boatup = new ImageTexture("models/tex_boat_up_bump.png");
        ImageTexture* bumptex_lotus = new ImageTexture("models/tex_lotus_bump.png");
        // Bump Textures
        // Mountains
        texlib->insert(std::pair<std::string, Texture*>("Mountain.001", bumptex_mountain));
        texlib->insert(std::pair<std::string, Texture*>("Mountain.002", bumptex_mountain));
        texlib->insert(std::pair<std::string, Texture*>("Mountain.003", bumptex_mountain));
        
        //Stones
        texlib->insert(std::pair<std::string, Texture*>("Stone.002", bumptex_stone));
            
        //No bump tex on birds => nullptr
        texlib->insert(std::pair<std::string, Texture*>("Bird.001", nullptr));
        texlib->insert(std::pair<std::string, Texture*>("Bird.002", nullptr));
            
        //Flower
        texlib->insert(std::pair<std::string, Texture*>("lotus.001", bumptex_lotus));
        texlib->insert(std::pair<std::string, Texture*>("lotus.002", bumptex_lotus));
        texlib->insert(std::pair<std::string, Texture*>("lotus.003", bumptex_lotus));
        texlib->insert(std::pair<std::string, Texture*>("lotus.004", bumptex_lotus));
        texlib->insert(std::pair<std::string, Texture*>("lotus.005", bumptex_lotus));
            
        //Different bump tex for different parts of the boat
        texlib->insert(std::pair<std::string, Texture*>("Boat.001", bumptex_boatout));
        texlib->insert(std::pair<std::string, Texture*>("Boat.002", bumptex_boatinside));
        texlib->insert(std::pair<std::string, Texture*>("Boat.003", bumptex_boatup));
        return texlib;
    }
MatLib* gettable() {
        MatLib* matlib = new MatLib;
        // Textures
        // Mountains
        ImageTexture* grasstex = new ImageTexture("models/tex_grass.png");

        // Boat
        ImageTexture* boattex_out = new ImageTexture("models/tex_boat_out.png");
        ImageTexture* boattex_in = new ImageTexture("models/tex_boat_inside.png");
        ImageTexture* boattex_up = new ImageTexture("models/tex_boat_up.png");

        // Bird
        ImageTexture* birdtex = new ImageTexture("models/tex_bird.png");


        // Stone
        ImageTexture* stonetex = new ImageTexture("models/tex_stones.png");

        // River Bed
        ImageTexture* riverbedtex = new ImageTexture("models/tex_riverbed.png");

        //flower
        ImageTexture* flowertex = new ImageTexture("models/tex_flower.png");


        ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
        ConstantTexture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));
        ConstantTexture* brasstex = new ConstantTexture(RGBColor(0.91f, 0.737f, 0.114f));

        //Bird
        matlib->insert(std::pair<std::string, Material*>("Bird.001", new LambertianMaterial(blacktex, birdtex)));
        matlib->insert(std::pair<std::string, Material*>("Bird.002", new LambertianMaterial(blacktex, birdtex)));

        //Mountain
        matlib->insert(std::pair<std::string, Material*>("Mountain.001", new LambertianMaterial(blacktex, grasstex)));
        matlib->insert(std::pair<std::string, Material*>("Mountain.002", new LambertianMaterial(blacktex, grasstex)));
        matlib->insert(std::pair<std::string, Material*>("Mountain.003", new LambertianMaterial(blacktex, grasstex)));

        // River
        Material* sphereMaterial1 = new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f);
        Material* sphereMaterial2 = new GlassMaterial(2.0f);
        matlib->insert(std::pair<std::string, Material*>("River", sphereMaterial1));

        //Boat
        matlib->insert(std::pair<std::string, Material*>("Boat.001", new LambertianMaterial(blacktex, boattex_out)));
        matlib->insert(std::pair<std::string, Material*>("Boat.002", new LambertianMaterial(blacktex, boattex_in)));
        matlib->insert(std::pair<std::string, Material*>("Boat.003", new LambertianMaterial(blacktex, boattex_up)));

        //Stone
        matlib->insert(std::pair<std::string, Material*>("Stone.002", new LambertianMaterial(blacktex, stonetex)));

        //RiverBed
        matlib->insert(std::pair<std::string, Material*>("riverbed", new LambertianMaterial(blacktex, riverbedtex)));

        //Krishna
        matlib->insert(std::pair<std::string, Material*>("Krishna", new CookTorranceMaterial(brasstex, 0.4f, 2.f, 0.3f)));

        //RiverBed
        matlib->insert(std::pair<std::string, Material*>("lotus.001", new LambertianMaterial(blacktex, flowertex)));
        matlib->insert(std::pair<std::string, Material*>("lotus.002", new LambertianMaterial(blacktex, flowertex)));
        matlib->insert(std::pair<std::string, Material*>("lotus.003", new LambertianMaterial(blacktex, flowertex)));
        matlib->insert(std::pair<std::string, Material*>("lotus.004", new LambertianMaterial(blacktex, flowertex)));
        matlib->insert(std::pair<std::string, Material*>("lotus.005", new LambertianMaterial(blacktex, flowertex)));
    
       

        //transparent screen
        //matlib->insert(std::pair<std::string, Material*>("Screen.001", new GlassMaterial(1.0f)));


        //drops on the screen
        matlib->insert(std::pair<std::string, Material*>("drop.001", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.002", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.003", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.004", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.005", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.006", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.007", new GlassMaterial(2.0f)));        
        matlib->insert(std::pair<std::string, Material*>("drop.008", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.009", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.010", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.011", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.012", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.013", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.015", new GlassMaterial(2.0f)));
        matlib->insert(std::pair<std::string, Material*>("drop.014", new GlassMaterial(2.0f)));
        
    return matlib;
}





void renderCompScene(int numSamples, const char* filename) {
    
    World world;
    BVH* scene = new BVH();

    MatLib* gettable1 = gettable();
    loadOBJ(scene, "models/", "scene_new_cp.obj", gettable1, gettex());
    
    //perlin texture for the blue sky
    PerlinTexture* skyBlueTex = new PerlinTexture(RGBColor(1.f, 1.f, 1.f), RGBColor(0.0, 0.509f, 1.f));
    skyBlueTex->addOctave(0.5f, 10.0f);
    skyBlueTex->addOctave(0.25f, 20.0f);
    skyBlueTex->addOctave(0.125f, 40.0f);
    skyBlueTex->addOctave(0.125f, 80.0f);

    FlatMaterial perlinSkyMat(skyBlueTex);
    
    EnvironmentalCoordMapper* envmapper = new EnvironmentalCoordMapper();
    scene->add(new Environmental(envmapper, &perlinSkyMat));

    // Light
    ConstantTexture* lightsrctex1 = new ConstantTexture(RGBColor::rep(1200.0f));
    
    ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    
    Material* lightsource1 = new LambertianMaterial(lightsrctex1, blacktex);
    
    Sphere* light1 = new Sphere(Point(800.0, 100.5, -602) , 20.0f, nullptr, lightsource1);
    scene->rebuildIndex();
    
    world.scene = scene;
    
    AreaLight als1(light1);
    world.light.push_back(&als1);
    
    //small point light to light the boat
    world.light.push_back(new PointLight(Point(2.5, 2.5, -5.0), RGBColor::rep(60.f)));
    
    //set the resolution of the image here
    Image img(480, 270);
    //Image img(1920, 1080);
    
    //Recursive raytracer
    RecursiveRayTracingIntegrator integrator(&world);
    
    //DOF camera
    DOFPerspectiveCamera cam(Point(5.54116, 0.438489, 6.309074), Vector(-0.737665, 0.187156, -0.669518), Vector(-0.064537, 0.996195, -0.058575), pi * 9 / 32, pi / 2, 7.2f, 0.03f);
    Renderer engine(&cam, &integrator);
    if (numSamples > 1)
        engine.setSamples(numSamples);
    engine.render(img);
    img.writePNG(filename);
}



}


void a_scene() {
    
    auto start = std::chrono::high_resolution_clock::now();
    //set the no of samples and the name of the image
    renderCompScene(1, "scene.png");
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time for building tree: " << elapsed.count() << " s\n";
}

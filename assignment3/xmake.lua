add_requires("opengl", "glu")
target("raytracer3")
    set_kind("binary")
    add_deps("common")
    add_packages("opengl", "glu")

    add_files("*.cpp", {includedirs="."})
    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0")


task("raytracer3_scenes")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake -w raytracer3")
        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_01_cube_orthographic.txt -size 200 200 -output ".. imgs .."/output3_01.tga -gui")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_02_cube_perspective.txt -size 200 200 -output ".. imgs .."/output3_02.tga -gui")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_03_bunny_mesh_200.txt -size 200 200 -output ".. imgs .."/output3_03.tga -gui")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_04_bunny_mesh_1k.txt -size 200 200 -output ".. imgs .."/output3_04.tga -gui")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_05_axes_cube.txt -size 200 200 -output ".. imgs .."/output3_05.tga -gui")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_06_crazy_transforms.txt -size 200 200 -output ".. imgs .."/output3_06.tga -gui")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_07_plane.txt -size 200 200 -output ".. imgs .."/output3_07.tga -gui -tessellation 10 5")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_08_sphere.txt -size 200 200 -output ".. imgs .."/output3_08.tga -gui -tessellation 10 5")
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_08_sphere.txt -size 200 200 -output ".. imgs .."/output3_08.tga -gui -tessellation 20 10")
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_08_sphere.txt -size 200 200 -output ".. imgs .."/output3_08.tga -gui -tessellation 10 5 -gouraud")
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_08_sphere.txt -size 200 200 -output ".. imgs .."/output3_08.tga -gui -tessellation 20 10 -gouraud")
        
        -- os.exec("xmake r raytracer3 -input ".. assets .."/scene3_09_exponent_variations.txt -size 300 300 -output ".. imgs .."/output3_09.tga -gui -tessellation 100 50 -gouraud -normals ".. imgs .."/output3_09_normal.tga")
        
        os.exec("xmake r raytracer3 -input ".. assets .."/scene3_10_exponent_variations_back.txt -size 300 300 -output ".. imgs .."/output3_10.tga -gui -tessellation 100 50 -gouraud")
        -- OPTIONAL specular fix
        
        os.exec("xmake r raytracer3 -input ".. assets .."/scene3_11_weird_lighting_diffuse.txt -size 200 200 -output ".. imgs .."/output3_11.tga -gui -tessellation 100 50 -gouraud")
        
        os.exec("xmake r raytracer3 -input ".. assets .."/scene3_12_weird_lighting_specular.txt -size 200 200 -output ".. imgs .."/output3_12.tga -gui -tessellation 100 50 -gouraud")
    end)
    set_menu {}

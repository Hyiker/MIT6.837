target("raytracer2")
    set_kind("binary")
    add_deps("common")

    add_files("*.cpp", {includedirs="."})
    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES")


task("raytracer2_scenes")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake -w raytracer2")
        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_01_diffuse.txt -size 200 200 -output ".. imgs .."/output2_01.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_02_ambient.txt -size 200 200 -output ".. imgs .."/output2_02.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_03_colored_lights.txt -size 200 200 -output ".. imgs .."/output2_03.tga -normals ".. imgs .."/normals2_03.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_04_perspective.txt -size 200 200 -output ".. imgs .."/output2_04.tga -normals ".. imgs .."/normals2_04.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_05_inside_sphere.txt -size 200 200 -output ".. imgs .."/output2_05.tga -depth 9 11 ".. imgs .."/depth2_05.tga -normals ".. imgs .."/normals2_05.tga -shade_back")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_05_inside_sphere.txt -size 200 200 -output ".. imgs .."/output2_05_no_back.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_06_plane.txt -size 200 200 -output ".. imgs .."/output2_06.tga -depth 8 20 ".. imgs .."/depth2_06.tga -normals ".. imgs .."/normals2_06.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_07_sphere_triangles.txt -size 200 200 -output ".. imgs .."/output2_07.tga -depth 9 11 ".. imgs .."/depth2_07.tga -normals ".. imgs .."/normals2_07.tga -shade_back")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_07_sphere_triangles.txt -size 200 200 -output ".. imgs .."/output2_07_no_back.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_08_cube.txt -size 200 200 -output ".. imgs .."/output2_08.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_09_bunny_200.txt -size 200 200 -output ".. imgs .."/output2_09.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_10_bunny_1k.txt -size 200 200 -output ".. imgs .."/output2_10.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_11_squashed_sphere.txt -size 200 200 -output ".. imgs .."/output2_11.tga -normals ".. imgs .."/normals2_11.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_12_rotated_sphere.txt -size 200 200 -output ".. imgs .."/output2_12.tga -normals ".. imgs .."/normals2_12.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_13_rotated_squashed_sphere.txt -size 200 200 -output ".. imgs .."/output2_13.tga -normals ".. imgs .."/normals2_13.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_14_axes_cube.txt -size 200 200 -output ".. imgs .."/output2_14.tga")
        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_15_crazy_transforms.txt -size 200 200 -output ".. imgs .."/output2_15.tga")

        os.exec("xmake r raytracer2 -input ".. assets .."/scene2_16_t_scale.txt -size 200 200 -output ".. imgs .."/output2_16.tga -depth 2 7 ".. imgs .."/depth2_16.tga")
    end)
    set_menu {}

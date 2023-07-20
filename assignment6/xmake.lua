add_requires("opengl", "glu")
target("raytracer6")
    set_kind("binary")
    add_deps("common")
    add_packages("opengl", "glu")

    add_files("*.cpp", {includedirs="."})
    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")


task("raytracer6_scene6")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake -w raytracer6")
        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_01_sphere.txt -output ".. imgs .."/output6_01a.tga -size 200 200 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_01_sphere.txt -output ".. imgs .."/output6_01b.tga -size 200 200 -grid 10 10 10 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_01_sphere.txt -output ".. imgs .."/output6_01c.tga -size 200 200 -grid 10 10 10 -visualize_grid")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_02_sphere_triangles.txt -output ".. imgs .."/output6_02a.tga -size 200 200 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_02_sphere_triangles.txt -output ".. imgs .."/output6_02b.tga -size 200 200 -grid 10 10 10 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_02_sphere_triangles.txt -output ".. imgs .."/output6_02c.tga -size 200 200 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_02_sphere_triangles.txt -output ".. imgs .."/output6_02d.tga -size 200 200 -grid 10 10 10 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_02_sphere_triangles.txt -output ".. imgs .."/output6_02e.tga -size 200 200 -grid 10 10 10 -visualize_grid")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_03_sphere_plane.txt -output ".. imgs .."/output6_03a.tga -size 200 200 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_03_sphere_plane.txt -output ".. imgs .."/output6_03b.tga -size 200 200 -grid 10 10 10 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_03_sphere_plane.txt -output ".. imgs .."/output6_03c.tga -size 200 200 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_03_sphere_plane.txt -output ".. imgs .."/output6_03d.tga -size 200 200 -grid 10 10 10 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_03_sphere_plane.txt -output ".. imgs .."/output6_03e.tga -size 200 200 -grid 10 10 10 -visualize_grid")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_04_bunny_mesh_200.txt -output ".. imgs .."/output6_04a.tga -size 200 200 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_04_bunny_mesh_200.txt -output ".. imgs .."/output6_04b.tga -size 200 200 -grid 10 10 7 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_04_bunny_mesh_200.txt -output ".. imgs .."/output6_04c.tga -size 200 200 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_04_bunny_mesh_200.txt -output ".. imgs .."/output6_04d.tga -size 200 200 -grid 10 10 7 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_04_bunny_mesh_200.txt -output ".. imgs .."/output6_04e.tga -size 200 200 -grid 10 10 7 -visualize_grid")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_05_bunny_mesh_1k.txt -output ".. imgs .."/output6_05.tga -size 200 200 -grid 15 15 12 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_06_bunny_mesh_5k.txt -output ".. imgs .."/output6_06.tga -size 200 200 -grid 20 20 15 -stats -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_07_bunny_mesh_40k.txt -output ".. imgs .."/output6_07.tga -size 200 200 -grid 40 40 33 -stats -shadows")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_08_scale_translate.txt -size 200 200 -output ".. imgs .."/output6_08a.tga")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_08_scale_translate.txt -size 200 200 -output ".. imgs .."/output6_08b.tga -grid 15 15 15")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_08_scale_translate.txt -size 200 200 -output ".. imgs .."/output6_08c.tga -grid 15 15 15 -visualize_grid")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_09_rotated_triangles.txt -size 200 200 -output ".. imgs .."/output6_09a.tga")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_09_rotated_triangles.txt -size 200 200 -output ".. imgs .."/output6_09b.tga -grid 15 15 9")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_09_rotated_triangles.txt -size 200 200 -output ".. imgs .."/output6_09c.tga -grid 15 15 9 -visualize_grid")
        -- -- Note: the grid voxelization of the green triangle uses an optional special case for transformed triangles and will look different if you have not implemented this option.
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_10_nested_transformations.txt -size 200 200 -output ".. imgs .."/output6_10a.tga")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_10_nested_transformations.txt -size 200 200 -output ".. imgs .."/output6_10b.tga -grid 30 30 30")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_10_nested_transformations.txt -size 200 200 -output ".. imgs .."/output6_10c.tga -grid 30 30 30 -visualize_grid")
        -- -- Note: the grid voxelization of the blue rhombus uses an optional special case for transformed triangles and will look different if you have not implemented this option.
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_11_mirrored_floor.txt -size 200 200 -output ".. imgs .."/output6_11a.tga -shadows -bounces 1 -weight 0.01 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_11_mirrored_floor.txt -size 200 200 -output ".. imgs .."/output6_11b.tga -shadows -bounces 1 -weight 0.01 -grid 40 10 40 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_11_mirrored_floor.txt -size 200 200 -output ".. imgs .."/output6_11c.tga -grid 40 10 40 -visualize_grid")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_12_faceted_gem.txt -size 200 200 -output ".. imgs .."/output6_12a.tga -shadows -shade_back -bounces 5 -weight 0.01 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_12_faceted_gem.txt -size 200 200 -output ".. imgs .."/output6_12b.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -stats")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_12_faceted_gem.txt -size 200 200 -output ".. imgs .."/output6_12c.tga -grid 20 20 20 -visualize_grid")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_13_checkerboard.txt -size 200 200 -output ".. imgs .."/output6_13.tga -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_14_glass_sphere.txt -size 200 200 -output ".. imgs .."/output6_14.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_15_marble_cubes.txt -size 300 300 -output ".. imgs .."/output6_15.tga")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_16_wood_cubes.txt -size 300 300 -output ".. imgs .."/output6_16.tga")
        
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_17_marble_vase.txt -size 300 300 -output ".. imgs .."/output6_17a.tga -grid 15 30 15 -bounces 1 -shadows")
        -- os.exec("xmake r raytracer6 -input ".. assets .."/scene6_17_marble_vase.txt -size 300 300 -output ".. imgs .."/output6_17b.tga -grid 15 30 15 -visualize_grid")
        -- -- UPDATE (11/2): this new image correctly handles procedural textures with a large ambient light contribution.
        
        os.exec("xmake r raytracer6 -input ".. assets .."/scene6_18_6.837_logo.txt -size 400 200 -output ".. imgs .."/output6_18a.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 80 30 3")
        os.exec("xmake r raytracer6 -input ".. assets .."/scene6_18_6.837_logo.txt -size 400 200 -output ".. imgs .."/output6_18b.tga -grid 80 30 3 -visualize_grid")
    end)
    set_menu {}

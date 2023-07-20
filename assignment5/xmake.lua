add_requires("opengl", "glu")
target("raytracer5")
    set_kind("binary")
    add_deps("common")
    add_packages("opengl", "glu")

    add_files("*.cpp", {includedirs="."})
    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")


task("raytracer5_scene5")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake -w raytracer5")
        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_01_sphere.txt -size 200 200 -output ".. imgs .."/output5_01a.tga -gui -grid 1 1 1 -visualize_grid")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_01_sphere.txt -size 200 200 -output ".. imgs .."/output5_01b.tga -gui -grid 5 5 5 -visualize_grid")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_01_sphere.txt -size 200 200 -output ".. imgs .."/output5_01c.tga -gui -grid 15 15 15 -visualize_grid")
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_02_spheres.txt -size 200 200 -output ".. imgs .."/output5_02a.tga -gui -grid 15 15 15 -visualize_grid")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_02_spheres.txt -size 200 200 -output ".. imgs .."/output5_02b.tga -gui -grid 15 15 3 -visualize_grid")
        
        -- -- Visualize Hit Cells
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_02_spheres.txt -size 200 200 -gui -grid 8 8 8 -visualize_grid")
        -- -- Visualize Entered Faces
		
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_03_offcenter_spheres.txt -size 200 200 -output ".. imgs .."/output5_03.tga -gui -grid 20 20 20 -visualize_grid")
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_04_plane_test.txt -size 200 200 -gui -tessellation 30 15 -gouraud")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_04_plane_test.txt -size 200 200 -output ".. imgs .."/output5_04.tga -gui -grid 15 15 15 -visualize_grid")
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_05_sphere_triangles.txt -size 200 200 -gui -tessellation 30 15 -gouraud")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_05_sphere_triangles.txt -size 200 200 -output ".. imgs .."/output5_05.tga -gui -grid 20 20 10 -visualize_grid")
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_06_bunny_mesh_200.txt -size 200 200 -output ".. imgs .."/output5_06.tga -gui -grid 10 10 7 -visualize_grid")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_07_bunny_mesh_1k.txt -size 200 200 -output ".. imgs .."/output5_07.tga -gui -grid 15 15 12 -visualize_grid")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_08_bunny_mesh_5k.txt -size 200 200 -output ".. imgs .."/output5_08.tga -gui -grid 20 20 15 -visualize_grid")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_09_bunny_mesh_40k.txt -size 200 200 -output ".. imgs .."/output5_09.tga -gui -grid 40 40 33 -visualize_grid")
        
        os.exec("xmake r raytracer5 -input ".. assets .."/scene5_10_scale_translate.txt -size 200 200 -gui -tessellation 30 15 -gouraud")
        os.exec("xmake r raytracer5 -input ".. assets .."/scene5_10_scale_translate.txt -size 200 200 -output ".. imgs .."/output5_10.tga -gui -grid 15 15 15 -visualize_grid")
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_11_rotated_triangles.txt -size 200 200 -gui")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_11_rotated_triangles.txt -size 200 200 -output ".. imgs .."/output5_11.tga -gui -grid 15 15 9 -visualize_grid")
        -- -- Note: the grid voxelization of the green triangle uses an optional special case for transformed triangles and will look different if you have not implemented this option.
        
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_12_nested_transformations.txt -size 200 200 -gui")
        -- os.exec("xmake r raytracer5 -input ".. assets .."/scene5_12_nested_transformations.txt -size 200 200 -output ".. imgs .."/output5_12.tga -gui -grid 30 30 30 -visualize_grid")
    end)
    set_menu {}
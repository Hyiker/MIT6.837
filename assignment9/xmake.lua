add_requires("opengl", "glu")
-- target("raytracer9")
--     set_kind("binary")
--     add_deps("common")
--     add_packages("opengl", "glu")

--     add_files("*.cpp", {includedirs="."})
--     set_policy("build.warning", true)
--     set_warnings("allextra")
--     set_languages("c17", "cxx20")
--     add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")

target("particle_system9")
    set_kind("binary")
    add_deps("common")
    add_packages("opengl", "glu")

    add_files("*.cpp", {includedirs="."})
    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")

task("scene9")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake build particle_system9")

        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")

        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_01_hose.txt -refresh 0.1 -dt 0.1")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_01_hose.txt -refresh 0.05 -dt 0.05")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_01_hose.txt -refresh 0.1 -dt 0.1 -motion_blur")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_01_hose.txt -refresh 0.05 -dt 0.05 -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_02_hose_gravity.txt -refresh 0.05 -dt 0.05 -draw_vectors 0.1")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_02_hose_gravity.txt -refresh 0.05 -dt 0.05 -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_03_hose_force.txt -refresh 0.05 -dt 0.05 -draw_vectors 0.1")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_03_hose_force.txt -refresh 0.05 -dt 0.05 -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_04_circle_euler.txt -refresh 0.1 -dt 0.1 -integrator_color -draw_vectors 0.02")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_04_circle_euler.txt -refresh 0.05 -dt 0.05 -integrator_color -motion_blur")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_04_circle_euler.txt -refresh 0.01 -dt 0.01 -integrator_color -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_05_circle_midpoint.txt -refresh 0.1 -dt 0.1 -integrator_color -draw_vectors 0.02")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_05_circle_midpoint.txt -refresh 0.05 -dt 0.05 -integrator_color -motion_blur")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_05_circle_midpoint.txt -refresh 0.01 -dt 0.01 -integrator_color -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_06_circle_rungekutta.txt -refresh 0.1 -dt 0.1 -integrator_color -draw_vectors 0.02")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_06_circle_rungekutta.txt -refresh 0.05 -dt 0.05 -integrator_color -motion_blur")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_06_circle_rungekutta.txt -refresh 0.01 -dt 0.01 -integrator_color -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_07_wave.txt -refresh 0.01 -dt 0.2 -integrator_color -motion_blur")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_07_wave.txt -refresh 0.01 -dt 0.05 -integrator_color -motion_blur")
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_07_wave.txt -refresh 0.01 -dt 0.01 -integrator_color -motion_blur")
        
        os.exec("xmake r particle_system9 -input ".. assets .."/system9_08_fire.txt -refresh 0.05 -dt 0.05 -motion_blur")
        
        -- os.exec("xmake r particle_system9 -input ".. assets .."/system9_09_wind.txt -motion_blur -dt 0.05 -refresh 0.05")
    end)
    set_menu{}

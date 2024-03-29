add_requires("opengl", "glu", "tbb")

target("raytracerx")
    set_kind("binary")
    add_deps("common")
    set_symbols("debug")
    add_packages("opengl", "glu", "tbb")

    add_includedirs("include")
    add_files("*.cpp")

    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")

task("raytracerx_scenes")
    set_category("plugin")
    on_run(function ()

        import("core.base.option")

        local flags = ""
        if option.get("gui") then
            flags = flags .. " -gui"
        end

        os.exec("xmake f -m release -c")
        os.exec("xmake build raytracerx")

        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")

        os.exec("xmake r raytracerx -input ".. assets .."/scene1.txt -size 628 628 -output ".. imgs .."/scene1.exr -shadows -jittered_samples 1024 -tent_filter 1.5 -bounces 16 -bvh" .. flags)
        -- os.exec("xmake r raytracerx -input ".. assets .."/scene2.txt -size 628 628 -output ".. imgs .."/scene2.exr -shadows -jittered_samples 1024 -tent_filter 1.0 -bounces 12 -grid 30 30 30" .. flags)
        -- os.exec("xmake r raytracerx -input ".. assets .."/scene2.txt -size 628 628 -output ".. imgs .."/scene2.exr -shadows -jittered_samples 1024 -tent_filter 1.0 -bounces 12 -bvh" .. flags)
    

        -- print("scene2 with 30x30x30 grid")
        -- os.exec("xmake r raytracerx -input ".. assets .."/scene2.txt -size 300 300 -output ".. imgs .."/scene2_grid.exr -shadows -jittered_samples 128 -tent_filter 1.0 -bounces 12 -grid 30 30 30" .. flags)
        -- print("scene2 with bvh")
        -- os.exec("xmake r raytracerx -input ".. assets .."/scene2.txt -size 300 300 -output ".. imgs .."/scene2_bvh.exr -shadows -jittered_samples 128 -tent_filter 1.0 -bounces 12 -bvh" .. flags)


    end)
    set_menu{
        options = {
            {"g", "gui", "k", nil, "enable gui"}
        }
    }

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

        -- os.exec("xmake r raytracerx -input ".. assets .."/scene7_03_marble_vase.txt -size 200 200 -output ".. imgs .."/output7_03a.tga -shadows -jittered_samples 16 -tent_filter 1.0 -bounces 1")
        -- os.exec("xmake r raytracerx -input ".. assets .."/marble_vase_pbr.txt -size 200 200 -output ".. imgs .."/marble_vase_pbr.tga -shadows -jittered_samples 128 -tent_filter 1.0 -bounces 10 -grid 30 30 30")
        os.exec("xmake r raytracerx -input ".. assets .."/scene1.txt -size 400 400 -output ".. imgs .."/scene1.tga -shadows -jittered_samples 1024 -tent_filter 1.5 -bounces 10 -grid 40 40 40" .. flags)
        -- os.exec("xmake r raytracerx -input ".. assets .."/scene2.txt -size 200 200 -output ".. imgs .."/scene2.tga -shadows -jittered_samples 128 -tent_filter 1.5 -bounces 12 -grid 30 30 30" .. flags)

    end)
    set_menu{
        options = {
            {"g", "gui", "k", nil, "enable gui"}
        }
    }

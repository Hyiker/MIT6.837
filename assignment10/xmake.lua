add_requires("opengl", "glu", "tbb")
target("raytracerx")
    set_kind("binary")
    add_deps("common")
    add_packages("opengl", "glu", "tbb")

    add_files("*.cpp", {includedirs="."})
    set_policy("build.warning", true)
    set_warnings("allextra")
    set_languages("c17", "cxx20")
    add_defines("_USE_MATH_DEFINES", "_HAS_STD_BYTE=0", "NOMINMAX")

task("raytracerx_scenes")
    set_category("plugin")
    on_run(function ()
        os.exec("xmake f -m release -c")
        os.exec("xmake build raytracerx")

        local assets = path.join(os.scriptdir(), "assets")
        local imgs = path.join(os.scriptdir(), "imgs")

    end)
    set_menu{}

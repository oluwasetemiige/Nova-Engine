add_executable(NovaEditor
    src/main.cpp
    src/EditorLayer.cpp)
target_link_libraries(NovaEditor PRIVATE Nova)
nova_set_warnings(NovaEditor)

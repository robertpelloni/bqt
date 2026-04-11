#include <iostream>
namespace ImGui {
    void NewFrame() { std::cout << "[ImGui MOCK] NewFrame" << std::endl; }
    void Render() { std::cout << "[ImGui MOCK] Render" << std::endl; }
}
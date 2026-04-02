// juce_mock.cpp
#include <iostream>

namespace juce {
    void MessageManager::getInstance() {
        std::cout << "JUCE MessageManager mock initialized" << std::endl;
    }
}

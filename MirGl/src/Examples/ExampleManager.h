#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "Examples/IExample.h"

namespace Mir {

struct ExampleManager {
    std::vector<std::unique_ptr<IExample>> examples;
    size_t currentIndex = 0;
    IExample* currentExample = nullptr;
    
    // Add/select examples
    void addExample(std::unique_ptr<IExample> example) {
        example->setup();
        examples.push_back(std::move(example));
        if (examples.size() == 1) {
            selectExample(0);
        }
    }
    
    void selectExample(size_t index) {
        if (index < examples.size()) {
            size_t previousIndex = currentIndex;
            currentIndex = index;
            currentExample = examples[index].get();
            if (examples.size() > 1 && previousIndex < examples.size()) {
                examples[previousIndex]->cleanup();
            }
            examples[currentIndex]->setup();
        }
    }
    
    // Helper methods
    size_t count() const { return examples.size(); }
    const char* getCurrentName() const { 
        return currentExample ? currentExample->getName() : "None"; 
    }
};

} // namespace Mir
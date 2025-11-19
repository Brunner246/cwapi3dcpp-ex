#include <cwapi3d/composite/Component.h>
#include <cwapi3d/composite/ComponentVisitor.h>

#include <iostream>
#include <string>
#include <cstdint>

using namespace cwapi3d::composite;

// Example 1: Element structure representation
struct ElementData {
    int elementId;
    std::string elementType;
    double length;
    
    friend std::ostream& operator<<(std::ostream& os, const ElementData& data) {
        return os << "ID:" << data.elementId << " Type:" << data.elementType 
                  << " Length:" << data.length;
    }
};

void example_element_structure() {
    std::cout << "\n=== Example 1: Element Structure Hierarchy ===\n\n";
    
    // Create a building structure using ElementData
    const auto building = std::make_unique<CompositeNode<ElementData>>("Building",
        ElementData{1000, "Building", 0.0});
    
    // Add floors
    auto floor1 = std::make_unique<CompositeNode<ElementData>>("Floor 1",
        ElementData{1100, "Floor", 0.0});
    
    // Add beams to floor 1
    auto beam1 = std::make_unique<LeafNode<ElementData>>("Beam 1", 
        ElementData{1101, "I-Beam", 5.5});
    auto beam2 = std::make_unique<LeafNode<ElementData>>("Beam 2", 
        ElementData{1102, "I-Beam", 6.0});
    
    floor1->addChild(std::move(beam1));
    floor1->addChild(std::move(beam2));
    
    // Add columns to floor 1
    auto column1 = std::make_unique<LeafNode<ElementData>>("Column 1", 
        ElementData{1103, "H-Column", 3.0});
    auto column2 = std::make_unique<LeafNode<ElementData>>("Column 2", 
        ElementData{1104, "H-Column", 3.0});
    
    floor1->addChild(std::move(column1));
    floor1->addChild(std::move(column2));
    
    building->addChild(std::move(floor1));
    
    // Add floor 2
    auto floor2 = std::make_unique<CompositeNode<ElementData>>("Floor 2",
        ElementData{1200, "Floor", 0.0});
    
    auto beam3 = std::make_unique<LeafNode<ElementData>>("Beam 3", 
        ElementData{1201, "I-Beam", 5.0});
    floor2->addChild(std::move(beam3));
    
    building->addChild(std::move(floor2));
    
    // Print the structure
    const PrintVisitorBase<ElementData> printer;
    building->accept(printer);
    
    // Traverse and count elements
    int totalElements = 0;
    building->traverse([&totalElements](const ComponentBase<ElementData>& comp) {
        if (comp.hasData()) {
            totalElements++;
        }
    });
    
    std::cout << "\nTotal elements with data: " << totalElements << "\n";
}

// Example 2: Simple element ID hierarchy
void example_element_ids() {
    std::cout << "\n=== Example 2: Element ID Hierarchy ===\n\n";
    
    // Create a hierarchy using just element IDs (uint64_t)
    const auto project = std::make_unique<CompositeNode<uint64_t>>("Project", 1000);
    
    auto zone1 = std::make_unique<CompositeNode<uint64_t>>("Zone A", 2000);
    zone1->addChild(std::make_unique<LeafNode<uint64_t>>("Element 1", 3001));
    zone1->addChild(std::make_unique<LeafNode<uint64_t>>("Element 2", 3002));
    zone1->addChild(std::make_unique<LeafNode<uint64_t>>("Element 3", 3003));
    
    auto zone2 = std::make_unique<CompositeNode<uint64_t>>("Zone B", 2001);
    zone2->addChild(std::make_unique<LeafNode<uint64_t>>("Element 4", 3004));
    zone2->addChild(std::make_unique<LeafNode<uint64_t>>("Element 5", 3005));
    
    project->addChild(std::move(zone1));
    project->addChild(std::move(zone2));
    
    // Print the structure
    const PrintVisitorBase<uint64_t> printer;
    project->accept(printer);
    
    // Collect all element IDs
    std::vector<uint64_t> allIds;
    project->traverse([&allIds](const ComponentBase<uint64_t>& comp) {
        if (comp.hasData()) {
            allIds.push_back(comp.data());
        }
    });
    
    std::cout << "\nCollected IDs: ";
    for (const auto id : allIds) {
        std::cout << id << " ";
    }
    std::cout << "\n";
}

// Example 3: String-based hierarchy (e.g., file paths or categories)
void example_string_hierarchy() {
    std::cout << "\n=== Example 3: String-based Hierarchy ===\n\n";
    
    // Create a category hierarchy using strings
    const auto root = std::make_unique<CompositeNode<std::string>>("Root", "/");
    
    auto structural = std::make_unique<CompositeNode<std::string>>("Structural", 
        "/structural");
    structural->addChild(std::make_unique<LeafNode<std::string>>("Beams", 
        "/structural/beams"));
    structural->addChild(std::make_unique<LeafNode<std::string>>("Columns", 
        "/structural/columns"));
    structural->addChild(std::make_unique<LeafNode<std::string>>("Slabs", 
        "/structural/slabs"));
    
    auto architectural = std::make_unique<CompositeNode<std::string>>("Architectural", 
        "/architectural");
    architectural->addChild(std::make_unique<LeafNode<std::string>>("Walls", 
        "/architectural/walls"));
    architectural->addChild(std::make_unique<LeafNode<std::string>>("Doors", 
        "/architectural/doors"));
    
    root->addChild(std::move(structural));
    root->addChild(std::move(architectural));
    
    // Print the structure
    const PrintVisitorBase<std::string> printer;
    root->accept(printer);
}

// Example 4: Custom visitor for specific data processing
template<typename T>
class CountingVisitor : public ComponentVisitorBase<T> {
public:
    CountingVisitor() : compositeCount(0), leafCount(0) {}
    
    void visitComposite(const CompositeNode<T>& composite) const override {
        compositeCount++;
        for (const auto& child : composite) {
            child->accept(*this);
        }
    }
    
    void visitLeaf(const LeafNode<T>& leaf) const override {
        std::cout << leaf.name() << std::endl;
        leafCount++;
    }
    
    int getCompositeCount() const { return compositeCount; }
    int getLeafCount() const { return leafCount; }
    
private:
    mutable int compositeCount;
    mutable int leafCount;
};

void example_custom_visitor() {
    std::cout << "\n=== Example 4: Custom Visitor for Statistics ===\n\n";

    const auto root = std::make_unique<CompositeNode<int>>("Root", 1);
    
    auto group1 = std::make_unique<CompositeNode<int>>("Group 1", 10);
    group1->addChild(std::make_unique<LeafNode<int>>("Item 1", 100));
    group1->addChild(std::make_unique<LeafNode<int>>("Item 2", 101));
    
    auto group2 = std::make_unique<CompositeNode<int>>("Group 2", 20);
    group2->addChild(std::make_unique<LeafNode<int>>("Item 3", 200));
    group2->addChild(std::make_unique<LeafNode<int>>("Item 4", 201));
    group2->addChild(std::make_unique<LeafNode<int>>("Item 5", 202));
    
    root->addChild(std::move(group1));
    root->addChild(std::move(group2));

    const CountingVisitor<int> counter;
    root->accept(counter);
    
    std::cout << "Statistics:\n";
    std::cout << "  Composite nodes: " << counter.getCompositeCount() << "\n";
    std::cout << "  Leaf nodes: " << counter.getLeafCount() << "\n";
    std::cout << "  Total nodes: " << (counter.getCompositeCount() + counter.getLeafCount()) << "\n";
}

// Example 5: Modifying data through traversal
void example_data_modification() {
    std::cout << "\n=== Example 5: Modifying Data Through Traversal ===\n\n";

    const auto root = std::make_unique<CompositeNode<double>>("Measurements", 0.0);
    
    root->addChild(std::make_unique<LeafNode<double>>("Value 1", 10.5));
    root->addChild(std::make_unique<LeafNode<double>>("Value 2", 20.3));
    root->addChild(std::make_unique<LeafNode<double>>("Value 3", 15.7));
    
    std::cout << "Original values:\n";
    const PrintVisitorBase<double> printer;
    root->accept(printer);
    
    // Multiply all values by 2
    root->traverse([](const ComponentBase<double>& comp) {
        if (comp.hasData() && !comp.isComposite()) {
            std::cout << "Multiplied values: " << comp.data() * 2;
        }
    });
    
    std::cout << "\nAfter multiplying by 2:\n";
    root->accept(printer);
    
    // Calculate sum
    double sum = 0.0;
    root->traverse([&sum](const ComponentBase<double>& comp) {
        if (comp.hasData() && !comp.isComposite()) {
            sum += comp.data();
        }
    });
    
    std::cout << "\nSum of all values: " << sum << "\n";
}

int main() {
    std::cout << "==============================================\n";
    std::cout << "  Generic Composite Pattern Examples\n";
    std::cout << "==============================================\n";
    
    example_element_structure();
    example_element_ids();
    example_string_hierarchy();
    example_custom_visitor();
    example_data_modification();
    
    std::cout << "\n==============================================\n";
    std::cout << "  All examples completed successfully!\n";
    std::cout << "==============================================\n";
    
    return 0;
}

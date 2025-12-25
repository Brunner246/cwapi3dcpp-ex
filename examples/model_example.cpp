/**
 * @file model_example.cpp
 * @brief Demonstrates the flexible CAD/BIM model architecture
 * 
 * This example shows:
 * - Creating domain entities (Building, Floor, Element)
 * - Dependency injection of components (Geometry, Attributes)
 * - Establishing relations without containment hierarchies
 * - Using services for mutation logic
 * - Querying the model in various ways
 * 
 * Architecture highlights:
 * - Element is JUST an identity - no geometry or attributes
 * - Geometry and attributes are injected at runtime
 * - Relations are explicit and changeable
 * - No global state - everything in ModelContext
 * - Service layer handles all business logic
 */

#include <iostream>
#include <memory>

#include "cwapi3d/model/AssignmentService.h"
#include "cwapi3d/model/BRepGeometry.h"
#include "cwapi3d/model/Building.h"
#include "cwapi3d/model/BuildingStorey.h"
#include "cwapi3d/model/Element.h"
#include "cwapi3d/model/IAttributeComponent.h"
#include "cwapi3d/model/IGeometryComponent.h"
#include "cwapi3d/model/ModelContext.h"
#include "cwapi3d/model/PropertySet.h"

using namespace CwAPI3D::Model;

void printSeparator(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n\n";
}

int main() {
    printSeparator("CAD/BIM Model Architecture Demo");

    // ========== Create Model Context ==========
    // All model state lives here - no global state or singletons
    ModelContext context;
    AssignmentService service(context);

    std::cout << "Created ModelContext (root of all model state)\n";

    // ========== Create Building Hierarchy ==========
    printSeparator("Creating Domain Entities");

    auto* building = context.createBuilding("Office Building");
    building->setAddress("123 Main Street");
    std::cout << "Created Building: " << building->name() << "\n";
    std::cout << "  Address: " << building->address().value_or("N/A") << "\n";
    std::cout << "  ID: " << building->id().toString() << "\n";

    auto* groundFloor = context.createFloor("Ground Floor", 0.0);
    auto* firstFloor = context.createFloor("First Floor", 3.5);

    std::cout << "\nCreated Floors:\n";
    std::cout << "  - " << groundFloor->name()
              << " (elevation: " << groundFloor->elevation().value_or(0.0) << "m)\n";
    std::cout << "  - " << firstFloor->name()
              << " (elevation: " << firstFloor->elevation().value_or(0.0) << "m)\n";

    // ========== Establish Relations ==========
    printSeparator("Establishing Relations (Not Containment!)");

    // Assign floors to building
    service.assignFloorToBuilding(groundFloor->id(), building->id());
    service.assignFloorToBuilding(firstFloor->id(), building->id());

    std::cout << "Assigned floors to building using relation registry\n";
    std::cout << "Note: Floors are NOT children of building - relation is separate\n";

    // ========== Create Elements ==========
    printSeparator("Creating Elements (Identity Only)");

    auto* wall1 = context.createElement("Wall-001");
    wall1->setDescription("External wall on ground floor");

    auto* wall2 = context.createElement("Wall-002");
    wall2->setDescription("Internal partition on first floor");

    auto* column1 = context.createElement("Column-001");

    std::cout << "Created elements:\n";
    std::cout << "  - " << wall1->name() << "\n";
    std::cout << "  - " << wall2->name() << "\n";
    std::cout << "  - " << column1->name() << "\n";
    std::cout << "\nImportant: Elements have NO geometry or attributes yet!\n";
    std::cout << "They are JUST identities at this point.\n";

    // ========== Inject Geometry Components ==========
    printSeparator("Dependency Injection: Geometry");

    // Create simple box geometry for wall1
    auto wall1Geom = std::make_unique<BRepGeometry>();
    wall1Geom->addVertex({{0.0, 0.0, 0.0}});
    wall1Geom->addVertex({{5.0, 0.0, 0.0}});
    wall1Geom->addVertex({{5.0, 0.3, 0.0}});
    wall1Geom->addVertex({{0.0, 0.3, 0.0}});
    wall1Geom->addVertex({{0.0, 0.0, 3.0}});
    wall1Geom->addVertex({{5.0, 0.0, 3.0}});
    wall1Geom->addVertex({{5.0, 0.3, 3.0}});
    wall1Geom->addVertex({{0.0, 0.3, 3.0}});

    service.addComponent(wall1->id(), std::move(wall1Geom));

    std::cout << "Injected BRep geometry into " << wall1->name() << "\n";

    // Verify injection
    if (auto* geom = service.getComponent<BRepGeometry>(wall1->id())) {
        auto bbox = geom->getBoundingBox();
        std::cout << "  Geometry type: " << geom->getGeometryType() << "\n";
        std::cout << "  Bounding box: ("
                  << bbox.min[0] << ", " << bbox.min[1] << ", " << bbox.min[2] << ") to ("
                  << bbox.max[0] << ", " << bbox.max[1] << ", " << bbox.max[2] << ")\n";
        std::cout << "  Vertex count: " << geom->vertices().size() << "\n";
    }

    // ========== Inject Attribute Components ==========
    printSeparator("Dependency Injection: Attributes");

    // Add material properties
    auto materialProps = std::make_unique<PropertySet>("Material Properties");
    materialProps->setAttribute("Material", std::string("Concrete"));
    materialProps->setAttribute("Density", 2400.0);  // kg/m³
    materialProps->setAttribute("ThermalConductivity", 1.4);  // W/(m·K)
    materialProps->setAttribute("FireRating", std::string("REI 90"));

    service.addComponent(wall1->id(), std::move(materialProps));

    std::cout << "Injected material properties into " << wall1->name() << "\n";

    // Verify attributes
    if (auto* props = service.getComponent<PropertySet>(wall1->id())) {
        std::cout << "  Property Set: " << props->name() << "\n";
        std::cout << "  Attributes:\n";

        if (props->hasAttribute("Material")) {
            auto material = std::any_cast<std::string>(props->getAttribute("Material"));
            std::cout << "    - Material: " << material << "\n";
        }

        if (props->hasAttribute("Density")) {
            auto density = std::any_cast<double>(props->getAttribute("Density"));
            std::cout << "    - Density: " << density << " kg/m³\n";
        }

        if (props->hasAttribute("FireRating")) {
            auto rating = std::any_cast<std::string>(props->getAttribute("FireRating"));
            std::cout << "    - Fire Rating: " << rating << "\n";
        }
    }

    // Add classification
    auto classification = std::make_unique<PropertySet>("Classification");
    classification->setAttribute("System", std::string("Uniclass"));
    classification->setAttribute("Code", std::string("Ss_25_10_20"));
    classification->setAttribute("Description", std::string("External walls"));

    service.addComponent(wall1->id(), std::move(classification));

    std::cout << "\nMultiple components per element: " << wall1->name() << " now has:\n";
    std::cout << "  - Geometry (BRepGeometry)\n";
    std::cout << "  - Material Properties (PropertySet)\n";
    std::cout << "  - Classification (PropertySet)\n";

    // ========== Assign Elements to Floors ==========
    printSeparator("Assigning Elements to Floors");

    service.assignElementToFloor(wall1->id(), groundFloor->id());
    service.assignElementToFloor(wall2->id(), firstFloor->id());
    service.assignElementToFloor(column1->id(), groundFloor->id());

    std::cout << "Assigned elements to floors using service layer\n";
    std::cout << "Note: This is a RELATION, not containment\n";
    std::cout << "Elements can be easily reassigned at runtime\n";

    // ========== Query Model ==========
    printSeparator("Querying the Model");

    // Query by floor
    std::cout << "Elements on " << groundFloor->name() << ":\n";
    auto groundElements = service.getElementsOnFloor(groundFloor->id());
    for (const auto* element : groundElements) {
        std::cout << "  - " << element->name() << "\n";
    }

    std::cout << "\nElements on " << firstFloor->name() << ":\n";
    auto firstElements = service.getElementsOnFloor(firstFloor->id());
    for (const auto* element : firstElements) {
        std::cout << "  - " << element->name() << "\n";
    }

    // Query by building
    std::cout << "\nAll elements in " << building->name() << ":\n";
    auto buildingElements = service.getElementsInBuilding(building->id());
    for (const auto* element : buildingElements) {
        std::cout << "  - " << element->name() << "\n";
    }

    // Hierarchical query
    std::cout << "\nHierarchical query: Building for " << wall1->name() << ":\n";
    if (auto* bldg = service.getBuildingForElement(wall1->id())) {
        std::cout << "  -> " << bldg->name() << "\n";
    }

    // ========== Demonstrate Runtime Flexibility ==========
    printSeparator("Runtime Flexibility: Reassign Element");

    std::cout << wall2->name() << " is currently on " << firstFloor->name() << "\n";
    std::cout << "Moving it to " << groundFloor->name() << "...\n";

    service.assignElementToFloor(wall2->id(), groundFloor->id());

    std::cout << "\nElements on " << groundFloor->name() << " after reassignment:\n";
    groundElements = service.getElementsOnFloor(groundFloor->id());
    for (const auto* element : groundElements) {
        std::cout << "  - " << element->name() << "\n";
    }

    std::cout << "\nNo object restructuring needed - just updated the relation!\n";

    // ========== Component Query ==========
    printSeparator("Component Queries");

    auto elementsWithGeometry = context.componentRegistry().getEntitiesWithComponent<BRepGeometry>();
    std::cout << "Elements with BRep geometry: " << elementsWithGeometry.size() << "\n";
    for (const auto& id : elementsWithGeometry) {
        if (auto* elem = context.getElement(id)) {
            std::cout << "  - " << elem->name() << "\n";
        }
    }

    // ========== Statistics ==========
    printSeparator("Model Statistics");

    std::cout << "Buildings: " << context.buildingCount() << "\n";
    std::cout << "Floors: " << context.floorCount() << "\n";
    std::cout << "Elements: " << context.elementCount() << "\n";
    std::cout << "Entities with components: "
              << context.componentRegistry().getEntityCount() << "\n";

    // ========== Architecture Summary ==========
    printSeparator("Architecture Summary");

    std::cout << "✓ Elements are stable identities only\n";
    std::cout << "✓ Geometry and attributes injected at runtime\n";
    std::cout << "✓ Relations are explicit, not containment\n";
    std::cout << "✓ No global state - everything in ModelContext\n";
    std::cout << "✓ Service layer handles mutation logic\n";
    std::cout << "✓ Type-safe component registry\n";
    std::cout << "✓ Suitable for Undo/Redo (command pattern ready)\n";
    std::cout << "✓ Multi-document support (multiple contexts)\n";
    std::cout << "✓ Testable, extensible, performant\n";

    std::cout << "\n========== Demo Complete ==========\n\n";

    return 0;
}

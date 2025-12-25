# CAD/BIM Model Architecture

A professional, flexible building model architecture designed for CAD/BIM development.

## 🏗️ Architecture Overview

This is a **decoupled, component-based architecture** that separates:
- **Identity** (Element, Floor, Building)
- **Data** (Geometry, Attributes)
- **Relations** (FloorAssignment, BuildingAssignment)
- **Behavior** (AssignmentService)

### Key Design Principles

1. **Element is Identity Only**
   - Elements have no geometry or attributes directly
   - Geometry and attributes are injected at runtime
   - Element is just a stable EntityId with a name

2. **Dependency Injection**
   - Components (geometry, attributes) are injected via ComponentRegistry
   - Type-safe, runtime component attachment
   - Multiple components per entity

3. **Explicit Relations, Not Containment**
   - Element → Floor is a relation, not a parent-child link
   - Floor → Building is a relation
   - Relations stored separately in registries
   - Easy runtime reassignment

4. **No Global State**
   - All model state lives in ModelContext
   - Multiple contexts = multi-document support
   - No singletons

5. **Service Layer**
   - All mutation logic in AssignmentService
   - Services operate on ModelContext
   - Clear separation of concerns

## 📦 Module Structure

```
model/
├── include/cwapi3d/model/
│   ├── EntityId.h              # Strong entity identifier
│   ├── Element.h               # Element (identity only)
│   ├── Floor.h                 # Floor/Storey
│   ├── Building.h              # Building
│   ├── IComponent.h            # Component base interface
│   ├── IGeometryComponent.h    # Geometry interface
│   ├── IAttributeComponent.h   # Attribute interface
│   ├── BRepGeometry.h          # Example BRep geometry
│   ├── PropertySet.h           # Example property set
│   ├── ComponentRegistry.h     # Component storage & injection
│   ├── FloorAssignment.h       # Element-Floor relations
│   ├── BuildingAssignment.h    # Floor-Building relations
│   ├── ModelContext.h          # Root of all model state
│   └── AssignmentService.h     # Service layer
└── src/
    └── [implementations]
```

## 🎯 Core Concepts

### EntityId

Strong type for entity identifiers. Thread-safe generation via atomic counter.

```cpp
auto id = EntityId::create();
if (id.isValid()) {
    std::cout << id.toString();
}
```

### Domain Entities

```cpp
// Building - top-level organization
Building building("Office Building");
building.setAddress("123 Main St");

// Floor - horizontal level
Floor floor("Ground Floor", 0.0);  // elevation = 0.0m

// Element - stable identity
Element element("Wall-001");
element.setDescription("External wall");
```

### Component Injection

```cpp
ModelContext context;
AssignmentService service(context);

auto* element = context.createElement("Wall");

// Inject geometry
auto geom = std::make_unique<BRepGeometry>();
geom->addVertex({{0, 0, 0}});
service.addComponent(element->id(), std::move(geom));

// Inject attributes
auto props = std::make_unique<PropertySet>("Material");
props->setAttribute("Material", std::string("Concrete"));
props->setAttribute("Density", 2400.0);
service.addComponent(element->id(), std::move(props));

// Retrieve
auto* geometry = service.getComponent<BRepGeometry>(element->id());
auto* properties = service.getComponent<PropertySet>(element->id());
```

### Relations

```cpp
// Assign element to floor (NOT containment!)
service.assignElementToFloor(element->id(), floor->id());

// Assign floor to building
service.assignFloorToBuilding(floor->id(), building->id());

// Query
auto elements = service.getElementsOnFloor(floor->id());
auto floors = service.getFloorsInBuilding(building->id());

// Hierarchical query
auto* building = service.getBuildingForElement(element->id());

// Runtime reassignment (Undo/Redo friendly)
service.assignElementToFloor(element->id(), anotherFloor->id());
```

## 🔧 Example Usage

See [examples/model_example.cpp](../examples/model_example.cpp) for a complete demonstration.

```cpp
// Create context (no global state)
ModelContext context;
AssignmentService service(context);

// Create hierarchy
auto* building = context.createBuilding("Office");
auto* floor = context.createFloor("Ground Floor", 0.0);
auto* element = context.createElement("Wall-001");

// Establish relations
service.assignFloorToBuilding(floor->id(), building->id());
service.assignElementToFloor(element->id(), floor->id());

// Inject components
auto geom = std::make_unique<BRepGeometry>();
service.addComponent(element->id(), std::move(geom));

// Query
auto elementsOnFloor = service.getElementsOnFloor(floor->id());
auto elementsInBuilding = service.getElementsInBuilding(building->id());
```

## ✅ Benefits

### Flexibility
- Runtime component injection
- Easy reassignment of relations
- Multiple component types per entity

### Testability
- No global state
- Mockable services
- Clear interfaces

### Extensibility
- Add new component types without modifying entities
- Add new relation types independently
- Service layer can evolve separately

### Performance
- Type-safe component lookup
- Efficient relation queries
- Lazy component loading possible

### Undo/Redo Ready
- All mutations through service layer
- Relations are explicit and reversible
- Command pattern can wrap service calls

### Multi-Document
- Multiple ModelContext instances
- No shared state between contexts

## 🧪 Testing

Comprehensive unit tests cover:
- EntityId generation and uniqueness
- Domain entity lifecycle
- Component registry operations
- Geometry and attribute components
- Floor and building assignments
- ModelContext entity management
- AssignmentService validation and queries

Run tests:
```bash
cd build
ctest -R test_model
```

## 🏛️ Architecture Patterns

### Domain-Driven Design (DDD)
- Entities: Building, Floor, Element
- Value Objects: EntityId
- Repositories: ComponentRegistry, FloorAssignment
- Services: AssignmentService

### Dependency Injection
- Components injected at runtime
- No compile-time coupling
- Interface-based design

### Entity-Component System (ECS)
- Entities are just IDs
- Components are data
- Systems operate on entities with specific components

### Repository Pattern
- ComponentRegistry stores and retrieves components
- FloorAssignment manages relations
- ModelContext owns all entities

## 🔮 Future Extensions

### Command Pattern for Undo/Redo
```cpp
class AssignElementCommand : public ICommand {
    EntityId elementId;
    EntityId floorId;
    // execute(), undo()
};
```

### Events/Observers
```cpp
class ModelObserver {
    virtual void onElementAssigned(EntityId, EntityId) = 0;
};
```

### Spatial Indexing
```cpp
class SpatialIndex {
    std::vector<Element*> queryByBounds(BoundingBox);
};
```

### Lazy Loading
```cpp
class LazyGeometry : public IGeometryComponent {
    // Load geometry on demand
};
```

### IFC Export/Import
```cpp
class IFCExporter {
    void exportBuilding(const Building&, const ModelContext&);
};
```

## 📚 References

- **IFC (Industry Foundation Classes)**: Inspired the property set pattern
- **Entity-Component-System**: Component injection pattern
- **Domain-Driven Design**: Entity/service separation
- **Clean Architecture**: Dependency rule, interface segregation

## 🎓 Why This Architecture?

### Problem: Traditional Hierarchies

```cpp
// ❌ Traditional approach (problems)
class Element {
    Geometry* geometry;  // Hard dependency
    std::vector<Attribute*> attributes;  // Tight coupling
    Floor* parent;  // Bidirectional link (hard to change)
};
```

**Problems:**
- Hard to change geometry representation
- Circular dependencies
- Difficult to test
- Hard to implement Undo/Redo
- Tight coupling

### Solution: Decoupled Architecture

```cpp
// ✓ Our approach (benefits)
class Element {
    EntityId id;
    std::string name;
    // That's it!
};

// Geometry injected separately
registry.addComponent(element.id(), geometry);

// Relations stored separately
assignments.assignElementToFloor(element.id(), floor.id());
```

**Benefits:**
- Loose coupling
- Easy to test
- Easy to extend
- Undo/Redo friendly
- Clear separation of concerns

## 🚀 Getting Started

1. **Create a model context:**
   ```cpp
   ModelContext context;
   AssignmentService service(context);
   ```

2. **Create entities:**
   ```cpp
   auto* building = context.createBuilding("My Building");
   auto* floor = context.createFloor("Level 1", 0.0);
   auto* element = context.createElement("Wall-1");
   ```

3. **Inject components:**
   ```cpp
   service.addComponent(element->id(), std::make_unique<BRepGeometry>());
   ```

4. **Establish relations:**
   ```cpp
   service.assignFloorToBuilding(floor->id(), building->id());
   service.assignElementToFloor(element->id(), floor->id());
   ```

5. **Query:**
   ```cpp
   auto elements = service.getElementsInBuilding(building->id());
   ```

---

**Author:** Senior C++ Architect specializing in CAD/BIM kernel development
**Date:** December 2025
**Version:** 1.0

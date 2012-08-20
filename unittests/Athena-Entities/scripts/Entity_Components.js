scene = new Athena.Entities.Scene('test');
entity = scene.create("Entity");

CHECK_EQUAL(1, entity.nbComponents);

component1 = new Athena.Entities.Component("Component1", entity.components);

CHECK_EQUAL(2, entity.nbComponents);

component2 = new Athena.Entities.Component("Component2", entity.components);

CHECK_EQUAL(3, entity.nbComponents);


CHECK(entity.getComponent(1) !== undefined);
CHECK_EQUAL('Component1', entity.getComponent(1).name);

CHECK(entity.getComponent(component1.id) !== undefined);
CHECK_EQUAL('Component1', entity.getComponent(component1.id).name);

CHECK(entity.getComponent(2) !== undefined);
CHECK_EQUAL('Component2', entity.getComponent(2).name);

CHECK(entity.getComponent(component2.id) !== undefined);
CHECK_EQUAL('Component2', entity.getComponent(component2.id).name);


CHECK(entity.getComponent(0) !== undefined);
CHECK(entity.getComponent(0).position !== undefined);

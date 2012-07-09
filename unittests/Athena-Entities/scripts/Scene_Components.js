scene = new Athena.Entities.Scene('test');

CHECK_EQUAL(0, scene.nbComponents);

component1 = new Athena.Entities.Component("Component1", scene.components);

CHECK_EQUAL(1, scene.nbComponents);

component2 = new Athena.Entities.Component("Component2", scene.components);

CHECK_EQUAL(2, scene.nbComponents);


CHECK(scene.getComponent(0) !== undefined);
CHECK_EQUAL('Component1', scene.getComponent(0).name);

CHECK(scene.getComponent(component1.id) !== undefined);
CHECK_EQUAL('Component1', scene.getComponent(component1.id).name);

CHECK(scene.getComponent(1) !== undefined);
CHECK_EQUAL('Component2', scene.getComponent(1).name);

CHECK(scene.getComponent(component2.id) !== undefined);
CHECK_EQUAL('Component2', scene.getComponent(component2.id).name);

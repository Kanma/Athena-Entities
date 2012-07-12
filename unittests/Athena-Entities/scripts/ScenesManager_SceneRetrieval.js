manager = new Athena.Entities.ScenesManager();

// The test environment already created a scene
CHECK_EQUAL(1, manager.nbScenes);

scene1 = manager.create('Scene1')
scene2 = manager.create('Scene2')

CHECK_EQUAL(3, manager.nbScenes);


CHECK(manager.getScene(1) !== undefined);
CHECK_EQUAL('Scene1', manager.getScene(1).name);

CHECK(manager.getScene(scene1.name) !== undefined);
CHECK_EQUAL(scene1.name, manager.getScene(scene1.name).name);

CHECK(manager.getScene(2) !== undefined);
CHECK_EQUAL('Scene2', manager.getScene(2).name);

CHECK(manager.getScene(scene2.name) !== undefined);
CHECK_EQUAL(scene2.name, manager.getScene(scene2.name).name);

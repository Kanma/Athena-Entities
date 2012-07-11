manager = new Athena.Entities.ScenesManager();

scene1 = manager.create('Scene1');
scene2 = manager.create('Scene2');

// The test environment already created a scene
CHECK_EQUAL(3, manager.nbScenes);

manager.destroyAll();

CHECK_EQUAL(0, manager.nbScenes);

manager = new Athena.Entities.ScenesManager();

// The test environment already created a scene
CHECK_EQUAL(1, manager.nbScenes);

scene = manager.create('Scene');

CHECK_EQUAL(2, manager.nbScenes);

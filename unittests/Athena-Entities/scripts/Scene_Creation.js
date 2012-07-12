scene = new Athena.Entities.Scene('test');

CHECK_EQUAL('test', scene.name);
CHECK_EQUAL(true, scene.enabled);
CHECK_EQUAL(false, scene.shown);

CHECK_EQUAL(0, scene.nbEntities);

CHECK(scene.components !== undefined);
CHECK_EQUAL(scene.name, scene.components.scene.name);
CHECK_EQUAL(0, scene.nbComponents);

CHECK(scene.signals !== undefined);

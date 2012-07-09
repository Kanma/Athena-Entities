scene = new Athena.Entities.Scene('test');

CHECK_EQUAL(0, scene.nbEntities);

scene.create("Entity1");
scene.create("Entity2");

CHECK_EQUAL(2, scene.nbEntities);

scene.destroyAll();

CHECK_EQUAL(0, scene.nbEntities);

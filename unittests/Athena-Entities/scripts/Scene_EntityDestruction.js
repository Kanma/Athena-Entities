scene = new Athena.Entities.Scene('test');

CHECK_EQUAL(0, scene.nbEntities);

scene.create("Entity1");
scene.create("Entity2");

CHECK_EQUAL(2, scene.nbEntities);

scene.destroy(scene.getEntity('Entity1'));

CHECK_EQUAL(1, scene.nbEntities);

scene.destroy('Entity2');

CHECK_EQUAL(0, scene.nbEntities);

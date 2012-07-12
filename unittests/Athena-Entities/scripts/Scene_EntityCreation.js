scene = new Athena.Entities.Scene('test');

CHECK_EQUAL(0, scene.nbEntities);

entity1 = scene.create("Entity1");

CHECK_EQUAL(1, scene.nbEntities);

entity2 = scene.create("Entity2");

CHECK_EQUAL(2, scene.nbEntities);


CHECK(scene.getEntity(0) !== undefined);
CHECK_EQUAL('Entity1', scene.getEntity(0).name);

CHECK(scene.getEntity('Entity1') !== undefined);
CHECK_EQUAL('Entity1', scene.getEntity('Entity1').name);

CHECK(scene.getEntity(1) !== undefined);
CHECK_EQUAL('Entity2', scene.getEntity(1).name);

CHECK(scene.getEntity('Entity2') !== undefined);
CHECK_EQUAL('Entity2', scene.getEntity('Entity2').name);

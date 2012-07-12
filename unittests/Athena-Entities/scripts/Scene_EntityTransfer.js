scene1 = new Athena.Entities.Scene('scene1');
scene2 = new Athena.Entities.Scene('scene2');

scene1.create("Entity1");
scene1.create("Entity2");

CHECK_EQUAL(2, scene1.nbEntities);
CHECK_EQUAL(0, scene2.nbEntities);

scene2.transfer(scene1.getEntity('Entity1'));

CHECK_EQUAL(1, scene1.nbEntities);
CHECK_EQUAL(1, scene2.nbEntities);
CHECK(scene2.getEntity('Entity1') !== undefined);
CHECK_EQUAL('scene2', scene2.getEntity('Entity1').scene.name);

scene2.transfer('Entity2', scene1);

CHECK_EQUAL(0, scene1.nbEntities);
CHECK_EQUAL(2, scene2.nbEntities);
CHECK(scene2.getEntity('Entity2') !== undefined);
CHECK_EQUAL('scene2', scene2.getEntity('Entity2').scene.name);

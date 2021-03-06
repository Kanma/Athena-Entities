scene = new Athena.Entities.Scene('test');

entity1 = scene.create("Entity1");

CHECK_EQUAL(0, entity1.nbChildren);

entity2 = scene.create("Entity2");

CHECK(entity2.parent === undefined);


entity1.addChild(entity2);

CHECK_EQUAL(1, entity1.nbChildren);
CHECK(entity2.parent !== undefined);
CHECK_EQUAL('Entity1', entity2.parent.name);

child = entity1.getChild(0)
CHECK(child !== undefined);
CHECK_EQUAL('Entity2', child.name);

child = entity1.getChild('Entity2')
CHECK(child !== undefined);

entity1.removeChild(entity2);

CHECK_EQUAL(0, entity1.nbChildren);
CHECK(entity2.parent === undefined);


entity1.addChild(entity2);

entity1.destroyAllChildren();

CHECK_EQUAL(0, entity1.nbChildren);

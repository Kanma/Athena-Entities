scene = new Athena.Entities.Scene('test');

entity = scene.create("Entity");

CHECK_EQUAL(true, entity.enabled);
CHECK_EQUAL('Entity', entity.name);
CHECK_EQUAL('test', entity.scene.name);
CHECK(entity.parent === undefined);
CHECK_EQUAL(0, entity.nbChildren);
CHECK_EQUAL(1, entity.nbComponents);
CHECK(entity.components !== undefined);
CHECK_EQUAL(entity.name, entity.components.entity.name);
CHECK(entity.signals !== undefined);
CHECK(entity.transforms !== undefined);
CHECK(entity.animationsMixer === undefined);

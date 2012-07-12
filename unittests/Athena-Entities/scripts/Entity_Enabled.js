scene = new Athena.Entities.Scene('test');
entity = scene.create("Entity");

CHECK_EQUAL(true, entity.enabled);

entity.enabled = false;

CHECK_EQUAL(false, entity.enabled);

entity.enabled = true;

CHECK_EQUAL(true, entity.enabled);

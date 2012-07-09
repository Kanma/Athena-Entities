scene = new Athena.Entities.Scene('test');

CHECK_EQUAL(true, scene.enabled);

scene.enabled = false;

CHECK_EQUAL(false, scene.enabled);

scene.enabled = true;

CHECK_EQUAL(true, scene.enabled);

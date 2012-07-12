scene = new Athena.Entities.Scene('test');

CHECK_EQUAL(false, scene.shown);

scene.shown = true;

CHECK_EQUAL(true, scene.shown);

scene.shown = false;

CHECK_EQUAL(false, scene.shown);

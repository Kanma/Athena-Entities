scene = new Athena.Entities.Scene('test');

entity = scene.create("Entity");

CHECK(entity.animationsMixer === undefined);

mixer = entity.createAnimationsMixer();

CHECK(entity.animationsMixer !== undefined);
CHECK_EQUAL(0, mixer.nbAnimations);

list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);

scale = transforms.scale;

CHECK_CLOSE(1.0, scale.x);
CHECK_CLOSE(1.0, scale.y);
CHECK_CLOSE(1.0, scale.z);

scale.x = 1.0;
scale.y = 2.0;
scale.z = 3.0;

transforms.scale = scale;

scale2 = transforms.scale;

CHECK_CLOSE(1.0, scale2.x);
CHECK_CLOSE(2.0, scale2.y);
CHECK_CLOSE(3.0, scale2.z);

scale3 = transforms.worldScale;

CHECK_CLOSE(1.0, scale3.x);
CHECK_CLOSE(2.0, scale3.y);
CHECK_CLOSE(3.0, scale3.z);

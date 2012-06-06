list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);

transforms.translate(1.0, 2.0, 3.0);

position = transforms.position

CHECK_CLOSE(1.0, position.x);
CHECK_CLOSE(2.0, position.y);
CHECK_CLOSE(3.0, position.z);

transforms.translate(new Athena.Math.Vector3(1.0, 2.0, 3.0), Athena.Entities.TS_WORLD);

position = transforms.position

CHECK_CLOSE(2.0, position.x);
CHECK_CLOSE(4.0, position.y);
CHECK_CLOSE(6.0, position.z);

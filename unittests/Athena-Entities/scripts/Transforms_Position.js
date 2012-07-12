list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);

position = transforms.position;

CHECK_CLOSE(0.0, position.x);
CHECK_CLOSE(0.0, position.y);
CHECK_CLOSE(0.0, position.z);

position.x = 1.0;
position.y = 2.0;
position.z = 3.0;

transforms.position = position;

position2 = transforms.position;

CHECK_CLOSE(1.0, position2.x);
CHECK_CLOSE(2.0, position2.y);
CHECK_CLOSE(3.0, position2.z);

position3 = transforms.worldPosition;

CHECK_CLOSE(1.0, position3.x);
CHECK_CLOSE(2.0, position3.y);
CHECK_CLOSE(3.0, position3.z);

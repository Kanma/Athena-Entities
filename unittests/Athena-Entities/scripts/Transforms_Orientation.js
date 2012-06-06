list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);

orientation = transforms.orientation

CHECK_CLOSE(0.0, orientation.x);
CHECK_CLOSE(0.0, orientation.y);
CHECK_CLOSE(0.0, orientation.z);
CHECK_CLOSE(1.0, orientation.w);


orientation.x = 1.0
orientation.y = 2.0
orientation.z = 3.0
orientation.w = 4.0

transforms.orientation = orientation

orientation2 = transforms.orientation

CHECK_CLOSE(1.0, orientation2.x);
CHECK_CLOSE(2.0, orientation2.y);
CHECK_CLOSE(3.0, orientation2.z);
CHECK_CLOSE(4.0, orientation2.w);

orientation3 = transforms.worldOrientation

CHECK_CLOSE(1.0, orientation3.x);
CHECK_CLOSE(2.0, orientation3.y);
CHECK_CLOSE(3.0, orientation3.z);
CHECK_CLOSE(4.0, orientation3.w);


transforms.resetOrientation();

orientation = transforms.orientation

CHECK_CLOSE(0.0, orientation.x);
CHECK_CLOSE(0.0, orientation.y);
CHECK_CLOSE(0.0, orientation.z);
CHECK_CLOSE(1.0, orientation.w);

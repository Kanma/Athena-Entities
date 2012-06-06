list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);

orientation = transforms.orientation

CHECK_CLOSE(0.0, orientation.x);
CHECK_CLOSE(0.0, orientation.y);
CHECK_CLOSE(0.0, orientation.z);
CHECK_CLOSE(1.0, orientation.w);


// (local) -Z pointing at (world) X
transforms.setDirection(new Athena.Math.Vector3(1.0, 0.0, 0.0), Athena.Entities.TS_WORLD);

ref = new Athena.Math.Quaternion(-Math.PI / 2, Athena.Math.Vector3_UNIT_Y);

orientation = transforms.orientation

CHECK_CLOSE(ref.x, orientation.x);
CHECK_CLOSE(ref.y, orientation.y);
CHECK_CLOSE(ref.z, orientation.z);
CHECK_CLOSE(ref.w, orientation.w);


// (local) -Z pointing at (world) -X
transforms.setDirection(new Athena.Math.Vector3(-1.0, 0.0, 0.0), Athena.Entities.TS_WORLD);

ref = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);

orientation = transforms.orientation

CHECK_CLOSE(ref.x, orientation.x);
CHECK_CLOSE(ref.y, orientation.y);
CHECK_CLOSE(ref.z, orientation.z);
CHECK_CLOSE(ref.w, orientation.w);


// (local) -Z pointing at (world) -Z
transforms.setDirection(new Athena.Math.Vector3(0.0, 0.0, -1.0), Athena.Entities.TS_WORLD);

ref = new Athena.Math.Quaternion(0, Athena.Math.Vector3_UNIT_Y);

orientation = transforms.orientation

CHECK_CLOSE(ref.x, orientation.x);
CHECK_CLOSE(ref.y, orientation.y);
CHECK_CLOSE(ref.z, orientation.z);
CHECK_CLOSE(ref.w, orientation.w);


// (local) -Z pointing at (world) Z
transforms.setDirection(new Athena.Math.Vector3(0.0, 0.0, 1.0), Athena.Entities.TS_WORLD);

ref = new Athena.Math.Quaternion(Math.PI, Athena.Math.Vector3_UNIT_Y);

orientation = transforms.orientation

CHECK_CLOSE(ref.x, orientation.x);
CHECK_CLOSE(ref.y, orientation.y);
CHECK_CLOSE(ref.z, orientation.z);
CHECK_CLOSE(ref.w, orientation.w);


// (local) -Z pointing at (world) (1.0, 0.0, 1.0)
transforms.setDirection(new Athena.Math.Vector3(1.0, 0.0, 1.0), Athena.Entities.TS_WORLD);

qref = new Athena.Math.Quaternion(Math.PI / 4, Athena.Math.Vector3_UNIT_Y);
ref = qref * Athena.Math.Vector3_UNIT_X;

orientation = transforms.orientation
test = orientation * Athena.Math.Vector3_UNIT_X;

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) -Z pointing at (world) (1.0, 0.0, -1.0)
transforms.setDirection(new Athena.Math.Vector3(1.0, 0.0, 1.0), Athena.Entities.TS_WORLD);

qref = new Athena.Math.Quaternion(-Math.PI / 4, Athena.Math.Vector3_UNIT_Y);
ref = qref * Athena.Math.Vector3_UNIT_X;

orientation = transforms.orientation
test = orientation * Athena.Math.Vector3_UNIT_X;

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) -Z pointing at (world) (-1.0, 0.0, 1.0)
transforms.setDirection(new Athena.Math.Vector3(-1.0, 0.0, 1.0), Athena.Entities.TS_WORLD);

qref = new Athena.Math.Quaternion(3.0 * Math.PI / 4, Athena.Math.Vector3_UNIT_Y);
ref = qref * Athena.Math.Vector3_UNIT_X;

orientation = transforms.orientation
test = orientation * Athena.Math.Vector3_UNIT_X;

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) -Z pointing at (world) (-1.0, 0.0, -1.0)
transforms.setDirection(new Athena.Math.Vector3(-1.0, 0.0, -1.0), Athena.Entities.TS_WORLD);

qref = new Athena.Math.Quaternion(5.0 * Math.PI / 4, Athena.Math.Vector3_UNIT_Y);
ref = qref * Athena.Math.Vector3_UNIT_X;

orientation = transforms.orientation
test = orientation * Athena.Math.Vector3_UNIT_X;

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) X pointing at (world) -Z
transforms.setDirection(new Athena.Math.Vector3(0.0, 0.0, -1.0), Athena.Entities.TS_WORLD, Athena.Math.Vector3_UNIT_X);

ref = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);

orientation = transforms.orientation

CHECK_CLOSE(ref.x, orientation.x);
CHECK_CLOSE(ref.y, orientation.y);
CHECK_CLOSE(ref.z, orientation.z);
CHECK_CLOSE(ref.w, orientation.w);

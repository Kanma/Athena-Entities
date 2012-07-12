list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);

orientation = transforms.orientation

CHECK_CLOSE(0.0, orientation.x);
CHECK_CLOSE(0.0, orientation.y);
CHECK_CLOSE(0.0, orientation.z);
CHECK_CLOSE(1.0, orientation.w);

transforms.position = new Athena.Math.Vector3(10.0, 0.0, 10.0);


// (local) -Z pointing at (world) (0.0, 0.0, 0.0)
transforms.lookAt(new Athena.Math.Vector3(0.0, 0.0, 0.0), Athena.Entities.TS_WORLD);

qref = new Athena.Math.Quaternion(5.0 * Math.PI / 4, Athena.Math.Vector3_UNIT_Y);
ref = qref.mul(Athena.Math.Vector3_UNIT_X);

orientation = transforms.orientation
test = orientation.mul(Athena.Math.Vector3_UNIT_X);

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) -Z pointing at (world) (100.0, 0.0, 10.0)
transforms.lookAt(new Athena.Math.Vector3(100.0, 0.0, 10.0), Athena.Entities.TS_WORLD);

qref = new Athena.Math.Quaternion(0, Athena.Math.Vector3_UNIT_Y);
ref = qref.mul(Athena.Math.Vector3_UNIT_X);

orientation = transforms.orientation
test = orientation.mul(Athena.Math.Vector3_UNIT_X);

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) -Z pointing at (local) (-100.0, 0.0, 0.0)
transforms.lookAt(new Athena.Math.Vector3(-100.0, 0.0, 0.0), Athena.Entities.TS_LOCAL);

qref = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);
ref = qref.mul(Athena.Math.Vector3_UNIT_X);

orientation = transforms.orientation
test = orientation.mul(Athena.Math.Vector3_UNIT_X);

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);


// (local) X pointing at (world) (0.0, 0.0, 0.0)
transforms.lookAt(new Athena.Math.Vector3(0.0, 0.0, 0.0), Athena.Entities.TS_WORLD, Athena.Math.Vector3_UNIT_X);

qref = new Athena.Math.Quaternion(5.0 * Math.PI / 4, Athena.Math.Vector3_UNIT_Y);
ref = qref.mul(Athena.Math.Vector3_UNIT_X);

orientation = transforms.orientation
test = orientation.mul(Athena.Math.Vector3_UNIT_X);

CHECK_CLOSE(ref.x, test.x);
CHECK_CLOSE(ref.y, test.y);
CHECK_CLOSE(ref.z, test.z);

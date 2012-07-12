function toVector(orientation)
{
    return orientation.mul(Athena.Math.Vector3_UNIT_Z);
}


list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);


// Vector + angle - Local
transforms.orientation = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);
transforms.rotate(Athena.Math.Vector3_UNIT_X, Math.PI / 2);

to_test = toVector(transforms.orientation);

CHECK_CLOSE(0.0, to_test.x);
CHECK_CLOSE(-1.0, to_test.y);
CHECK_CLOSE(0.0, to_test.z);


// Vector + angle - World
transforms.orientation = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);
transforms.rotate(Athena.Math.Vector3_UNIT_X, Math.PI / 2, Athena.Entities.TS_WORLD);

to_test = toVector(transforms.orientation);

CHECK_CLOSE(1.0, to_test.x);
CHECK_CLOSE(0.0, to_test.y);
CHECK_CLOSE(0.0, to_test.z);


// Quaternion - Local
transforms.orientation = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);
transforms.rotate(new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_X));

to_test = toVector(transforms.orientation);

CHECK_CLOSE(0.0, to_test.x);
CHECK_CLOSE(-1.0, to_test.y);
CHECK_CLOSE(0.0, to_test.z);


// Quaternion - World
transforms.orientation = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_Y);
transforms.rotate(new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_X), Athena.Entities.TS_WORLD);

to_test = toVector(transforms.orientation);

CHECK_CLOSE(1.0, to_test.x);
CHECK_CLOSE(0.0, to_test.y);
CHECK_CLOSE(0.0, to_test.z);

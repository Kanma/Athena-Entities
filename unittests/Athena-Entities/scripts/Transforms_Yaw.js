function toVector(orientation)
{
    return orientation.mul(Athena.Math.Vector3_UNIT_X);
}


list = new Athena.Entities.ComponentsList();

transforms = new Athena.Entities.Transforms("Transforms", list);


// Local
transforms.orientation = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_X);
transforms.yaw(Math.PI / 2);

to_test = toVector(transforms.orientation);

CHECK_CLOSE(0.0, to_test.x);
CHECK_CLOSE(1.0, to_test.y);
CHECK_CLOSE(0.0, to_test.z);


// World
transforms.orientation = new Athena.Math.Quaternion(Math.PI / 2, Athena.Math.Vector3_UNIT_X);
transforms.yaw(Math.PI / 2, Athena.Entities.TS_WORLD);

to_test = toVector(transforms.orientation);

CHECK_CLOSE(0.0, to_test.x);
CHECK_CLOSE(0.0, to_test.y);
CHECK_CLOSE(-1.0, to_test.z);

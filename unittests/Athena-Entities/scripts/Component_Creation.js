list = new Athena.Entities.ComponentsList();

component = new Athena.Entities.Component("Component", list);

CHECK_EQUAL(Athena.Entities.Component_TYPE, component.type);
CHECK_EQUAL("Other://Component", component.id.toString());
CHECK_EQUAL("Component", component.name);

CHECK(component.list !== undefined);

CHECK_EQUAL("Component", component.list.getComponent(0).name);

CHECK(component.transforms === undefined);
CHECK(component.signals !== undefined);

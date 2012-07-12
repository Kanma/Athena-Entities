list = new Athena.Entities.ComponentsList();

CHECK_EQUAL(0, list.nbComponents);

component = new Athena.Entities.Component("Component", list);

CHECK_EQUAL(1, list.nbComponents);

CHECK(list.getComponent(0) !== undefined);
CHECK_EQUAL('Component', list.getComponent(0).name);

CHECK(list.getComponent(component.id) !== undefined);
CHECK_EQUAL('Component', list.getComponent(component.id).name);

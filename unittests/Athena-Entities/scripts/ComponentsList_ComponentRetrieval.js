list = new Athena.Entities.ComponentsList();

CHECK_EQUAL(0, list.nbComponents);

component = new Athena.Entities.Transforms("Component", list);

CHECK_EQUAL(1, list.nbComponents);

CHECK(list.getComponent(0) !== undefined);

component2 = list.getComponent(0);

CHECK_EQUAL('Component', component2.name);
CHECK(component2.position !== undefined);

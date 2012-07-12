manager = new Athena.Entities.ComponentsManager();

list = new Athena.Entities.ComponentsList();
component = new Athena.Entities.Component("Component", list);

CHECK_EQUAL(1, list.nbComponents);

manager.destroy(component);

CHECK_EQUAL(0, list.nbComponents);

manager = new Athena.Entities.ComponentsManager();

list = new Athena.Entities.ComponentsList();

CHECK_EQUAL(0, list.nbComponents);

component = manager.create(Athena.Entities.Transforms_TYPE, 'component', list);

CHECK_EQUAL(1, list.nbComponents);

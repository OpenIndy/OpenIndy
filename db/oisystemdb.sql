-- alle Plugins die es fuer eine OpenIndy Installation gibt
CREATE TABLE plugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	name text,
	description text,
	version text,
	author text,
	compiler text,
	operating_sys text,
	has_dependencies boolean,
	file_path text,
	is_active boolean
);

-- alle Functions eines Plugins
CREATE TABLE functionPlugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	plugin_id integer,
	name text,
	description text,
	FOREIGN KEY (plugin_id) REFERENCES plugin(id)
);

-- alle sensor configs die im config Ordner gespeichert sind
CREATE TABLE sensorConfig (
	id integer PRIMARY KEY autoincrement,
	name text,
	use_as_default boolean
);

-- alle simulation plugins
CREATE TABLE simulationPlugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	plugin_id integer,
	name text,
	description text,
	FOREIGN KEY (plugin_id) REFERENCES plugin(id)
);

-- alle Sensoren eines Plugins
CREATE TABLE sensorPlugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	plugin_id integer,
	name text,
	description text,
	FOREIGN KEY (plugin_id) REFERENCES plugin(id)
);

-- alle Netzausgleichungen eines Plugins
CREATE TABLE networkAdjustmentPlugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	plugin_id integer,
	name text,
	description text,
	FOREIGN KEY (plugin_id) REFERENCES plugin(id)
);

-- alle measurement configs die im config Ordner gespeichert sind
CREATE TABLE measurementConfig (
	id integer PRIMARY KEY autoincrement,
	name text
);

-- alle Element Typen die es fuer eine OpenIndy Installation gibt
CREATE TABLE element (
	id integer PRIMARY KEY autoincrement,
	measurementConfig_id integer,
	element_type text,
	FOREIGN KEY (measurementConfig_id) REFERENCES measurementConfig(id)
);

-- fuer jedes Plugin alle Elements, die zur Berechnung benötigt werden
CREATE TABLE pluginElement (
	id integer PRIMARY KEY autoincrement,
	functionPlugin_id integer,
	element_id integer,
	element_infinite boolean,
	FOREIGN KEY (functionPlugin_id) REFERENCES functionPlugin(id),
	FOREIGN KEY (element_id) REFERENCES element(id)
);

-- fuer jedes Feature alle Plugins, die dieses Feature als Ergebnis liefern
CREATE TABLE elementPlugin (
	id integer PRIMARY KEY autoincrement,
	element_id integer,
	functionPlugin_id integer,
	use_as_default boolean,
	FOREIGN KEY (element_id) REFERENCES element(id),
	FOREIGN KEY (functionPlugin_id) REFERENCES functionPlugin(id)
);

-- alle GUI Tools eines Plugins
CREATE TABLE toolPlugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	plugin_id integer,
	name text,
	description text,
	FOREIGN KEY (plugin_id) REFERENCES plugin(id)
);

-- alle Im- und Export Algorithmen eines Plugins
CREATE TABLE exchangePlugin (
	id integer PRIMARY KEY autoincrement,
	iid text,
	plugin_id integer,
	name text,
	description text,
	FOREIGN KEY (plugin_id) REFERENCES plugin(id)
);

-- für jeden Im- und Export Algorithmus alle Geometrien, die unterstützt werden
CREATE TABLE elementExchange (
	id integer PRIMARY KEY autoincrement,
	element_id integer,
	exchangePlugin_id integer,
	use_as_default boolean,
	FOREIGN KEY (element_id) REFERENCES element(id),
	FOREIGN KEY (exchangePlugin_id) REFERENCES exchangePlugin(id)
);

-- für jedes special format Plugin alle Dateiendungen, die unterstützt werden
CREATE TABLE exchangeFileExtensions (
	id integer PRIMARY KEY autoincrement,
	exchangePlugin_id integer,
	file_extension text,
	FOREIGN KEY (exchangePlugin_id) REFERENCES exchangePlugin(id)
);
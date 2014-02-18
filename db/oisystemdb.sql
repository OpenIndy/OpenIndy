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

-- alle Feature Typen die es fuer eine OpenIndy Installation gibt
CREATE TABLE element (
	id integer PRIMARY KEY autoincrement,
	element_type text
);

-- fuer jedes Plugin alle Features, die zur Berechnung benötigt werden
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
	FOREIGN KEY (element_id) REFERENCES element(id),
	FOREIGN KEY (functionPlugin_id) REFERENCES functionPlugin(id)
);
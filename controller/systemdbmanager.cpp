#include "systemdbmanager.h"

//initialize static variables
QSqlDatabase SystemDbManager::db;
bool SystemDbManager::isInit = false;

QThreadStorage<QMap<QString, sdb::Plugin> > SystemDbManager::caches;

/*!
 * \brief SystemDbManager::addPlugin
 * Saves an OpenIndy plugin in the system database
 * \param plugin
 * \return
 */
bool SystemDbManager::addPlugin(const sdb::Plugin &plugin){

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        bool transaction = SystemDbManager::db.transaction();
        if(transaction){

            //save plugin and get the id
            int pluginId = SystemDbManager::savePlugin(plugin);

            //if the plugin was saved save the available plugin implementations
            if(pluginId > -1){

                foreach(const sdb::Function &function, plugin.functions){
                    SystemDbManager::saveFunction(function, pluginId);
                }
                foreach(const sdb::Sensor &sensor, plugin.sensors){
                    SystemDbManager::saveSensor(sensor, pluginId);
                }
                foreach(const sdb::Simulation &simulation, plugin.simulations){
                    SystemDbManager::saveSimulation(simulation, pluginId);
                }
                foreach(const sdb::Exchange &exchange, plugin.exchanges){
                    SystemDbManager::saveExchange(exchange, pluginId);
                }
                foreach(const sdb::Tool &tool, plugin.tools){
                    SystemDbManager::saveTool(tool, pluginId);
                }
                foreach(const sdb::NetworkAdjustment &networkAdjustment, plugin.networkAdjustments){
                    SystemDbManager::saveNetworkAdjustment(networkAdjustment, pluginId);
                }

            }

            if(!SystemDbManager::db.commit()){
                SystemDbManager::db.rollback();
                SystemDbManager::disconnect();
                return false;
            }

        }else{
            Console::getInstance()->addLine( QString("Database error: %1").arg(SystemDbManager::db.lastError().text()), eCriticalMessage );
            SystemDbManager::disconnect();
            return false;
        }

        SystemDbManager::disconnect();
        return true;

    }

    return false;

}

/*!
 * \brief SystemDbManager::removePlugin
 * Removes an OpenIndy plugin from the system database
 * \param plugin
 * \return
 */
bool SystemDbManager::removePlugin(const sdb::Plugin &plugin){

    //TODO implement plugin deletion

    return false;

}

/*!
 * \brief SystemDbManager::getPlugins
 * \return
 */
QList<sdb::Plugin> SystemDbManager::getPlugins(){

    QList<sdb::Plugin> plugins;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //second command for plugin implementations
        QSqlQuery command2(SystemDbManager::db);
        QString query2;

        //third command for function plugins
        QSqlQuery command3(SystemDbManager::db);
        QString query3;

        //query all available plugins
        query = QString("SELECT %1 %2 FROM plugin")
                .arg("id, iid, name, description, version, author, compiler, operating_sys,")
                .arg("has_dependencies, file_path, is_active");
        command.exec(query);
        qDebug() << query;
        while(command.next()){

            sdb::Plugin plugin;
            plugin.id = command.value("id").toInt();
            plugin.iid = command.value("iid").toString();
            plugin.name = command.value("name").toString();
            plugin.description = command.value("description").toString();
            plugin.version = command.value("version").toString();
            plugin.author = command.value("author").toString();
            plugin.compiler = command.value("compiler").toString();
            plugin.operating_sys = command.value("operating_sys").toString();
            plugin.has_dependencies = command.value("has_dependencies").toBool();
            plugin.file_path = command.value("file_path").toString();
            plugin.is_active = command.value("is_active").toBool();

            //query all available sensors
            query2 = QString("SELECT id, iid, name, description FROM sensorPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Sensor sensor;
                sensor.id = command2.value("id").toInt();
                sensor.iid = command2.value("iid").toString();
                sensor.name = command2.value("name").toString();
                sensor.description = command2.value("description").toString();
                plugin.sensors.append(sensor);
            }

            //query all available simulations
            query2 = QString("SELECT id, iid, name, description FROM simulationPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Simulation simulation;
                simulation.id = command2.value("id").toInt();
                simulation.iid = command2.value("iid").toString();
                simulation.name = command2.value("name").toString();
                simulation.description = command2.value("description").toString();
                plugin.simulations.append(simulation);
            }

            //query all available network adjustments
            query2 = QString("SELECT id, iid, name, description FROM networkAdjustmentPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::NetworkAdjustment networkAdjustment;
                networkAdjustment.id = command2.value("id").toInt();
                networkAdjustment.iid = command2.value("iid").toString();
                networkAdjustment.name = command2.value("name").toString();
                networkAdjustment.description = command2.value("description").toString();
                plugin.networkAdjustments.append(networkAdjustment);
            }

            //query all available tools
            query2 = QString("SELECT id, iid, name, description FROM toolPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Tool tool;
                tool.id = command2.value("id").toInt();
                tool.iid = command2.value("iid").toString();
                tool.name = command2.value("name").toString();
                tool.description = command2.value("description").toString();
                plugin.tools.append(tool);
            }

            //query all available exchanges
            query2 = QString("SELECT id, iid, name, description FROM exchangePlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Exchange exchange;
                exchange.id = command2.value("id").toInt();
                exchange.iid = command2.value("iid").toString();
                exchange.name = command2.value("name").toString();
                exchange.description = command2.value("description").toString();
                plugin.exchanges.append(exchange);
            }

            //query all available functions
            query2 = QString("SELECT id, iid, name, description FROM functionPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Function function;
                function.id = command2.value("id").toInt();
                function.iid = command2.value("iid").toString();
                function.name = command2.value("name").toString();
                function.description = command2.value("description").toString();

                //query needed elements
                query3 = QString("SELECT e.element_type AS element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                        .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1").arg(function.id));
                command3.exec(query3);
                while(command3.next()){
                    function.neededElements.append(getElementTypeEnum(command3.value("element_type").toString()));
                }

                //query applicable for
                query3 = QString("SELECT e.element_type AS element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                        .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1").arg(function.id));
                command3.exec(query3);
                while(command3.next()){
                    function.applicableFor.append(getFeatureTypeEnum(command3.value("element_type").toString()));
                }

                plugin.functions.append(function);
            }

            plugins.append(plugin);

        }

        SystemDbManager::disconnect();

    }

    return plugins;

}

/*!
 * \brief SystemDbManager::getPlugin
 * Get the plugin with the specified name from cache or database
 * \param name
 * \return
 */
sdb::Plugin SystemDbManager::getPlugin(const QString &name) {
    if(caches.localData().contains(name)) {
        return caches.localData().value(name);
    } else {
        sdb::Plugin plugin = SystemDbManager::getPluginFromDB(name);
        if(plugin.id > -1) {
            caches.localData().insert(name, plugin);
        }
        return plugin;
    }
}

 /*!
  * \brief SystemDbManager::getPluginFromDB
  * Get the plugin with the specified name
  * \param name
  * \return
  */
sdb::Plugin SystemDbManager::getPluginFromDB(const QString &name){

    sdb::Plugin plugin;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //second command for plugin implementations
        QSqlQuery command2(SystemDbManager::db);
        QString query2;

        //third command for function plugins
        QSqlQuery command3(SystemDbManager::db);
        QString query3;

        //query all available plugins
        query = QString("SELECT %1 %2 FROM plugin WHERE name = '%3'")
                .arg("id, iid, name, description, version, author, compiler, operating_sys,")
                .arg("has_dependencies, file_path, is_active")
                .arg(name);
        command.exec(query);
        if(command.next()){

            plugin.id = command.value("id").toInt();
            plugin.iid = command.value("iid").toString();
            plugin.name = command.value("name").toString();
            plugin.description = command.value("description").toString();
            plugin.version = command.value("version").toString();
            plugin.author = command.value("author").toString();
            plugin.compiler = command.value("compiler").toString();
            plugin.operating_sys = command.value("operating_sys").toString();
            plugin.has_dependencies = command.value("has_dependencies").toBool();
            plugin.file_path = command.value("file_path").toString();
            plugin.is_active = command.value("is_active").toBool();

            //query all available sensors
            query2 = QString("SELECT id, iid, name, description FROM sensorPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Sensor sensor;
                sensor.id = command2.value("id").toInt();
                sensor.iid = command2.value("iid").toString();
                sensor.name = command2.value("name").toString();
                sensor.description = command2.value("description").toString();
                plugin.sensors.append(sensor);
            }

            //query all available simulations
            query2 = QString("SELECT id, iid, name, description FROM simulationPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Simulation simulation;
                simulation.id = command2.value("id").toInt();
                simulation.iid = command2.value("iid").toString();
                simulation.name = command2.value("name").toString();
                simulation.description = command2.value("description").toString();
                plugin.simulations.append(simulation);
            }

            //query all available network adjustments
            query2 = QString("SELECT id, iid, name, description FROM networkAdjustmentPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::NetworkAdjustment networkAdjustment;
                networkAdjustment.id = command2.value("id").toInt();
                networkAdjustment.iid = command2.value("iid").toString();
                networkAdjustment.name = command2.value("name").toString();
                networkAdjustment.description = command2.value("description").toString();
                plugin.networkAdjustments.append(networkAdjustment);
            }

            //query all available tools
            query2 = QString("SELECT id, iid, name, description FROM toolPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Tool tool;
                tool.id = command2.value("id").toInt();
                tool.iid = command2.value("iid").toString();
                tool.name = command2.value("name").toString();
                tool.description = command2.value("description").toString();
                plugin.tools.append(tool);
            }

            //query all available exchanges
            query2 = QString("SELECT id, iid, name, description FROM exchangePlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Exchange exchange;
                exchange.id = command2.value("id").toInt();
                exchange.iid = command2.value("iid").toString();
                exchange.name = command2.value("name").toString();
                exchange.description = command2.value("description").toString();
                plugin.exchanges.append(exchange);
            }

            //query all available functions
            query2 = QString("SELECT id, iid, name, description FROM functionPlugin WHERE plugin_id = %1").arg(plugin.id);
            command2.exec(query2);
            while(command2.next()){
                sdb::Function function;
                function.id = command2.value("id").toInt();
                function.iid = command2.value("iid").toString();
                function.name = command2.value("name").toString();
                function.description = command2.value("description").toString();

                //query needed elements
                query3 = QString("SELECT e.element_type AS element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                        .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1").arg(function.id));
                command3.exec(query3);
                while(command3.next()){
                    function.neededElements.append(getElementTypeEnum(command3.value("element_type").toString()));
                }

                //query applicable for
                query3 = QString("SELECT e.element_type AS element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                        .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1").arg(function.id));
                command3.exec(query3);
                while(command3.next()){
                    function.applicableFor.append(getFeatureTypeEnum(command3.value("element_type").toString()));
                }

                plugin.functions.append(function);
            }

        }

        SystemDbManager::disconnect();

    }

    return plugin;

}

/*!
 * \brief SystemDbManager::getSensors
 * Returns a list of all available sensors
 * \return
 */
QList<sdb::Sensor> SystemDbManager::getSensors(){

    QList<sdb::Sensor> sensors;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM sensorPlugin AS sp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, sp.name AS name,")
                .arg("sp.id AS id, sp.iid AS iid, sp.description AS description")
                .arg("ON sp.plugin_id = p.id");
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up sensor
            sdb::Sensor sensor;
            sensor.id = command.value("id").toInt();
            sensor.iid = command.value("iid").toString();
            sensor.name = command.value("name").toString();
            sensor.description = command.value("description").toString();
            sensor.plugin = plugin;

            sensors.append(sensor);

        }

        SystemDbManager::disconnect();

    }

    return sensors;

}

/*!
 * \brief SystemDbManager::getLaserTrackers
 * Returns a list of all available laser trackers
 * \return
 */
QList<sdb::Sensor> SystemDbManager::getLaserTrackers(){

    QList<sdb::Sensor> sensors;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM sensorPlugin AS sp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, sp.name AS name,")
                .arg("sp.id AS id, sp.iid AS iid, sp.description AS description")
                .arg(QString("ON sp.plugin_id = p.id WHERE sp.iid = '%1'").arg(OiMetaData::iid_LaserTracker));
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up sensor
            sdb::Sensor sensor;
            sensor.id = command.value("id").toInt();
            sensor.iid = command.value("iid").toString();
            sensor.name = command.value("name").toString();
            sensor.description = command.value("description").toString();
            sensor.plugin = plugin;

            sensors.append(sensor);

        }

        SystemDbManager::disconnect();

    }

    return sensors;

}

/*!
 * \brief SystemDbManager::getTotalStations
 * Returns a list of all available total stations
 * \return
 */
QList<sdb::Sensor> SystemDbManager::getTotalStations(){

    QList<sdb::Sensor> sensors;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM sensorPlugin AS sp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, sp.name AS name,")
                .arg("sp.id AS id, sp.iid AS iid, sp.description AS description")
                .arg(QString("ON sp.plugin_id = p.id WHERE sp.iid = '%1'").arg(OiMetaData::iid_TotalStation));
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up sensor
            sdb::Sensor sensor;
            sensor.id = command.value("id").toInt();
            sensor.iid = command.value("iid").toString();
            sensor.name = command.value("name").toString();
            sensor.description = command.value("description").toString();
            sensor.plugin = plugin;

            sensors.append(sensor);

        }

        SystemDbManager::disconnect();

    }

    return sensors;

}

/*!
 * \brief SystemDbManager::getFunctions
 * Returns a list of all available functions
 * \return
 */
QList<sdb::Function> SystemDbManager::getFunctions(){

    QList<sdb::Function> functions;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;
        QSqlQuery command2(SystemDbManager::db);
        QString query2;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM functionPlugin AS fp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, fp.name AS name,")
                .arg("fp.id AS id, fp.iid AS iid, fp.description AS description")
                .arg("ON fp.plugin_id = p.id");
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up function
            sdb::Function function;
            function.id = command.value("id").toInt();
            function.iid = command.value("iid").toString();
            function.name = command.value("name").toString();
            function.description = command.value("description").toString();
            function.plugin = plugin;

            //query needed elements
            query2 = QString("SELECT e.element_type AS element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1").arg(function.id));
            command2.exec(query2);
            while(command2.next()){
                function.neededElements.append(getElementTypeEnum(command2.value("element_type").toString()));
            }

            //query applicable for
            query2 = QString("SELECT e.element_type AS element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1").arg(function.id));
            command2.exec(query2);
            while(command2.next()){
                function.applicableFor.append(getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            functions.append(function);

        }

        SystemDbManager::disconnect();

    }

    return functions;

}

/*!
 * \brief SystemDbManager::getCreateFunctions
 * Returns a list of all available create functions
 * \param type
 * \return
 */
QList<sdb::Function> SystemDbManager::getCreateFunctions(const FeatureTypes &type){

    QList<sdb::Function> functions;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;
        QSqlQuery command2(SystemDbManager::db);
        QString query2;

        //distinguish between trafo params and other features
        QString condition;
        if(type == eTrafoParamFeature){
            condition = QString("fp.iid = '%1'").arg(OiMetaData::iid_SystemTransformation);
        }else{
            condition = QString("fp.iid = '%1' OR fp.iid = '%2'").arg(OiMetaData::iid_FitFunction).arg(OiMetaData::iid_ConstructFunction);
        }

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM functionPlugin AS fp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, fp.name AS name,")
                .arg("fp.id AS id, fp.iid AS iid, fp.description AS description")
                .arg(QString("ON fp.plugin_id = p.id WHERE %1").arg(condition));
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up function
            sdb::Function function;
            function.id = command.value("id").toInt();
            function.iid = command.value("iid").toString();
            function.name = command.value("name").toString();
            function.description = command.value("description").toString();
            function.plugin = plugin;

            //query needed elements
            query2 = QString("SELECT e.element_type AS element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1").arg(function.id));
            command2.exec(query2);
            while(command2.next()){
                function.neededElements.append(getElementTypeEnum(command2.value("element_type").toString()));
            }

            //query applicable for
            query2 = QString("SELECT e.element_type AS element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1").arg(function.id));
            command2.exec(query2);
            while(command2.next()){
                function.applicableFor.append(getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            //only add the function if it is applicable for the specified feature type
            if(function.applicableFor.contains(type)){
                functions.append(function);
            }

        }

        SystemDbManager::disconnect();

    }

    return functions;

}

/*!
 * \brief SystemDbManager::getChangeFunctions
 * Returns a list of all available change functions
 * \param type
 * \return
 */
QList<sdb::Function> SystemDbManager::getChangeFunctions(const FeatureTypes &type){

    QList<sdb::Function> functions;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;
        QSqlQuery command2(SystemDbManager::db);
        QString query2;

        //exclude fit, construct functions and system trafos
        QString condition = QString("NOT (fp.iid = '%1' OR fp.iid = '%2' OR fp.iid = '%3')")
                .arg(OiMetaData::iid_SystemTransformation)
                .arg(OiMetaData::iid_FitFunction)
                .arg(OiMetaData::iid_ConstructFunction);

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM functionPlugin AS fp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, fp.name AS name,")
                .arg("fp.id AS id, fp.iid AS iid, fp.description AS description")
                .arg(QString("ON fp.plugin_id = p.id WHERE %1").arg(condition));
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up function
            sdb::Function function;
            function.id = command.value("id").toInt();
            function.iid = command.value("iid").toString();
            function.name = command.value("name").toString();
            function.description = command.value("description").toString();
            function.plugin = plugin;

            //query needed elements
            query2 = QString("SELECT e.element_type AS element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1").arg(function.id));
            command2.exec(query2);
            while(command2.next()){
                function.neededElements.append(getElementTypeEnum(command2.value("element_type").toString()));
            }

            //query applicable for
            query2 = QString("SELECT e.element_type AS element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1").arg(function.id));
            command2.exec(query2);
            while(command2.next()){
                function.applicableFor.append(getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            //only add the function if it is applicable for the specified feature type
            if(function.applicableFor.contains(type)){
                functions.append(function);
            }

        }

        SystemDbManager::disconnect();

    }

    return functions;

}

/*!
 * \brief SystemDbManager::getDefaultFunction
 * \param type
 * \return
 */
sdb::Function SystemDbManager::getDefaultFunction(const FeatureTypes &type){

    sdb::Function result;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);

        QString query = QString("SELECT %1 FROM functionPlugin AS fp %2 %3 %4 WHERE %5")
                .arg("fp.id, fp.iid, fp.name, fp.description, p.name AS pluginName, p.file_path AS filepath")
                .arg("INNER JOIN elementPlugin AS ep ON fp.id = ep.functionPlugin_id")
                .arg("INNER JOIN plugin AS p ON p.id = fp.plugin_id")
                .arg("INNER JOIN element AS e ON e.id = ep.element_id")
                .arg(QString("e.element_type = \'%1\' AND ep.use_as_default = 1").arg(getFeatureTypeName(type)));

        command.exec(query);
        while(command.next()){

            result.id = command.value("id").toInt();
            result.iid = command.value("iid").toString();
            result.name = command.value("name").toString();
            result.description = command.value("description").toString();
            sdb::Plugin plugin;
            plugin.name = command.value("pluginName").toString();
            plugin.file_path = command.value("filepath").toString();
            result.plugin = plugin;

            QSqlQuery command2(SystemDbManager::db);

            //query applicableFor features
            QString queryApplicableFor = QString("SELECT e.element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1")
                         .arg(result.id));
            command2.exec(queryApplicableFor);
            while(command2.next()){
                result.applicableFor.append(getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            //query neededElements elements
            QString queryNeededElements = QString("SELECT e.element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1")
                         .arg(result.id));
            command2.exec(queryNeededElements);
            while(command2.next()){
                result.neededElements.append(getElementTypeEnum(command2.value("element_type").toString()));
            }

            break;

        }

        SystemDbManager::disconnect();
    }

    return result;

}

/*!
 * \brief SystemDbManager::setDefaultFunction
 * \param type
 * \param functionName
 * \param pluginPath
 */
void SystemDbManager::setDefaultFunction(const FeatureTypes &type, const QString &functionName, const QString &pluginPath){

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        bool transaction = SystemDbManager::db.transaction();
        if(transaction){

            QSqlQuery command(SystemDbManager::db);

            //set default state of all functions for featureType to false
            QString query = QString("UPDATE elementPlugin SET use_as_default = 0 WHERE id IN %1")
                    .arg(QString("(SELECT ep.id FROM elementPlugin AS ep %1 %2")
                         .arg("INNER JOIN element AS e ON ep.element_id = e.id")
                         .arg(QString("WHERE e.element_type = \'%1\')").arg(getFeatureTypeName(type))));

            if(command.exec(query)){

                //set default state of the given function to true
                query = QString("UPDATE elementPlugin SET use_as_default = 1 WHERE id IN %1")
                        .arg(QString("(SELECT ep.id FROM elementPlugin AS ep %1 %2 %3 %4")
                             .arg("INNER JOIN element AS e ON ep.element_id = e.id")
                             .arg("INNER JOIN functionPlugin AS fp ON ep.functionPlugin_id = fp.id")
                             .arg("INNER JOIN plugin AS p ON fp.plugin_id = p.id")
                             .arg(QString("WHERE e.element_type = \'%1\' AND fp.name = \'%2\' AND p.file_path = \'%3\')")
                                  .arg(getFeatureTypeName(type))
                                  .arg(functionName)
                                  .arg(pluginPath)));
                command.exec(query);

            }

            if(!SystemDbManager::db.commit()){
                SystemDbManager::db.rollback();
            }

        }else{
            Console::getInstance()->addLine( QString("Database error: %1").arg(SystemDbManager::db.lastError().text()), eCriticalMessage );
        }

        SystemDbManager::disconnect();
    }

}

/*!
 * \brief SystemDbManager::getTools
 * Returns a list of all available tools
 * \return
 */
QList<sdb::Tool> SystemDbManager::getTools(){

    QList<sdb::Tool> tools;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM toolPlugin AS tp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, tp.name AS name,")
                .arg("tp.id AS id, tp.iid AS iid, tp.description AS description")
                .arg("ON tp.plugin_id = p.id");
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up tool
            sdb::Tool tool;
            tool.id = command.value("id").toInt();
            tool.iid = command.value("iid").toString();
            tool.name = command.value("name").toString();
            tool.description = command.value("description").toString();
            tool.plugin = plugin;

            tools.append(tool);

        }

        SystemDbManager::disconnect();

    }

    return tools;

}

/*!
 * \brief SystemDbManager::getSimulations
 * Returns a list of all available simulations
 * \return
 */
QList<sdb::Simulation> SystemDbManager::getSimulations(){

    QList<sdb::Simulation> simulations;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM simulationPlugin AS sp INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, sp.name AS name,")
                .arg("sp.id AS id, sp.iid AS iid, sp.description AS description")
                .arg("ON sp.plugin_id = p.id");
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up simulation
            sdb::Simulation simulation;
            simulation.id = command.value("id").toInt();
            simulation.iid = command.value("iid").toString();
            simulation.name = command.value("name").toString();
            simulation.description = command.value("description").toString();
            simulation.plugin = plugin;

            simulations.append(simulation);

        }

        SystemDbManager::disconnect();

    }

    return simulations;

}

/*!
 * \brief SystemDbManager::getNetworkAdjustments
 * Returns a list of all available network adjustments
 * \return
 */
QList<sdb::NetworkAdjustment> SystemDbManager::getNetworkAdjustments(){

    QList<sdb::NetworkAdjustment> networkAdjustments;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM networkAdjustmentPlugin AS nap INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, nap.name AS name,")
                .arg("nap.id AS id, nap.iid AS iid, nap.description AS description")
                .arg("ON nap.plugin_id = p.id");
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up networkAdjustment
            sdb::NetworkAdjustment networkAdjustment;
            networkAdjustment.id = command.value("id").toInt();
            networkAdjustment.iid = command.value("iid").toString();
            networkAdjustment.name = command.value("name").toString();
            networkAdjustment.description = command.value("description").toString();
            networkAdjustment.plugin = plugin;

            networkAdjustments.append(networkAdjustment);

        }

        SystemDbManager::disconnect();

    }

    return networkAdjustments;

}

/*!
 * \brief SystemDbManager::getExchanges
 * \return
 */
QList<sdb::Exchange> SystemDbManager::getExchanges(){

    QList<sdb::Exchange> exchanges;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT %1 %2 %3 %4 %5 FROM exchangePlugin AS ep INNER JOIN plugin AS p %6")
                .arg("p.id AS pid, p.iid AS piid, p.name AS pname, p.description AS pdescription,")
                .arg("p.version AS pversion, p.author AS pauthor, p.compiler AS pcompiler,")
                .arg("p.operating_sys AS poperating_sys, p.has_dependencies AS phas_dependencies,")
                .arg("p.file_path AS pfile_path, p.is_active AS pis_active, ep.name AS name,")
                .arg("ep.id AS id, ep.iid AS iid, ep.description AS description")
                .arg("ON ep.plugin_id = p.id");
        command.exec(query);
        while(command.next()){

            //set up plugin
            sdb::Plugin plugin;
            plugin.id = command.value("pid").toInt();
            plugin.iid = command.value("piid").toString();
            plugin.name = command.value("pname").toString();
            plugin.description = command.value("pdescription").toString();
            plugin.version = command.value("pversion").toString();
            plugin.author = command.value("pauthor").toString();
            plugin.compiler = command.value("pcompiler").toString();
            plugin.operating_sys = command.value("poperating_sys").toString();
            plugin.has_dependencies = command.value("phas_dependencies").toBool();
            plugin.file_path = command.value("pfile_path").toString();
            plugin.is_active = command.value("pis_active").toBool();

            //set up exchange
            sdb::Exchange exchange;
            exchange.id = command.value("id").toInt();
            exchange.iid = command.value("iid").toString();
            exchange.name = command.value("name").toString();
            exchange.description = command.value("description").toString();
            exchange.plugin = plugin;

            exchanges.append(exchange);

        }

        SystemDbManager::disconnect();

    }

    return exchanges;

}

/*!
 * \brief SystemDbManager::getMeasurementConfigs
 * Returns a list of all available measurement config names
 * \return
 */
QStringList SystemDbManager::getMeasurementConfigs(){

    QStringList mConfigs;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        //query all available plugins
        query = QString("SELECT name FROM measurementConfig");
        command.exec(query);
        while(command.next()){
            mConfigs.append(command.value("name").toString());
        }

        SystemDbManager::disconnect();

    }

    return mConfigs;

}

/*!
 * \brief SystemDbManager::addMeasurementConfig
 * Adds a measurement config if not yet exists
 * \param name
 * \return
 */
bool SystemDbManager::addMeasurementConfig(const QString &name){

    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QString query = QString("INSERT INTO measurementConfig (name) SELECT '%1' %2;")
                .arg(name)
                .arg(QString("WHERE NOT EXISTS(SELECT 1 FROM measurementConfig WHERE name = '%1')")
                     .arg(name));

        QSqlQuery command(SystemDbManager::db);
        check = command.exec(query);

        SystemDbManager::disconnect();

    }

    return check;

}

/*!
 * \brief SystemDbManager::removeMeasurementConfig
 * Remove a measurement config if it exists
 * \param name
 * \return
 */
bool SystemDbManager::removeMeasurementConfig(const QString &name){

    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QString query = QString("DELETE FROM measurementConfig WHERE name = '%1';").arg(name);

        QSqlQuery command(SystemDbManager::db);
        check = command.exec(query);

        SystemDbManager::disconnect();

    }

    return check;

}

/*!
 * \brief SystemDbManager::getDefaultMeasurementConfig
 * Returns the default measurement config for the specified element type or an empty string
 * \param elementType
 * \return
 */
QString SystemDbManager::getDefaultMeasurementConfig(const QString &elementType){

    QString name;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QString query = QString("SELECT m.name FROM measurementConfig AS m INNER JOIN element AS e")
                .append(" ON m.id = e.measurementConfig_id WHERE element_type = '%1'").arg(elementType);

        QSqlQuery command(SystemDbManager::db);
        command.exec(query);

        if(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                name = val.toString();
            }
        }

        SystemDbManager::disconnect();

    }

    return name;

}

/*!
 * \brief SystemDbManager::setDefaultMeasurementConfig
 * Set the default measurement config for the specified element type
 * \param name
 * \param elementType
 * \return
 */
bool SystemDbManager::setDefaultMeasurementConfig(const QString &name, const QString &elementType){

    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }

    if(SystemDbManager::connect()){

        QString query;
        QSqlQuery command(SystemDbManager::db);

        //get the id of the measurement config with name
        int mConfigId = -1;
        query = QString("SELECT id FROM measurementConfig WHERE name = '%1'").arg(name);
        command.exec(query);
        if(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                mConfigId = val.toInt();
            }
        }

        //return false if no measurement config with name is in the database
        if(mConfigId < 0){
            return false;
        }

        //save that measurement config as default for the given geometry type
        query = QString("UPDATE element SET measurementConfig_id = %1 WHERE element_type = '%2'")
                .arg(mConfigId).arg(elementType);
        check = command.exec(query);

        SystemDbManager::disconnect();

    }

    return check;

}

/*!
 * \brief SystemDbManager::getSensorConfigs
 * Returns a list of all available sensor configs
 * \return
 */
QStringList SystemDbManager::getSensorConfigs(){

    QStringList sConfigs;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);
        QString query;

        query = QString("SELECT name FROM sensorConfig");
        command.exec(query);
        while(command.next()){
            sConfigs.append(command.value("name").toString());
        }

        SystemDbManager::disconnect();

    }

    return sConfigs;

}

/*!
 * \brief SystemDbManager::addSensorConfig
 * Adds a sensor config if not yet exists
 * \param name
 * \return
 */
bool SystemDbManager::addSensorConfig(const QString &name){

    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }

    if(SystemDbManager::connect()){

        QString query = QString("INSERT INTO sensorConfig (name, use_as_default) SELECT '%1', 0 %2;")
                .arg(name)
                .arg(QString("WHERE NOT EXISTS(SELECT 1 FROM sensorConfig WHERE name = '%1')")
                     .arg(name));

        QSqlQuery command(SystemDbManager::db);
        check = command.exec(query);

        SystemDbManager::disconnect();

    }

    return check;

}

/*!
 * \brief SystemDbManager::removeSensorConfig
 * \param name
 * \return
 */
bool SystemDbManager::removeSensorConfig(const QString &name){

    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }

    if(SystemDbManager::connect()){

        QString query = QString("DELETE FROM sensorConfigs WHERE name = '%1';").arg(name);

        QSqlQuery command(SystemDbManager::db);
        check = command.exec(query);

        SystemDbManager::disconnect();

    }

    return check;

}

/*!
 * \brief SystemDbManager::getDefaultSensorConfig
 * Returns the name of the default sensor config or an empty string
 * \return
 */
QString SystemDbManager::getDefaultSensorConfig(){

    QString name;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QString query = QString("SELECT name FROM sensorConfig WHERE use_as_default = 1;");

        QSqlQuery command(SystemDbManager::db);
        command.exec(query);

        if(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                name = val.toString();
            }
        }

        SystemDbManager::disconnect();

    }

    return name;

}

/*!
 * \brief SystemDbManager::setDefaultSensorConfig
 * Set the default sensor config
 * \param name
 * \return
 */
bool SystemDbManager::setDefaultSensorConfig(const QString &name){

    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }

    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);

        //set all sensor configs to not be the default
        QString query = QString("UPDATE sensorConfig SET use_as_default = 0;");
        check = command.exec(query);

        //set the given sensor config as default
        if(check){
            query = QString("UPDATE sensorConfig SET use_as_default = 1 WHERE name = '%1';").arg(name);
            check = command.exec(query);
        }

        SystemDbManager::disconnect();

    }

    return check;

}

/*!
 * \brief SystemDbManager::init
 */
void SystemDbManager::init(){

    SystemDbManager::db = QSqlDatabase::addDatabase("QSQLITE", "oisystemdb");

#ifdef Q_OS_MAC
    QString dbPath(qApp->applicationDirPath());
#endif

#ifdef Q_OS_WIN
    QString dbPath(qApp->applicationDirPath());
#endif

#ifdef Q_OS_LINUX
    QString dbPath(qApp->applicationDirPath());
#endif

    dbPath.append("/oisystemdb.sqlite");

    SystemDbManager::db.setDatabaseName(dbPath);
    SystemDbManager::isInit = true;

}

/*!
 * \brief SystemDbManager::connect
 * \return
 */
bool SystemDbManager::connect(){

    QFileInfo checkFile(SystemDbManager::db.databaseName());

    if(checkFile.isFile()){
        SystemDbManager::db.open();
    }
    return SystemDbManager::db.isOpen();

}

/*!
 * \brief SystemDbManager::disconnect
 */
void SystemDbManager::disconnect(){
    if(SystemDbManager::db.isOpen()){
        SystemDbManager::db.close();
    }
}

/*!
 * \brief SystemDbManager::savePlugin
 * Saves the given plugin in the database and returns the plugin id or -1
 * \param plugin
 * \return
 */
int SystemDbManager::savePlugin(const sdb::Plugin &plugin){

    int pluginId = -1;

    QDir pluginDir(qApp->applicationDirPath()); //get application path
    QString relativePath = pluginDir.relativeFilePath(plugin.file_path); //get relative path to plugin with application path as basis

    QString query = QString("INSERT INTO plugin (iid, name, description, version, author, compiler, operating_sys, "
                            "has_dependencies, file_path, is_active) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', %8, '%9', %10);")
        .arg(plugin.iid).arg(plugin.name).arg(plugin.description).arg(plugin.version)
        .arg(plugin.author).arg(plugin.compiler).arg(plugin.operating_sys).arg(QString(plugin.has_dependencies?"1":"0"))
        .arg(relativePath).arg("1");

    QSqlQuery command(SystemDbManager::db);
    bool check = command.exec(query);

    if(check){
        pluginId = SystemDbManager::getLastId("plugin");
    }

    if(command.lastError().isValid()){
        Console::getInstance()->addLine( QString("Database error: %1").arg(command.lastError().text()), eCriticalMessage );
        pluginId = -1;
    }

    return pluginId;

}

/*!
 * \brief SystemDbManager::saveFunction
 * \param function
 * \param plugin_id
 */
void SystemDbManager::saveFunction(const sdb::Function &function, const int &plugin_id){

    QStringList elements;

    //get id's of features for which this plugin is applicable
    for(int i=0; i<function.applicableFor.size(); i++){
        elements.append( getFeatureTypeName(function.applicableFor.at(i)) );
    }
    QList<int> applicableFor = SystemDbManager::getElementIds(elements);

    elements.clear();

    //get id's of elements which this plugin needs to be able to execute
    for(int i=0; i<function.neededElements.size(); i++){
        elements.append( getElementTypeName(function.neededElements.at(i)) );
    }
    QList<int> neededElements = SystemDbManager::getElementIds(elements);

    if(applicableFor.length() > 0){
        //insert function plugin
        QString query = QString("INSERT INTO functionPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
                .arg(plugin_id).arg(function.iid).arg(function.name).arg(function.description);
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);

        //get id of function plugin
        int fplugin_id = SystemDbManager::getLastId("functionPlugin");

        if(fplugin_id > -1){
            if(neededElements.length() > 0){
                //insert needed elements
                query = QString("INSERT INTO pluginElement (functionPlugin_id, element_id, element_infinite) VALUES (%1, %2, %3)")
                        .arg(fplugin_id).arg(neededElements.at(0)).arg("0");
                for(int i = 1; i < neededElements.length(); i++){
                    query.append(QString(", (%1, %2, %3)").arg(fplugin_id).arg(neededElements.at(i)).arg("0"));
                }
                command.exec(query);
            }

            //insert applicable for
            query = QString("INSERT INTO elementPlugin (functionPlugin_id, element_id) VALUES (%1, %2)")
                    .arg(fplugin_id).arg(applicableFor.at(0));
            for(int i = 1; i < applicableFor.length(); i++){
                query.append( QString(", (%1, %2)").arg(fplugin_id).arg(applicableFor.at(i)) );
            }
            command.exec(query);
        }
    }

}

/*!
 * \brief SystemDbManager::saveSensor
 * \param sensor
 * \param plugin_id
 */
void SystemDbManager::saveSensor(const sdb::Sensor &sensor, const int &plugin_id){

    QString query = QString("INSERT INTO sensorPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(plugin_id).arg(sensor.iid).arg(sensor.name).arg(sensor.description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);

}

/*!
 * \brief SystemDbManager::saveSimulation
 * \param simulation
 * \param plugin_id
 */
void SystemDbManager::saveSimulation(const sdb::Simulation &simulation, const int &plugin_id){

    QString query = QString("INSERT INTO simulationPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(plugin_id).arg(simulation.iid).arg(simulation.name).arg(simulation.description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);

}

/*!
 * \brief SystemDbManager::saveExchange
 * \param exchange
 * \param plugin_id
 */
void SystemDbManager::saveExchange(const sdb::Exchange &exchange, const int &plugin_id){

    QString query = QString("INSERT INTO exchangePlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(plugin_id).arg(exchange.iid).arg(exchange.name).arg(exchange.description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);

}

/*!
 * \brief SystemDbManager::saveTool
 * \param tool
 * \param plugin_id
 */
void SystemDbManager::saveTool(const sdb::Tool &tool, const int &plugin_id){

    QString query = QString("INSERT INTO toolPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(plugin_id).arg(tool.iid).arg(tool.name).arg(tool.description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);

}

/*!
 * \brief SystemDbManager::saveNetworkAdjustment
 * \param networkAdjustment
 * \param plugin_id
 */
void SystemDbManager::saveNetworkAdjustment(const sdb::NetworkAdjustment &networkAdjustment, const int &plugin_id){

    QString query = QString("INSERT INTO networkAdjustmentPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(plugin_id).arg(networkAdjustment.iid).arg(networkAdjustment.name).arg(networkAdjustment.description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);

}

/*!
 * \brief SystemDbManager::getLastId
 * Get the id of the last inserted row of the specified table or -1
 * \param table
 * \return
 */
int SystemDbManager::getLastId(const QString &table){

    int id = -1;
    if(table.compare("")){
        QString query = QString("SELECT max(id) FROM %1").arg(table);
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);
        if(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                id = val.toInt();
            }
        }
    }
    return id;

}

/*!
 * \brief SystemDbManager::getElementIds
 * Returns a list of the element id's
 * \param elements
 * \return
 */
QList<int> SystemDbManager::getElementIds(const QStringList &elements){

    QList<int> result;
    if(elements.length() > 0){
        QString query = QString("SELECT id FROM element WHERE element_type = '%1'").arg(elements.at(0));
        for(int i = 1; i < elements.length(); i++){
            query.append( QString(" OR element_type = '%1'").arg(elements.at(i)) );
        }
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);
        while(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                result.append(val.toInt());
            }
        }
    }
    return result;

}

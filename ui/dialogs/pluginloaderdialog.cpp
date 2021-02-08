#include "pluginloaderdialog.h"
#include "ui_pluginloaderdialog.h"

/*!
 * \brief PluginLoaderDialog::PluginLoaderDialog
 * \param parent
 */
PluginLoaderDialog::PluginLoaderDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::PluginLoaderDialog)
{
    ui->setupUi(this);

    //connect plugin loader
    QObject::connect(PluginLoader::getInstance().data(), &PluginLoader::updateProgress,
                     this->ui->progressBar, &QProgressBar::setValue, Qt::AutoConnection);
    QObject::connect(PluginLoader::getInstance().data(), &PluginLoader::importFinished,
                     this, &PluginLoaderDialog::importFinished, Qt::AutoConnection);
    //QObject::connect(PluginLoader::getInstance().data(), &PluginLoader::sendMessage,
    //                 this, &PluginLoaderDialog::sendMessage, Qt::AutoConnection);
}

/*!
 * \brief PluginLoaderDialog::~PluginLoaderDialog
 */
PluginLoaderDialog::~PluginLoaderDialog(){
    delete ui;
}

/*!
 * \brief PluginLoaderDialog::on_toolButton_Path_clicked
 */
void PluginLoaderDialog::on_toolButton_Path_clicked(){

    //let the user choose a folder
    QString pluginDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    //get and check plugin meta data as a preview
    PluginMetaData metaData = PluginLoader::getPluginMetaData(pluginDir);
    if(metaData.iid.compare("") == 0){
        Console::getInstance()->addLine(QString("No plugin found at path: %1").arg(pluginDir), eErrorMessage, eMessageBoxMessage);
        return;
    }

    //show meta info in GUI
    this->ui->lineEdit_Path->setText(metaData.path);
    this->ui->lineEdit_Name->setText(metaData.name);
    this->ui->lineEdit_pluginType->setText(metaData.iid);

    this->ui->textBrowser_description->clear();
    this->ui->textBrowser_description->append("<b>name</b>: " + metaData.name);
    this->ui->textBrowser_description->append("<b>author</b>: " + metaData.author);
    this->ui->textBrowser_description->append("<b>version</b>: " + metaData.pluginVersion);
    this->ui->textBrowser_description->append("<b>operating system</b>: " + metaData.operatingSystem);
    this->ui->textBrowser_description->append("<b>compiler</b>: " + metaData.compiler + "<hr /><p />");

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(100);
    this->ui->progressBar->setValue(0);

    if(metaData.dependencies){
        this->ui->textBrowser_description->append("<b>dependencies</b>:" + QString::number(metaData.dependenciesPath.size()));
    }else{
        this->ui->textBrowser_description->append("<b>dependencies</b>: none");
    }

    for(int i = 0; i < metaData.dependenciesPath.size(); i++){

        QJsonObject v = metaData.dependenciesPath.at(i).toObject();

        QString depName = v.value("name").toString() ;
        QString depPath = v.value("path").toString() ;

        QString tmpText(QString("   -" + depName + ": " + depPath));

        this->ui->textBrowser_description->append(tmpText);

    }

    this->ui->pushButton_Ok->setEnabled(true);

}

/*!
 * \brief PluginLoaderDialog::on_pushButton_Cancel_clicked
 */
void PluginLoaderDialog::on_pushButton_Cancel_clicked(){
    this->close();
}

/*!
 * \brief PluginLoaderDialog::on_pushButton_Ok_clicked
 */
void PluginLoaderDialog::on_pushButton_Ok_clicked(){
    if(this->ui->lineEdit_Path->text().compare("") != 0){
        PluginLoader::importPlugin(this->ui->lineEdit_Path->text());
        this->ui->textBrowser_description->append("<hr /><p />");
    }
}

/*!
 * \brief PluginLoaderDialog::importFinished
 * \param success
 */
void PluginLoaderDialog::importFinished(const bool &success){
    if(success){
        this->ui->progressBar->setValue(this->ui->progressBar->maximum());
        QMessageBox::information(this, "success", "plugin has been copied successfully.");
    }else{
        this->ui->progressBar->setValue(ui->progressBar->minimum());
        QMessageBox::critical(this,"error", "plugin has not been copied successfully.");
    }
}

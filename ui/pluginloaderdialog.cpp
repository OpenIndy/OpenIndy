#include "pluginloaderdialog.h"
#include "ui_pluginloaderdialog.h"

PluginLoaderDialog::PluginLoaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginLoaderDialog)
{
    ui->setupUi(this);

}

PluginLoaderDialog::~PluginLoaderDialog()
{
    delete ui;
}

void PluginLoaderDialog::on_toolButton_Path_clicked()
{


    QString pluginDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);



    QString filename = PluginLoader::findOiPlugin(pluginDir);

    if (filename != NULL){

    this->pluginPath = filename;

    ui->lineEdit_Path->setText(filename);

    ui->lineEdit_Name->setText(PluginLoader::myMetaInfo->name);

    ui->lineEdit_pluginType->setText(PluginLoader::myMetaInfo->iid);

    ui->textBrowser_description->append("#######################");
    ui->textBrowser_description->append("name: " + PluginLoader::myMetaInfo->name);
    ui->textBrowser_description->append("author: " + PluginLoader::myMetaInfo->author);
    ui->textBrowser_description->append("#######################");
    ui->textBrowser_description->append("version: " + PluginLoader::myMetaInfo->pluginVersion);
    ui->textBrowser_description->append("#######################");
    ui->textBrowser_description->append("operating system: " + PluginLoader::myMetaInfo->operatingSystem);
    ui->textBrowser_description->append("compiler: " + PluginLoader::myMetaInfo->compiler);
    ui->textBrowser_description->append("#######################");

    if (PluginLoader::myMetaInfo->dependencies){
      ui->textBrowser_description->append("dependencies: " + PluginLoader::myMetaInfo->dependeciesPath.size());
    }else{
      ui->textBrowser_description->append("dependencies: none");
    }

    for(int i = 0; i<PluginLoader::myMetaInfo->dependeciesPath.size();i++){

        QJsonObject v = PluginLoader::myMetaInfo->dependeciesPath.at(i).toObject();

        QString depName = v.value("name").toString() ;
         QString depPath = v.value("path").toString() ;

         QString tmpText(depName + ": " + depPath);

        ui->textBrowser_description->append(tmpText);

    }

    ui->textBrowser_description->append("#######################");
    ui->textBrowser_description->append("description: ");
    ui->textBrowser_description->append(PluginLoader::myMetaInfo->description);


    QMessageBox::information(this,"valid", "plugin valid");
    this->pluginPath = filename;

     ui->progressBar->setMaximum(PluginLoader::myMetaInfo->dependeciesPath.size()+1);

     ui->pushButton_Ok->setEnabled(true);

    }else{
        QMessageBox::critical(this,"error", "plugin not valid");
    }


}

void PluginLoaderDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}

void PluginLoaderDialog::on_pushButton_Ok_clicked()
{
    bool check = PluginLoader::copyPlugin(pluginPath);

    if (check){
        QMessageBox::information(this,"success", "plugin has been copied successfully.");
    }else{
        QMessageBox::critical(this,"error", "plugin was not copied successfully.");
    }
}

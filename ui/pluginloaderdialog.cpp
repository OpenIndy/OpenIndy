#include "pluginloaderdialog.h"
#include "ui_pluginloaderdialog.h"

PluginLoaderDialog::PluginLoaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginLoaderDialog)
{
    ui->setupUi(this);

    connect(PluginLoader::getCopier(),SIGNAL(copyCount(int)),ui->progressBar,SLOT(setValue(int)));
    connect(PluginLoader::getCopier(),SIGNAL(sendString(QString)),this,SLOT(printMsg(QString)));
    connect(PluginLoader::getCopier(),SIGNAL(sendErrorMsg(QString)),this,SLOT(printErrorMsg(QString)));
    connect(PluginLoader::getCopier(),SIGNAL(copyFinished(bool)),this,SLOT(copyFinished(bool)));

}

PluginLoaderDialog::~PluginLoaderDialog()
{
    delete ui;
}

void PluginLoaderDialog::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();
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

        this->ui->lineEdit_Path->setText(filename);

        this->ui->lineEdit_Name->setText(PluginLoader::myMetaInfo->name);

        this->ui->lineEdit_pluginType->setText(PluginLoader::myMetaInfo->iid);

        this->ui->textBrowser_description->append("#######################");
        this->ui->textBrowser_description->append("name: " + PluginLoader::myMetaInfo->name);
        this->ui->textBrowser_description->append("author: " + PluginLoader::myMetaInfo->author);
        this->ui->textBrowser_description->append("#######################");
        this->ui->textBrowser_description->append("version: " + PluginLoader::myMetaInfo->pluginVersion);
        this->ui->textBrowser_description->append("#######################");
        this->ui->textBrowser_description->append("operating system: " + PluginLoader::myMetaInfo->operatingSystem);
        this->ui->textBrowser_description->append("compiler: " + PluginLoader::myMetaInfo->compiler);
        this->ui->textBrowser_description->append("#######################");

        if (PluginLoader::myMetaInfo->dependencies){
            this->ui->textBrowser_description->append("dependencies: " + PluginLoader::myMetaInfo->dependeciesPath.size());
            this->ui->progressBar->setMinimum(0);
            this->ui->progressBar->setMaximum(PluginLoader::myMetaInfo->dependeciesPath.size());
        }else{
            this->ui->textBrowser_description->append("dependencies: none");
        }

        for(int i = 0; i<PluginLoader::myMetaInfo->dependeciesPath.size();i++){

            QJsonObject v = PluginLoader::myMetaInfo->dependeciesPath.at(i).toObject();

            QString depName = v.value("name").toString() ;
            QString depPath = v.value("path").toString() ;

            QString tmpText(depName + ": " + depPath);

            this->ui->textBrowser_description->append(tmpText);

        }

        this->ui->textBrowser_description->append("#######################");
        this->ui->textBrowser_description->append("description: ");
        this->ui->textBrowser_description->append(PluginLoader::myMetaInfo->description);
        this->ui->textBrowser_description->append("#######################");


        QMessageBox::information(this,"valid", "plugin valid");
        this->pluginPath = filename;

        this->ui->progressBar->setMaximum(PluginLoader::myMetaInfo->dependeciesPath.size()+1);

        this->ui->pushButton_Ok->setEnabled(true);

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
    bool check = PluginLoader::copyPlugin(this->pluginPath);

}

void PluginLoaderDialog::printMsg(QString msg)
{

    this->ui->textBrowser_description->append(msg);
}

void PluginLoaderDialog::printErrorMsg(QString msg)
{
    this->ui->textBrowser_description->append(msg);
}

void PluginLoaderDialog::copyFinished(bool check)
{
    if (check){
        QMessageBox::information(this,"success", "plugin has been copied successfully.");
    }else{
        QMessageBox::critical(this,"error", "plugin was not copied successfully.");
    }
}

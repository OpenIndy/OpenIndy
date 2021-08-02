#include "showlicensesdialog.h"
#include "ui_showlicensesdialog.h"


class FileItem: public QStandardItem {       // The class
  public:
    FileItem(QString text, QString path): QStandardItem(text), path(path) {

    }

    QString path;
};

ShowLicensesDialog::ShowLicensesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowLicensesDialog)
{
    ui->setupUi(this);

    // parse licenses from file system
    QDir licensesDir(QCoreApplication::applicationDirPath() + "/licenses");

    QStringList oiComponents = licensesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::SortFlag::Name);
    foreach(QString oiComponent, oiComponents) {
        QStandardItem* oiComponentItem = new QStandardItem(oiComponent);
        this->model.appendRow(oiComponentItem);

        QDir moduleDir(licensesDir.absolutePath() + "/" + oiComponent);
        QStringList modules = moduleDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::SortFlag::Name);
        foreach(QString module, modules) {
            QStandardItem* moduleItem = new QStandardItem(module);
            oiComponentItem->appendRow(moduleItem);

            QDir licenseFilesDir(moduleDir.absolutePath() + "/" + module);
            QStringList licenseFiles = licenseFilesDir.entryList(QDir::Files, QDir::SortFlag::Name);
            foreach(QString licenseFile, licenseFiles) {
                FileItem* licenseFileItem = new FileItem(licenseFile, licenseFilesDir.absolutePath() + "/" + licenseFile);
                moduleItem->appendRow(licenseFileItem);
            }
        }

    }

    QStringList columnNames;
    columnNames.append("License");
    this->model.setHorizontalHeaderLabels(columnNames);
    this->ui->treeView->setModel(&this->model);

    connect(this->ui->treeView, &QTreeView::clicked, this, &ShowLicensesDialog::onClick);

}

void ShowLicensesDialog::onClick(const QModelIndex& idx) {
    FileItem* fileItem = dynamic_cast<FileItem*>(this->model.itemFromIndex(idx));
    if(fileItem != nullptr) {
        QFile file(fileItem->path);
        file.open(QFile::ReadOnly | QFile::Text);
        this->ui->plainTextEdit->setPlainText(file.readAll());
    } else {
        this->ui->plainTextEdit->setPlainText("");
    }
}

ShowLicensesDialog::~ShowLicensesDialog()
{
    delete ui;
}

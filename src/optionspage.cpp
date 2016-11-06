#include "optionspage.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

OptionsPage::OptionsPage(QWidget *parent) : 
    QWizardPage(parent)
{
    setSubTitle(tr("Задайте настройки"));
    
    QFormLayout *form = new QFormLayout();
    
    //    QComboBox *qlBox = new QComboBox();
    //    form->addRow(tr("Размер:"), qlBox);
    
    QHBoxLayout *outLayout = new QHBoxLayout();
    m_out = new QLineEdit();
    QPushButton *brzBtn = new QPushButton(tr("..."));
    brzBtn->setMaximumWidth(30);
    connect(brzBtn, SIGNAL(clicked()), this, SLOT(selectOutPath()));
    outLayout->addWidget(m_out);
    outLayout->addWidget(brzBtn);
    form->addRow(tr("Сохранить в:"), outLayout);
    
    setLayout(form);
}

OptionsPage::~OptionsPage()
{
    
}

void OptionsPage::init(const QStringList &photos)
{
    QString path = !photos.isEmpty() ? photos.first() : "";
    if (!path.isEmpty()) {
        QFileInfo fi(path);
        QString dir_name = fi.absoluteDir().dirName();
        QString out_path = fi.absolutePath() + "/../" + dir_name + "_bpg";
        out_path = QDir::cleanPath(out_path);
        m_out->setText(out_path);
    }
}

BpgConverter::Options OptionsPage::options() const
{
    BpgConverter::Options opt;
    
    opt.outPath = m_out->text();
    
    return opt;
}

void OptionsPage::selectOutPath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Select output directory"), "");
    m_out->setText(path);
}

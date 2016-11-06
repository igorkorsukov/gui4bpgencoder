#include "selectphotospage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <QFileDialog>

const int PATH_COL(0);

SelectPhotosPage::SelectPhotosPage(QWidget *parent) : 
    QWizardPage(parent)
{
    
    setSubTitle(tr("Выберите фотографии для конвертации"));
    
    
    m_table = new QTableWidget();
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->setColumnCount(1);
    m_table->setHorizontalHeaderItem(PATH_COL, new QTableWidgetItem(tr("Фотографии")));
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setTextElideMode(Qt::ElideMiddle);
    
    connect(m_table, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectedChanged()));
    
    QPushButton *addBtn = new QPushButton(tr("Добавить"));
    addBtn->setMaximumWidth(80);
    connect(addBtn, SIGNAL(clicked()), this, SLOT(addFiles()));
    
    m_removeBtn = new QPushButton(tr("Удалить"));
    m_removeBtn->setMaximumWidth(80);
    m_removeBtn->setEnabled(false);
    connect(m_removeBtn, SIGNAL(clicked()), this, SLOT(removeSelectedFiles()));
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(m_removeBtn);
    
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_table);
    layout->addLayout(btnLayout);
    
    setLayout(layout);
}

SelectPhotosPage::~SelectPhotosPage()
{
    
}

QStringList SelectPhotosPage::photoPathList() const
{
    QStringList ret;
    int fcount = m_table->rowCount();
    for (int i = 0; i < fcount; ++i) {
        ret << m_table->item(i, PATH_COL)->data(Qt::UserRole).toString();
    }
    return ret;
}

void SelectPhotosPage::addFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Выберите фотографии"), "", "Images (*.png *.jpg)");
    
    int fcount = files.count();
    int lastrow = m_table->rowCount();
    
    for (int i = 0; i < fcount; ++i) {
        int iindex = lastrow + i;
        QString fpath = files.at(i);

        m_table->insertRow(iindex);
        
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(fpath);
        item->setData(Qt::UserRole, fpath);
        m_table->setItem(iindex, PATH_COL, item);
    }
}

void SelectPhotosPage::removeSelectedFiles()
{
    QList<QTableWidgetItem *> items = m_table->selectedItems();
    for(int i = 0; i < items.count(); ++i) {
        int row = items.at(i)->row();
        m_table->removeRow(row);
        delete items.at(i);
    }
}

void SelectPhotosPage::onSelectedChanged()
{
    if (m_removeBtn) {
        m_removeBtn->setEnabled(!m_table->selectedItems().isEmpty());
    }
}

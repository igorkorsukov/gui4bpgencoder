#include "convertpage.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDebug>
#include <QAbstractButton>

const int TIME_COL(0);
const int PATH_COL(1);

ConvertPage::ConvertPage(QWidget *parent) : 
    QWizardPage(parent)
{
    m_table = new QTableWidget(this);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->setAlternatingRowColors(true);
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderItem(TIME_COL, new QTableWidgetItem(tr("Время")));
    m_table->setHorizontalHeaderItem(PATH_COL, new QTableWidgetItem(tr("Фотография")));
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setTextElideMode(Qt::ElideMiddle);
    
    
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_table);
    
    setLayout(layout);
    
    m_converter = new BpgConverter(this);
    connect(m_converter, SIGNAL(fileStarted(QString)), this, SLOT(onFileStarted(QString)));
    connect(m_converter, SIGNAL(fileProgress(QString,int)), this, SLOT(onFileProgress(QString,int)));
    connect(m_converter, SIGNAL(fileFinished(QString,QString,bool)), this, SLOT(onFileFinished(QString,QString,bool)));
    connect(m_converter, SIGNAL(allFinished()), this, SLOT(onAllFinished()));
}

ConvertPage::~ConvertPage()
{
    
}

void ConvertPage::init(const QStringList &photos, const BpgConverter::Options &opt)
{
    setSubTitle(tr("Готовы к конвертации"));
    
    QAbstractButton *finishBtn = wizard()->button(QWizard::FinishButton);
    if (finishBtn) {
        finishBtn->disconnect();
        setButtonText(QWizard::FinishButton, tr("Конвертировать"));
        connect(finishBtn, SIGNAL(clicked()), this, SLOT(convert()));
    }
    
    QAbstractButton *cancelBtn = wizard()->button(QWizard::CancelButton);
    if (cancelBtn) {
        cancelBtn->disconnect();
        setButtonText(QWizard::CancelButton, tr("Отмена"));
        connect(cancelBtn, SIGNAL(clicked()), this, SLOT(abort()));
    }
    
    
    m_photos = photos; 
    m_options = opt;
    
    m_table->clearContents();
    m_indexByPath.clear();
    
    m_table->setRowCount(photos.count());
    
    for (int i = 0; i < photos.count(); ++i) {
        QString fpath = photos.at(i);
        
        QTableWidgetItem *timeItem = new QTableWidgetItem();
        timeItem->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(i, TIME_COL, timeItem);
        
        QTableWidgetItem *pathItem = new QTableWidgetItem();
        pathItem->setText(fpath);
        m_table->setItem(i, PATH_COL, pathItem);
        
        m_indexByPath.insert(fpath, i);
    }
}

void ConvertPage::convert()
{
    setSubTitle(tr("Дождитесь конвертации"));
    
    wizard()->button(QWizard::BackButton)->setEnabled(false);
    wizard()->button(QWizard::FinishButton)->setEnabled(false);
    
    m_converter->convert(m_photos, m_options);
}

void ConvertPage::abort()
{
    m_converter->abort();
}

void ConvertPage::onFileStarted(const QString &in_path)
{
    QTableWidgetItem *timeItem = itemByPath(in_path, TIME_COL);
    Q_ASSERT(timeItem);
    if (timeItem) {
        timeItem->setText(formatTime(0));
    }
}

void ConvertPage::onFileProgress(const QString &in_path, int ms)
{
    QTableWidgetItem *timeItem = itemByPath(in_path, TIME_COL);
    Q_ASSERT(timeItem);
    if (timeItem) {
        timeItem->setText(formatTime(ms));
    }
}

void ConvertPage::onFileFinished(const QString &in_path, const QString &out_path, bool success)
{
    Q_UNUSED(out_path)
    QTableWidgetItem *timeItem = itemByPath(in_path, TIME_COL);
    Q_ASSERT(timeItem);
    if (timeItem) {
        timeItem->setTextColor(success ? QColor("#4E9A06") : QColor("#A40000"));
    }
}

void ConvertPage::onAllFinished()
{
    setSubTitle(tr("Закончено"));
    
    QAbstractButton *cancelBtn = wizard()->button(QWizard::CancelButton);
    if (cancelBtn) {
        cancelBtn->disconnect();
        setButtonText(QWizard::CancelButton, tr("Закрыть"));
        connect(cancelBtn, SIGNAL(clicked()), wizard(), SLOT(close()));
    }
}

QString ConvertPage::formatTime(int ms) const
{
    int s = ms / 1000;
    int m = s / 60;
    int h = m / 60;
    
    if (h > 0) {
        return formatVal(h) + ":" + formatVal(m - h * 60) + ":" + formatVal(s - m * 60); 
    } else {
        return formatVal(m - h * 60) + ":" + formatVal(s - m * 60); 
    }
}

QString ConvertPage::formatVal(int val) const
{
    return val < 10 ? ("0" + QString::number(val)) : QString::number(val);
}


QTableWidgetItem* ConvertPage::itemByPath(const QString &path, int col) const
{
    int index = m_indexByPath.value(path);
    QTableWidgetItem *item = m_table->item(index, col);
    return item;
}

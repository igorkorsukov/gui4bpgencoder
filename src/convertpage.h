#ifndef CONVERTPAGE_H
#define CONVERTPAGE_H

#include <QWizardPage>
#include <QStringList>
#include <QHash>
#include "bpgconverter.h"

class QTableWidget;
class QTableWidgetItem;
class ConvertPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ConvertPage(QWidget *parent = 0);
    ~ConvertPage();
    
    void init(const QStringList &photos, const BpgConverter::Options &opt);
    
    
private slots:
    void onFileStarted(const QString &in_path);
    void onFileProgress(const QString &in_path, int ms);
    void onFileFinished(const QString &in_path, const QString &out_path, bool success);
    void onAllFinished();
    
    void convert();
    void abort();
    
private:
    
    QTableWidget *m_table;
    QStringList m_photos; 
    BpgConverter::Options m_options;
    BpgConverter *m_converter;
    QHash<QString, int> m_indexByPath;
    
    QTableWidgetItem* itemByPath(const QString &path, int col) const;
    QString formatTime(int ms) const;
    QString formatVal(int val) const;
};

#endif // CONVERTPAGE_H

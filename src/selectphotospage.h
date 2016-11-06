#ifndef SELECTPHOTOSPAGE_H
#define SELECTPHOTOSPAGE_H

#include <QWizardPage>
#include <QStringList>

class QTableWidget;
class QPushButton;
class SelectPhotosPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit SelectPhotosPage(QWidget *parent = 0);
    ~SelectPhotosPage();
    
    QStringList photoPathList() const;
    
private slots:
    void addFiles();
    void removeSelectedFiles();
    void onSelectedChanged();
    
private:
    
    QTableWidget *m_table;
    QPushButton *m_removeBtn;
};

#endif // SELECTPHOTOSPAGE_H

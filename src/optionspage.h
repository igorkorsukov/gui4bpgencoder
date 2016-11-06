#ifndef OPTIONSPAGE_H
#define OPTIONSPAGE_H

#include <QWizardPage>
#include "bpgconverter.h"

class QLineEdit;
class OptionsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit OptionsPage(QWidget *parent = 0);
    ~OptionsPage();
    
    BpgConverter::Options options() const;
    
    void init(const QStringList &photos);
    
private slots:
    void selectOutPath();
    
private:
    
    QLineEdit *m_out;
};

#endif // OPTIONSPAGE_H

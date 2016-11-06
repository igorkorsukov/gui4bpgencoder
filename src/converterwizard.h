#ifndef CONVERTERWIZARD_H
#define CONVERTERWIZARD_H

#include <QWizard>
#include "selectphotospage.h"
#include "optionspage.h"
#include "convertpage.h"

class ConverterWizard : public QWizard
{
    Q_OBJECT
public:
    explicit ConverterWizard(QWidget *parent = 0);
    ~ConverterWizard();
    
private:
    
    enum Pages {
        SelectPg = 0,
        OptionsPg,
        ConvertPg
    };
    
    virtual void initializePage(int id);
    
    SelectPhotosPage *m_selectPage;
    OptionsPage *m_optionsPage;
    ConvertPage *m_convertPage;
};

#endif // CONVERTERWIZARD_H

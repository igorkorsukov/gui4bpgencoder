#include "converterwizard.h"

ConverterWizard::ConverterWizard(QWidget *parent) :
    QWizard(parent)
{
    setWizardStyle(QWizard::ClassicStyle);
    setWindowTitle(tr("Конвертор в формат BPG"));
    
    setButtonText(QWizard::BackButton, tr("Назад"));
    setButtonText(QWizard::NextButton, tr("Далее"));
    setButtonText(QWizard::CancelButton, tr("Отмена"));
    
    resize(800, 600);
    
    m_selectPage = new SelectPhotosPage(this);
    m_optionsPage = new OptionsPage(this);
    m_convertPage = new ConvertPage(this);
    
    setPage(SelectPg, m_selectPage);
    setPage(OptionsPg, m_optionsPage);
    setPage(ConvertPg, m_convertPage);
    
    setStartId(SelectPg);
}

ConverterWizard::~ConverterWizard()
{
    
}

void ConverterWizard::initializePage(int id)
{
    switch (id) {
    case SelectPg: {
        
    } break;
    case OptionsPg: {
        
        QStringList photos = m_selectPage->photoPathList();
        m_optionsPage->init(photos);
        
    } break;
    case ConvertPg: {
        
        QStringList photos = m_selectPage->photoPathList();
        BpgConverter::Options opt = m_optionsPage->options();
        m_convertPage->init(photos, opt);
        
    } break;
    default:
        break;
    }
}


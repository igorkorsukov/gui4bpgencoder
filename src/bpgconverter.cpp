#include "bpgconverter.h"
#include <QtConcurrent/QtConcurrent>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QTime>

BpgConverter::BpgConverter(QObject *parent) : 
    QObject(parent), m_isAborted(false), m_unfinishedCount(0)
{
    
}

BpgConverter::~BpgConverter()
{
    
}

void BpgConverter::abort()
{
    {
        QMutexLocker locker(&m_mutex);
        m_isAborted = true;
    }
    
    if (m_unfinishedCount > 0) {
        QEventLoop loop;
        connect(this, SIGNAL(allFinished()), &loop, SLOT(quit()));
        loop.exec();
    }
}

void BpgConverter::convert(const QStringList &photos, const BpgConverter::Options &opt)
{
    QDir().mkpath(opt.outPath);
    
    m_unfinishedCount = photos.count();
    
    foreach (const QString &in_path, photos) {
        QtConcurrent::run(this, &BpgConverter::thread_convert, in_path, opt);
    }
}

void BpgConverter::thread_convert(const QString &in_path, const BpgConverter::Options &opt)
{    
    bool ret = false;
    QFileInfo in_fi(in_path);
    QString out_fpath = opt.outPath + "/" + in_fi.baseName() + ".bpg";
    
    if (isAborted()) {
        goto Finished;
    }
    
    {
        QStringList arguments;
        arguments << "-o" << out_fpath << in_fi.absoluteFilePath();
        
        QProcess proc;
        proc.setEnvironment(QProcess::systemEnvironment());
        
        QTime time;
        time.start();
        proc.start("bpg/bpgenc.exe", arguments);
        
        QMetaObject::invokeMethod(this, "onProcStarted", Q_ARG(QString, in_path));
        
        proc.waitForStarted();
        
        if (isAborted()) {
            goto Finished;
        }
        
        while (proc.state() == QProcess::Running) {
            
            if (isAborted()) {
                proc.kill();
                proc.waitForFinished();
                QFile(out_fpath).remove();
                goto Finished;
            }
            
            wait(250);
            QMetaObject::invokeMethod(this, "onProcProgress", Q_ARG(QString, in_path), Q_ARG(int, time.elapsed()));
        }
        
        ret = true;
    }
    
Finished:    
    QMetaObject::invokeMethod(this, "onProcFinished", Q_ARG(QString, in_path), Q_ARG(QString, out_fpath), Q_ARG(bool, ret));
}

void BpgConverter::wait(int ms) const
{
    QTimer timer;
    timer.setInterval(ms);
    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start();
    loop.exec();
}

void BpgConverter::onProcStarted(const QString &in_path)
{
    emit fileStarted(in_path);
}

void BpgConverter::onProcProgress(const QString &in_path, int ms)
{
    emit fileProgress(in_path, ms); 
}

void BpgConverter::onProcFinished(const QString &in_path, const QString &out_path, bool success)
{
    emit fileFinished(in_path, out_path, success);
    
    --m_unfinishedCount;
    
    if (m_unfinishedCount == 0) {
        emit allFinished();
    }
}

bool BpgConverter::isAborted() const
{
    QMutexLocker locker(&m_mutex);
    return m_isAborted;
}

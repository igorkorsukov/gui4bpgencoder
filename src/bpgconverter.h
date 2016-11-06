#ifndef BPGCONVERTER_H
#define BPGCONVERTER_H

#include <QObject>
#include <QMutex>

class BpgConverter : public QObject
{
    Q_OBJECT
public:
    explicit BpgConverter(QObject *parent = 0);
    ~BpgConverter();
    
    struct Options {
        QString outPath;
    };
    
    bool isAborted() const;
    
public slots:
    void convert(const QStringList &photos, const BpgConverter::Options &opt);
    void abort();
    
signals:
    void fileStarted(const QString &in_path);
    void fileProgress(const QString &in_path, int ms);
    void fileFinished(const QString &in_path, const QString &out_path, bool success);
    void allFinished();
    
private slots:
    void onProcStarted(const QString &in_path);
    void onProcProgress(const QString &in_path, int ms);
    void onProcFinished(const QString &in_path, const QString &out_path, bool success);
    
private:
    void thread_convert(const QString &in_path, const BpgConverter::Options &opt);
    void wait(int ms) const;
    
    
    bool m_isAborted;
    mutable QMutex m_mutex;
    int m_unfinishedCount;
};

#endif // BPGCONVERTER_H

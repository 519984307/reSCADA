#ifndef PID_H
#define PID_H
#include <QVariant>
#include <QObject>
//#include <QDateTime>
#include <QTimer>
#include "SCADAenums.h"
#include <QDateTime>

class PID : public QObject
{
    Q_OBJECT
    //    Q_PROPERTY(double setpoint READ getSetpoint WRITE setSetpoint NOTIFY s_setpointChd)
    //    Q_PROPERTY(double Kp       READ getKp       WRITE setKp       NOTIFY s_KpChd)
    //    Q_PROPERTY(double Ki       READ getKi       WRITE setKi       NOTIFY s_KiChd)
    //    Q_PROPERTY(double Kd       READ getKd       WRITE setKd       NOTIFY s_KdChd)
    //    Q_PROPERTY(int    Td       READ getTd       WRITE setTd       NOTIFY s_TdChd)
    //    Q_PROPERTY(double minU     READ getMinU     WRITE setMinU     NOTIFY s_minUChd)
    //    Q_PROPERTY(double maxU     READ getMaxU     WRITE setMaxU     NOTIFY s_maxUChd)
    //    Q_PROPERTY(double u        READ getU                          NOTIFY s_uChd)
    //    Q_PROPERTY(bool started    READ isStarted                     NOTIFY s_started)
    //    Q_PROPERTY(bool inverse    READ getInverse  WRITE setInverse  NOTIFY s_inverseChd)
    //    Q_PROPERTY(double inputValue READ getInputValue  WRITE setInputValue  NOTIFY s_inputValueChd)

public:
    PID(QObject * parent,
        double Kp = 1,
        double Ki = 0,
        double Kd = 0,
        double minSat = 0,
        double maxSat = 0,
        unsigned int T = 1,
        QVariant * InputValue  = nullptr,
        std::function<QVariant()>InputValueFun = nullptr,
        std::function<void(QVariant)>UToTargetFun = nullptr
        );

    double getKp() const;
    void setKp(const double Kp);

    double getKi() const;
    void setKi(const double Ki);

    double getKd() const;
    void setKd(const double Kd);

    unsigned int getTd() const;
    void setTd( const unsigned int &Td );

    double getMinU() const;
    void setMinU(const double minU);

    double getMaxU() const;
    void setMaxU(const double maxU);

    double getSetpoint() const;

    double getU() const;
    void setU(const double u);

    bool isStarted(){  if(Td_timer == nullptr) return false;
        else return Td_timer->isActive(); };

    std::function<void (QVariant)> getUtoTargetFun() const;
    void setUtoTargetFun(const std::function<void (QVariant)> &setManParam);

    std::function<QVariant ()> getInputValueFun() const;
    void setInputValueFun(const std::function<QVariant ()> &InputValueFun);

    QVariant *getInputValuePtr_QVar() const;
    void setInptValuePtr_QVar(QVariant *InputValuePtr);

    bool getInverse() const;
    void setInverse(bool Inverse);

    double getInputValue() const;

signals:
    void s_uChdQVar( QVariant );
    void s_uChd( double );
    void s_error( QString );
    void s_started( bool );
    void s_logging(Prom::MessType MessTypeID,  QDateTime DateTime, bool UserOrSys, QString Source, QString Message);

    void s_setpointChd( double );
    void s_KpChd( double );
    void s_KiChd( double );
    void s_KdChd( double );
    void s_TdChd( int );
    void s_minUChd( double );
    void s_maxUChd( double );
    void s_inverseChd( bool );
    void s_inputValueChd( double );

public slots:
    void setInputValue_QVar(QVariant InputValue);
    void setInputValue(double InputValue);
    void setSetpoint(const double setpoint);
    virtual void start( int T );
    virtual void start( ){ start( m_Td ); };
    virtual void stop();

private slots:
    void nextStep();

private:
    QTimer * Td_timer{nullptr};
    double m_u{ 0 };
    double m_inputValue{ 0 };
    double * m_inputValuePtr{ nullptr };
    QVariant * m_inputValuePtr_QVar{ nullptr };
    std::function<QVariant()> m_inputValueFun{ nullptr };
    std::function<void(QVariant)> m_uToTargetFun{ nullptr };
    double m_prevIntegral{ 0 };
    double m_prevUErr{ 0 };
    double m_setpoint{ 0 };

    double m_Kp{ 0 };
    double m_Ki{ 0 };
    double m_Kd{ 0 };
    unsigned int m_Td{ 1 };
    double m_minU{ 0 };
    double m_maxU{ 0 };

    double uErr, P, I, D;

    bool _inverse = false;
};

#endif

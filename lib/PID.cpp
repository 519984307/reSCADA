#include <qsettings.h>
#include <qdebug.h>
#include "PID.h"


extern QSettings * g_unitINI;
//------------------------------------------------------------------------------
PID::PID(QObject * parent,
         double Kp,
         double Ki,
         double Kd,
         double minSat,
         double maxSat,
         unsigned int T,
         QVariant * InputValue /*значение*/,
         std::function<QVariant()>InputValueFun /*значение*/,
         std::function<void(QVariant)>UToTargetFun /*упраляемый параметр*/
         )
    : QObject(parent),
      m_inputValuePtr_QVar( InputValue ),
      m_inputValueFun( InputValueFun ),
      m_uToTargetFun( UToTargetFun ),
      m_Kp( Kp ),
      m_Ki( Ki ),
      m_Kd( Kd ),
      m_Td( T ),
      m_minU( minSat ),
      m_maxU( maxSat )
{

}

//------------------------------------------------------------------------------
void PID::nextStep()
{
    //   static double uErr, P, I, D;
    static bool ok;
    if( m_inputValueFun != nullptr ){
        try {
            m_inputValue = m_inputValueFun().toDouble();
        }  catch (...) {

        }
    }
    else if( m_inputValuePtr_QVar != nullptr ){
        uErr = m_inputValuePtr_QVar->toDouble( &ok );
        if( ok ) m_inputValue = uErr;
    }
    else if( m_inputValuePtr != nullptr ){
        m_inputValue = *m_inputValuePtr;
    }

    if(_inverse){
        uErr = m_inputValue - m_setpoint;
    }
    else{
        uErr = m_setpoint - m_inputValue;
    }

    P = m_Kp * uErr;
    I = m_prevIntegral + m_Ki  * m_Td * uErr;
    D = m_Kd * ( uErr - m_prevUErr );
    m_u = P + I + D;

    m_u = qMax( m_minU, m_u );
    m_u = qMin( m_maxU, m_u );
    m_prevIntegral = I;
    m_prevUErr = uErr;
    if( m_uToTargetFun !=nullptr ){
        try {
            m_uToTargetFun( m_u );
        }  catch (...) {

        }
    }
    m_u = qRound(m_u * 10) / 10;

    emit s_inputValueChd( m_inputValue );
    emit s_uChd( m_u );
    emit s_uChdQVar( QVariant( m_u ) );
    emit s_logging  ( Prom::MessInfo,  QDateTime::currentDateTime(), false,
                      objectName(), "P " + QString::number(P)
                      + ";I " + QString::number(I)
                      + ";D " + QString::number(D)
                      + ";Er " + QString::number(uErr)
                      + ";u " + QString::number(m_u));

}
//------------------------------------------------------------------------------
double PID::getInputValue() const
{
    return m_inputValue;
}
//------------------------------------------------------------------------------
void PID::setInverse(bool Inverse)
{
    _inverse = Inverse;
    emit s_inverseChd( _inverse );
}
//------------------------------------------------------------------------------
bool PID::getInverse() const
{
    return _inverse;
}
//------------------------------------------------------------------------------
QVariant *PID::getInputValuePtr_QVar() const
{
    return m_inputValuePtr_QVar;
}
//------------------------------------------------------------------------------
void PID::setInptValuePtr_QVar(QVariant *InputValuePtr_QVar)
{
    m_inputValuePtr_QVar = InputValuePtr_QVar;
    if( InputValuePtr_QVar != nullptr ){
        emit s_inputValueChd( InputValuePtr_QVar->toDouble() );
    }
}
//------------------------------------------------------------------------------
std::function<QVariant ()> PID::getInputValueFun() const
{
    return m_inputValueFun;
}
//------------------------------------------------------------------------------
void PID::setInputValueFun(const std::function<QVariant ()> &InputValueFun)
{
    m_inputValueFun = InputValueFun;
}
//------------------------------------------------------------------------------
std::function<void (QVariant)> PID::getUtoTargetFun() const
{
    return m_uToTargetFun;
}
//------------------------------------------------------------------------------
void PID::setUtoTargetFun(const std::function<void (QVariant)> &UToTargetFun)
{
    m_uToTargetFun = UToTargetFun;
}
//------------------------------------------------------------------------------
void PID::start( int T )
{
    delete Td_timer;
    Td_timer = new QTimer(this);
    disconnect( Td_timer, &QTimer::timeout, this, &PID::nextStep );
    connect( Td_timer, &QTimer::timeout, this, &PID::nextStep );
    Td_timer->start( T * 1000 );
    m_prevIntegral = m_prevIntegral * 0.9;
    emit s_started( true );
}

//------------------------------------------------------------------------------
void PID::stop()
{
    if( Td_timer ){
        Td_timer->stop();
    }
    delete Td_timer;
    Td_timer = nullptr;
    emit s_started( false );
}

//------------------------------------------------------------------------------
void PID::setInputValue_QVar(QVariant InputValue)
{
    m_inputValue = InputValue.toDouble();
    emit s_inputValueChd( m_inputValue );
}
//------------------------------------------------------------------------------
void PID::setInputValue(double InputValue)
{
    m_inputValue = InputValue;
    emit s_inputValueChd( m_inputValue );
}
//------------------------------------------------------------------------------
double PID::getU() const
{
    return m_u;
}
//------------------------------------------------------------------------------
void PID::setU(const double u)
{
    m_u = u;
    emit s_uChd( m_u );
}
//------------------------------------------------------------------------------
unsigned int PID::getTd() const
{
    return m_Td;
}
//------------------------------------------------------------------------------
void PID::setTd(const unsigned int &Td)
{
    m_Td = Td;
    emit s_TdChd( Td );
}
//------------------------------------------------------------------------------
double PID::getMinU() const
{
    return m_minU;
}
//------------------------------------------------------------------------------
void PID::setMinU(const double minU)
{
    m_minU = minU;
    emit s_minUChd( m_minU );
}
//------------------------------------------------------------------------------
double PID::getMaxU() const
{
    return m_maxU;
}
//------------------------------------------------------------------------------
void PID::setMaxU(const double maxU)
{
    m_maxU = maxU;
    emit s_maxUChd( m_maxU );
}
//------------------------------------------------------------------------------
double PID::getSetpoint() const
{
    return m_setpoint;
}
//------------------------------------------------------------------------------
void PID::setSetpoint(const double setpoint)
{
    m_setpoint = setpoint;
    emit s_setpointChd(m_setpoint);
}
//------------------------------------------------------------------------------
double PID::getKd() const
{
    return m_Kd;
}
//------------------------------------------------------------------------------
void PID::setKd(const double Kd)
{
    m_Kd = Kd;
    emit s_KdChd( Kd );
}
//------------------------------------------------------------------------------
double PID::getKi() const
{
    return m_Ki;
}
//------------------------------------------------------------------------------
void PID::setKi(const double Ki)
{
    m_Ki = Ki;
    emit s_KiChd( Ki );
}
//------------------------------------------------------------------------------
double PID::getKp() const
{
    return m_Kp;
}
//------------------------------------------------------------------------------
void PID::setKp(const double Kp)
{
    m_Kp = Kp;
    emit s_KpChd( Kp );
}
//------------------------------------------------------------------------------

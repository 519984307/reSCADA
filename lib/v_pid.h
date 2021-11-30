#ifndef __V_PID_H
#define __V_PID_H

#include <QtGlobal>

#include <v_cs_global.h>

class CS_LIBRARY_EXPORT VPid
{
public:
    VPid(qreal proportional, qreal integral, qreal differential,
         qreal period, qreal minSaturation, qreal maxSaturation);

    qreal getValue(qreal input);

private:
    qreal m_lastResult;
    qreal m_lastInput;

    qreal m_proportional;
    qreal m_integral;
    qreal m_differential;
    qreal m_period;
    qreal m_minSaturation;
    qreal m_maxSaturation;
};

#endif /* __V_PID_H */

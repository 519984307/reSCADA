#include "v_pid.h"

VPid::VPid(qreal proportional, qreal integral, qreal differential,
           qreal period, qreal minSaturation, qreal maxSaturation)
    : m_proportional(proportional),
      m_integral(integral),
      m_differential(differential),
      m_period(period),
      m_minSaturation(minSaturation),
      m_maxSaturation(maxSaturation)
{
    m_lastResult = m_lastInput = 0;
}

qreal VPid::getValue(qreal input)
{
    qreal sum = m_lastResult + m_integral * m_period * input;
    qreal diff = m_differential / m_period * (input - m_lastInput);
    qreal result = m_proportional * input + sum + diff;

    result = qMax(m_minSaturation, result);
    result = qMin(m_maxSaturation, result);

    m_lastResult = sum;
    m_lastInput = input;

    return result;
}

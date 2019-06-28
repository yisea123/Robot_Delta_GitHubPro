#include "BaseAction.h"

BaseAction::BaseAction()
{
    m_oldPoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    m_newPoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//    m_curPoint = DAxis(0.0, 0.0, 0.0, 0.0, 0.0);

    m_oldPosition = NAxis(0, 0, 0, 0, 0 , 0 );
    m_newPosition = NAxis(0, 0, 0, 0, 0 , 0 );
//    m_curPosition = NAxis(0, 0, 0, 0, 0);

    m_tool = NULL;
    m_nLineSerialNum = 0;
	m_delaytime=0;
}

void  BaseAction::addPointList(QVector<Point>&  pp)
{
}

DAxis BaseAction::nAxisToDAxis(NAxis naxis, SystemParameter* para)
{
    DAxis daxis;
    daxis.x = ((double)naxis.x) / para->m_axisParm[AXIS_X].m_nPulseEquivalent * para->m_axisParm[AXIS_X].m_nRatioDenominator / para->m_axisParm[AXIS_X].m_nRatioNumerator;
    daxis.y = ((double)naxis.y) / para->m_axisParm[AXIS_Y].m_nPulseEquivalent * para->m_axisParm[AXIS_Y].m_nRatioDenominator / para->m_axisParm[AXIS_Y].m_nRatioNumerator;
    daxis.z = ((double)naxis.z) / para->m_axisParm[AXIS_Z].m_nPulseEquivalent * para->m_axisParm[AXIS_Z].m_nRatioDenominator / para->m_axisParm[AXIS_Z].m_nRatioNumerator;
    daxis.w= ((double)naxis.w) / para->m_axisParm[AXIS_W].m_nPulseEquivalent * para->m_axisParm[AXIS_W].m_nRatioDenominator / para->m_axisParm[AXIS_W].m_nRatioNumerator;
    daxis.p= ((double)naxis.p) / para->m_axisParm[AXIS_P].m_nPulseEquivalent * para->m_axisParm[AXIS_P].m_nRatioDenominator / para->m_axisParm[AXIS_P].m_nRatioNumerator;
    daxis.r = ((double)naxis.r) / para->m_axisParm[AXIS_R].m_nPulseEquivalent * para->m_axisParm[AXIS_R].m_nRatioDenominator / para->m_axisParm[AXIS_R].m_nRatioNumerator;
    
    return daxis;
}

NAxis BaseAction::dAxisToNAxis(DAxis daxis, SystemParameter* para)
{
    NAxis naxis;
    naxis.x = (int)(daxis.x * para->m_axisParm[AXIS_X].m_nPulseEquivalent * para->m_axisParm[AXIS_X].m_nRatioNumerator / para->m_axisParm[AXIS_X].m_nRatioDenominator);
    naxis.y = (int)(daxis.y * para->m_axisParm[AXIS_Y].m_nPulseEquivalent * para->m_axisParm[AXIS_Y].m_nRatioNumerator / para->m_axisParm[AXIS_Y].m_nRatioDenominator);
    naxis.z = (int)(daxis.z * para->m_axisParm[AXIS_Z].m_nPulseEquivalent * para->m_axisParm[AXIS_Z].m_nRatioNumerator / para->m_axisParm[AXIS_Z].m_nRatioDenominator);
    naxis.w = (int)(daxis.w * para->m_axisParm[AXIS_W].m_nPulseEquivalent * para->m_axisParm[AXIS_W].m_nRatioNumerator / para->m_axisParm[AXIS_W].m_nRatioDenominator);
    naxis.p = (int)(daxis.p * para->m_axisParm[AXIS_P].m_nPulseEquivalent * para->m_axisParm[AXIS_P].m_nRatioNumerator / para->m_axisParm[AXIS_P].m_nRatioDenominator);
    naxis.r = (int)(daxis.r * para->m_axisParm[AXIS_R].m_nPulseEquivalent * para->m_axisParm[AXIS_R].m_nRatioNumerator / para->m_axisParm[AXIS_R].m_nRatioDenominator);
    
    return naxis;
}

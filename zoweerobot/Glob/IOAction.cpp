#include "IOAction.h"

IOAction::IOAction()
{
    m_type = ACTION_IO_TYPE;
    m_nIoType=IO_OUT;
    
    m_nIONum = -1;
    m_bIoSts = false;

    m_nCheckIONum = -1;

    m_bCheckIoSts=false;
    waitTime=1;
}

void  IOAction::addPointList(QVector<Point>&  pp)
{

}

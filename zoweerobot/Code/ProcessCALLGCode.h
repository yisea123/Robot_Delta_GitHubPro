/**
Copyright (c)
@file ProcessCALLGCode.h
@author zhou
@version 1.0
@date 2018/07/10
@brief ProcessCALLGCode���͵Ķ��塣
*/

#ifndef PROCESSCALLGCODE_H
#define PROCESSCALLGCODE_H

#include "ComonTypes.h"
#include "BaseGCode.h"

class  ParseGCodeInterVariableCache;


class ProcessCALLGCode : public BaseGCode
{
public:
    ProcessCALLGCode();

    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);

protected:
    int    m_callType;    /**< �����Ӻ�������   �硰CALL  80���е�80 */
};

#endif // PROCESSCALLGCODE_H

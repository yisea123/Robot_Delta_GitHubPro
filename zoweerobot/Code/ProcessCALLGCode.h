/**
Copyright (c)
@file ProcessCALLGCode.h
@author zhou
@version 1.0
@date 2018/07/10
@brief ProcessCALLGCode类型的定义。
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
    int    m_callType;    /**< 调用子函数类型   如“CALL  80”中的80 */
};

#endif // PROCESSCALLGCODE_H

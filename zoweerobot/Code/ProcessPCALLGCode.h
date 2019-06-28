/**
Copyright (c)
@file ProcessPCALLGCode.h
@author lin
@version 1.0
@date 2015-09-11
@brief ProcessPCALLGCode类型的定义。
*/

#ifndef PROCESSPCALLGCODE_H
#define PROCESSPCALLGCODE_H

#include "ProcessCALLGCode.h"

#define  PCALL_LOCAL_VARIABLE              26

#define  PCALL_R_PATH_LENGTH_OR_RADIUS     17     /**< R--17 */
#define  PCALL_Q_HORIZONTAL_ANGLE          16
#define  PCALL_N_PUNCH_NUM                 13
#define  PCALL_T_TOOL_TYPE                 19


class ProcessPCALLGCode : public ProcessCALLGCode
{
public:
    ProcessPCALLGCode();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);

private:
    bool fillLocalVariable(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool  matchLocalVariable(char c, int& t, ErrorInfo& e);

protected:
    int  m_localVariable[PCALL_LOCAL_VARIABLE];    /**< 局部变量P0-P25,对应A-Z */

};

#endif // PROCESSPCALLGCODE_H

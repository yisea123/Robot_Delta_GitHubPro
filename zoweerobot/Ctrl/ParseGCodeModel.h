/**
Copyright (c)
@file ParseGCode.h
@author zhou
@version 1.0
@date 2018/08/10
@brief ParseGCodeInterVariableCache���͵Ķ��塣
*/

#ifndef PARSEGCODE_H
#define PARSEGCODE_H

#include <QObject>
#include <QList>
#include "ComonTypes.h"
#include "GlobalVariableType.h"

class BaseModule;
class BaseGCode;
class ParseGCodeInterVariableCache;

class MacroGCode;


class ParseGCodeModel : public QObject
{
    Q_OBJECT
public:
    explicit ParseGCodeModel(QObject *parent = 0);

    static ParseGCodeModel* loadParseController(BaseModule *m, ErrorInfo& e);
    bool startupParseController(BaseModule *m, QVector<BaseGCode *>* list, ErrorInfo& e);
    bool startupParseController(SystemParameter* para, BaseModule *m, QVector<BaseGCode *>* list, ErrorInfo& e);
    bool unloadParseController(BaseModule *m, ErrorInfo& e);
    ParseGCodeInterVariableCache*  getInterVariableCache();
    QVector<HeadToolInfo>*   getToolsInfo();

private:
    bool  recordFileHeadInfo(ErrorInfo& e);
    bool  paserPerNGLine(ContextRecord s, QVector<BaseGCode *>* list, ErrorInfo& e);
    bool  converBaseGCode(GCodeParseInfo& inf, QVector<BaseGCode *>* list, ErrorInfo& e);
    bool  checkGCodeActionType(GCodeParseInfo& inf, GCodeActionType& t, ErrorInfo& e);
    bool  checkGCodeProcessType(GCodeParseInfo& inf, GCodeProcessType& t, ErrorInfo& e);
//    bool  findGCodeCharacterKeyWords(GCodeParseInfo& inf, QString s, int& val);
    bool  getKeySymbolWeight(LineContextInfo info, int& ii, ErrorInfo& e);
    bool  getGSymbolWeight(int v, int& ii, ErrorInfo& e);


protected:
    QList<ContextRecord>*  m_pfileContext;   /**< G����Դ�ļ�text */
    int              m_nContextReadLine;   /**< ���������к� */
    int              m_nContextLinesNum;   /**< m_pfileContext�������� */
    ParseGCodeInterVariableCache*  m_interVariableCache;   /**< �������̵���ʱ�洢�� */
    
    bool     m_bMacroFlag;
    MacroGCode*  m_pMacroGCode;

signals:
    
public slots:
    
};

#endif // PARSEGCODE_H

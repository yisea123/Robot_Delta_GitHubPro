/**
Copyright (c)
@file ActionPunchGCode.h
@author zhou
@version 1.0
@date 2018/07/10
@brief ActionPunchGCode类型的定义。
*/

#ifndef ACTIONPUNCHGCODE_H
#define ACTIONPUNCHGCODE_H

#include "ActionMoveGCode.h"

const QString  GCodePunchString[] ={"GCODE_PUNCH_NULL",
                                  "GCODE_G28",
                                  "GCODE_G38",
                                  "GCODE_G29",
                                  "GCODE_G39",
                                  "GCODE_G26",
                                  "GCODE_G36",
                                  "GCODE_G37",
                                  "GCODE_G66",
                                  "GCODE_G67",
                                  "GCODE_G68",
                                  "GCODE_G69",
                                  "GCODE_G78",
                                  "GCODE_G79",
                                  "GCODE_NOT_PUNCH",
                                  "GCODE_PUNCH_IO"};

class ActionPunchGCode : public ActionMoveGCode
{
public:
    ActionPunchGCode();
    bool  fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    virtual QString  toDebugString();

public:
    GCodePunchType     m_nPunchG;    /**< G代码类型：G28、G38、G29、G39、G26、G36、G37、
                                                G66、G67、G68、G69、G78、G79 */
private:

protected:
    bool toolAngleRotary(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double angle);


    int runG28(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, int nK);
    int runG38(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, int nK);
    int runG29(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP, int nK);
    int runG39(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP, int nK);
    int runG26(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, int nK);
    int runG36(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, int nP, double dJ, int nK);
    int runG37(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, int nP, double dJ, int nK);
    int runG66(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP, double dQ, double dD);//正方形模具时dQ省略，dD=0时省略
    int runG66(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP, double dK);//正方形模具时，新增
    int runG67(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP);
    int runG68(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dK, double dP, double dQ);
    int runG69(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP, double dQ);
    int runG78(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dK, double dP, double dQ, double dD);
    int runG79(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, double dI, double dJ, double dP, double dQ, double dD);

protected:
//    double             m_dI;    I值
//    double             m_dJ;    J值
    QString            m_sP;    /**< P值 */
    double             m_dQ;    /**< Q值 */
    double             m_dD;    /**< D值 */
    QString            m_sK;    /**< K值 */
    double             m_dI;    /**< I值 */
    double             m_dJ;    /**< J值 */
    int                m_nT;    /**< T值 */

    bool               m_isPunchKAssign;      /**< K是否赋值 */
    bool               m_isPunchPAssign;      /**< P是否赋值 */
    bool               m_isPunchQAssign;      /**< Q是否赋值 */
    bool               m_isPunchDAssign;      /**< D是否赋值 */
    bool               m_isPunchIAssign;      /**< I是否赋值 */
    bool               m_isPunchJAssign;      /**< J是否赋值 */
    bool               m_isPunchTAssign;      /**< T是否赋值 */

};

#endif // ACTIONPUNCHGCODE_H

/**
Copyright (c)
@file ActionMoveGCode.h
@author zhou
@version 1.0
@date 2018-07-10
@brief ActionMoveGCode类型的定义。
*/

#ifndef MOVEGCODE_H
#define MOVEGCODE_H

#include "ActionReferenceGCode.h"

const QString  GCodeMoveString[] ={"GCODE_MOVE_NULL",
                                  "GCODE_G00",
                                  "GCODE_G01",
                                  "GCODE_G02",
                                  "GCODE_G03",
                                  "GCODE_G04",
                                  "GCODE_G25",
                                  "GCODE_G27",
                                  "GCODE_G50",
                                  "GCODE_G70"};

class ActionMoveGCode : public ActionReferenceGCode
{
public:
    ActionMoveGCode();

    Point  getEndPosition();
    bool getMoveXAssign();
    bool getMoveYAssign();
    bool fillGCodeVarient(ParseGCodeInterVariableCache*  Cache, GCodeParseInfo& inf, ErrorInfo& e);
    bool genrateAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions,ErrorInfo& e);
    ToolInfo*  getToolInfoById(EmulationInterVariableCache* Cache, int id);
    ToolInfo*  getToolInfoById(EmulationInterVariableCache* Cache, int id, ErrorInfo& e);
    QString  toDebugString();

private:
    bool lineMove(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e);
    bool arcMove(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e);
    bool delayAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e);
    
protected:
    bool ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts);
    bool ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts, int checkIONum);
    bool ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, int ioNum, bool sts, int checkIONum, bool checkSts);
    bool ioAction(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, ErrorInfo& e);
    bool moveToPoint(EmulationInterVariableCache* Cache, QVector<BaseAction *>& actions, Point point, ErrorInfo& e);
    int User2Robot(IN  CartesianPose* i_CUser, OUT  CartesianPose* i_CPos, IN CartesianPose* i_UserTrans);
    int UserToRobot(double &x,   double &y, int userindex,ParseGCodeInterVariableCache*  Cache);
    int UserToRobot(double &x,   double &y, int userindex,EmulationInterVariableCache*  Cache);
    int runG25(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK,ErrorInfo& e);
    int runG27(EmulationInterVariableCache *Cache, QVector<BaseAction *> &actions, double dI, int nP, double dJ, int nK,ErrorInfo& e);


protected:
    GCodeMoveType      m_nMoveG;          /**< G代码类型：G00、G01、G02、G03、G04、G25、G27、G50、G70 */
    double             m_dX;          /**< X轴坐标值 */
    double             m_dY;          /**< Y轴坐标值 */
    double             m_dZ;          /**< Z轴坐标值 */
    double             m_dW;        /**< W轴坐标值 */
    double             m_dP;          /**< P轴坐标值 */
    double             m_dR;          /**< R轴坐标值 */
    double             m_dC;          /**< C轴坐标值 */
    double             m_dI;          /**< I值 */
    double             m_dJ;          /**< J值 */
    double             m_dF;          /**< 进给速度F */
    int                   m_dT;          /**< 工具号*/
    int                   m_dO;          /**< IO索引号*/
    int                   m_dB;          /**< IO值*/
    int                   m_dU;          /**< 用户坐标系号*/
//    int                   m_dP;          /**< X方向列数*/
    int                   m_dK;          /**< Y方向行数*/
    double                   m_dD;          /**< 延时超时时间s*/
    int                   m_dH;          /**scara手系0-all 1-left 2-right 3-change hand no warning*/
    Point              m_EndPoint;    /**< 终点坐标 */

//    GCodePosition  m_Position;  /**< 位置信息 */
    bool  m_isMoveXAssign;      /**< X位置是否赋值 */
    bool  m_isMoveYAssign;      /**< Y位置是否赋值 */
    bool  m_isMoveZAssign;      /**< Z位置是否赋值 */
    bool  m_isMoveWAssign;      /**< W位置是否赋值 */
    bool  m_isMovePAssign;      /**< P位置是否赋值 */
    bool  m_isMoveRAssign;      /**< R位置是否赋值 */
    bool  m_isMoveCAssign;      /**< C位置是否赋值 */
    bool  m_isMoveIAssign;      /**< I位置是否赋值 */
    bool  m_isMoveJAssign;      /**< J位置是否赋值 */
    bool  m_isMoveFAssign;      /**< F位置是否赋值 */
    bool  m_isMoveTAssign;      /**< T位置是否赋值 */
    bool  m_isMoveOAssign;      /**< O位置是否赋值 */
    bool  m_isMoveBAssign;      /**< B位置是否赋值 */
    bool  m_isMoveUAssign;      /**< U位置是否赋值 */
    bool  m_isMoveDAssign;      /**< D位置是否赋值 */
//    bool  m_isMovePAssign;      /**< U位置是否赋值 */
    bool  m_isMoveKAssign;      /**< K位置是否赋值 */
    bool  m_isMoveHAssign;      /**< H位置是否赋值 */
    ToolInfo* m_curToolInfo;
    ToolInfo* m_prevToolInfo;
};

#endif // MOVEGCODE_H

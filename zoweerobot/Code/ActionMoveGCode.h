/**
Copyright (c)
@file ActionMoveGCode.h
@author zhou
@version 1.0
@date 2018-07-10
@brief ActionMoveGCode���͵Ķ��塣
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
    GCodeMoveType      m_nMoveG;          /**< G�������ͣ�G00��G01��G02��G03��G04��G25��G27��G50��G70 */
    double             m_dX;          /**< X������ֵ */
    double             m_dY;          /**< Y������ֵ */
    double             m_dZ;          /**< Z������ֵ */
    double             m_dW;        /**< W������ֵ */
    double             m_dP;          /**< P������ֵ */
    double             m_dR;          /**< R������ֵ */
    double             m_dC;          /**< C������ֵ */
    double             m_dI;          /**< Iֵ */
    double             m_dJ;          /**< Jֵ */
    double             m_dF;          /**< �����ٶ�F */
    int                   m_dT;          /**< ���ߺ�*/
    int                   m_dO;          /**< IO������*/
    int                   m_dB;          /**< IOֵ*/
    int                   m_dU;          /**< �û�����ϵ��*/
    //int                   m_dP;          /**< X��������*/
    int                   m_dK;          /**< Y��������*/
    double                   m_dD;          /**< ��ʱ��ʱʱ��s*/
    int                   m_dH;          /**scara��ϵ0-all 1-left 2-right 3-change hand no warning*/
    Point              m_EndPoint;    /**< �յ����� */

//    GCodePosition  m_Position;  /**< λ����Ϣ */
    bool  m_isMoveXAssign;      /**< Xλ���Ƿ�ֵ */
    bool  m_isMoveYAssign;      /**< Yλ���Ƿ�ֵ */
    bool  m_isMoveZAssign;      /**< Zλ���Ƿ�ֵ */
    bool  m_isMoveWAssign;      /**< Wλ���Ƿ�ֵ */
    bool  m_isMovePAssign;      /**< Pλ���Ƿ�ֵ */
    bool  m_isMoveRAssign;      /**< Rλ���Ƿ�ֵ */
    bool  m_isMoveCAssign;      /**< Cλ���Ƿ�ֵ */
    bool  m_isMoveIAssign;      /**< Iλ���Ƿ�ֵ */
    bool  m_isMoveJAssign;      /**< Jλ���Ƿ�ֵ */
    bool  m_isMoveFAssign;      /**< Fλ���Ƿ�ֵ */
    bool  m_isMoveTAssign;      /**< Tλ���Ƿ�ֵ */
    bool  m_isMoveOAssign;      /**< Oλ���Ƿ�ֵ */
    bool  m_isMoveBAssign;      /**< Bλ���Ƿ�ֵ */
    bool  m_isMoveUAssign;      /**< Uλ���Ƿ�ֵ */
    bool  m_isMoveDAssign;      /**< Dλ���Ƿ�ֵ */
    //bool  m_isMovePAssign;      /**< Uλ���Ƿ�ֵ */
    bool  m_isMoveKAssign;      /**< Kλ���Ƿ�ֵ */
    bool  m_isMoveHAssign;      /**< Hλ���Ƿ�ֵ */
    ToolInfo* m_curToolInfo;
    ToolInfo* m_prevToolInfo;
};

#endif // MOVEGCODE_H

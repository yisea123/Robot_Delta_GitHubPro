#ifndef EMULATIONCONTROLLERDEVICE_H
#define EMULATIONCONTROLLERDEVICE_H

#include "BaseDevice.h"
#include "cemulationcanvs.h"
#include "GlobalVariableType.h"
#include "BaseAction.h"
#include "MoveAction.h"
#include "PunchAction.h"
#include "ToolChangeAction.h"
#include "ToolRotateAction.h"
#include "ToolInfo.h"

#define   CIRCLE_SEGMENTATION_NUM    40
#define   TEXT_OFFSET_BASEPOS        20
#define   CARRIER_RADIUS_SIZE        20
#define   CARRIER_CROSS_SIZE         40

class CMachiningSimulationUi;

class EmulationControllerDevice : public BaseDevice
{
    Q_OBJECT
public:
    EmulationControllerDevice();
    EmulationControllerDevice(QString s, BaseModule* m);

    virtual int startDevice(ErrorInfo& e);
    virtual int restartDevice(ErrorInfo& e);
    virtual int pauseDevice(ErrorInfo& e);
    virtual int openDevice(ErrorInfo& e);
    virtual int stopDevice(ErrorInfo& e);
    void setCanvsHandle(CEmulationCanvs* c);
    void clearCanvsData();
    void removeCarrierGraphical();
    int drawBackground(Material mater,QVector<BaseAction *>*  actions, EmulationInterVariableCache* cache, ErrorInfo& e);
    int drawDesk(Material mater, EmulationInterVariableCache* cache);
    int drawDesk(Material mater, EmulationInterVariableCache* cache, QVector<BaseAction *>*  actions);

//    bool drawActions(QVector<BaseAction *>*  actions, ErrorInfo& e);
    bool drawOneActions(BaseAction *  action, ErrorInfo& e);
    bool drawOneActionsWithNoCarrier(BaseAction *  action, ErrorInfo& e);
    bool drawBackgroundActions(QVector<BaseAction *>*  actions, ErrorInfo& e);
    GraphicalInfo* drawMoveAction(MoveAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e);
    GraphicalInfo* drawPunchAction(PunchAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e);
    GraphicalInfo* drawToolChangeAction(ToolChangeAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e);
    GraphicalInfo* drawToolRotateAction(ToolRotateAction *a, Qt::PenStyle s, QColor c, ErrorInfo& e);

    CMachiningSimulationUi* getSimulationUiHandle();
    void setSimulationStateUi(CMachiningSimulationUi* ui);

protected:
    int initDevice(ErrorInfo& e);
    void drawCarrierGraphical(ToolInfo* tool, Point p, double r, double len, double a);
    void drawCarrierGraphical(BaseAction *act);
    void drawCarrierGraphical(ToolInfo* tool, Point p, double a);
    void drawCarrierGraphical(Point p);
    GraphicalInfo* drawCircle(Point p, double r);
    GraphicalInfo* drawHorizontalCross(Point p, double len);
    GraphicalInfo* drawVerticalCross(Point p, double len);
    GraphicalInfo* drawTool(ToolInfo* tool, Point p, double a);

public  slots:


protected:
    CEmulationCanvs*        m_canvs;
    CMachiningSimulationUi* m_pSimulationUI;


};

#endif // EMULATIONCONTROLLERDEVICE_H

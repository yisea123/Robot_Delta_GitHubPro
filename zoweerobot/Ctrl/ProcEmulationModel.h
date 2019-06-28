#ifndef PROCEMULATIONMODEL_H
#define PROCEMULATIONMODEL_H

//#include "ParseGCodeModel.h"
#include "GlobalVariableType.h"
#include "BaseDevice.h"
#include "EmulationControllerDevice.h"
#include "BaseAction.h"
#include "MoveAction.h"
#include "PunchAction.h"
#include "MGAction.h"
#include "IOAction.h"
#include "DelayAction.h"
#include "ToolInfo.h"
//#include "UI/cprogramemulation.h"
//#include "cmachiningsimulationui.h"
#include "cautomachiningstateui.h"

#include <QTimer>
#include <QThread>

class  ParseGCodeModel;
class  BaseGCode;

enum EmulateMotionResult
{
    EMULATE_FINISH,
    EMULATE_PAUSE,
    EMULATE_PAUSE_BRAEKPOINT,
    EMULATE_RUNNING
};

class ProcEmulationModel : public QObject
{
public:
    ProcEmulationModel();

    static ProcEmulationModel* loadProcEmulationController(BaseModule *m, ErrorInfo& e);
    bool startupProcEmulationController(BaseModule *m, QVector<BaseGCode *>* list, ErrorInfo& e);
    bool startupProcEmulationController(BaseModule *m, QVector<BaseGCode *>* list, ProcessInterVariableCache* cache, ErrorInfo& e);
    bool unloadProcEmulationController(BaseModule *m, ErrorInfo& e);
    bool clearActions();
    //EmulateMotionResult emulateMotion(CMachiningSimulationUi *ui, ErrorInfo& e);
    EmulateMotionResult emulateMotion(CAutoMachiningStateUi* ui, ErrorInfo& e);
    bool stepEmulateMotion(int& next, ErrorInfo& e);
    void restartemulateMotion(ErrorInfo& e);

    void setBreakpointLine(int nBreakpointLine);
    void closeBreakpoint();
    QVector<BaseAction *>* getBaseActionHandle();

    EmulationInterVariableCache* getEmulationInterVariableCacheHandle();

    EmulationInterVariableCache*  m_EmulationCache;

    void initActionRunNum();
    Material getRawMaterial();
private:
    bool initToolInfo(ErrorInfo& e);
    bool initToolInfo(SystemParameter* para, ErrorInfo& e);

private:
    ParseGCodeModel*  m_pParseModel;

    QVector<BaseAction *>     m_actions;
    EmulationControllerDevice *m_pDevice;
    BaseModule *m_pModule;

    int     m_nActionRunNum;

    int     m_nBreakpointLine;
    bool    m_bIsBreakPointRun;

    bool    m_bIsEmulationCacheNew;
public slots:


};

#endif // PROCEMULATIONMODEL_H

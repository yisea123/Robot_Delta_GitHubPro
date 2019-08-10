/************************************************************
  Copyright (C),
  �ļ�����ProcEmulationModel.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ProcEmulationModel���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/


#include "ProcEmulationModel.h"
#include "BaseModule.h"
//#include "EmulationControllerModule.h"
#include "SystemSchedule.h"
#include "BaseDevice.h"
#include "EmulationControllerDevice.h"
#include "BaseGCode.h"
#include "ActionReferenceGCode.h"
#include "ActionMoveGCode.h"
#include "ActionPunchGCode.h"
#include "CoordinateGcode.h"
#include "ParseGCodeModel.h"


/*************************************************
  �������ƣ�ProcEmulationModel()
  �������ܣ�ProcEmulationModel�Ĺ��캯��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
ProcEmulationModel::ProcEmulationModel()
{
    m_nBreakpointLine = -2;
    m_bIsBreakPointRun = false;
    m_bIsEmulationCacheNew = false;
}

/*************************************************
  �������ƣ�loadProcEmulationController(BaseModule *m, ErrorInfo& e)
  �������ܣ�����EmulationController������G�����ļ�����
  ���������  m  Moduleָ��
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
ProcEmulationModel* ProcEmulationModel::loadProcEmulationController(BaseModule *m, ErrorInfo& e)
{   
    ProcEmulationModel* model = new ProcEmulationModel;

    model->m_pParseModel = ParseGCodeModel::loadParseController(m, e);

    if(model->m_pParseModel != NULL)
    {
        return model;
    }
    else
    {
        delete model;
        return NULL;
    }

}

/*************************************************
  �������ƣ�startupProcEmulationController(BaseModule *m, QList<BaseGCode *>* list, ErrorInfo& e)
  �������ܣ���������
  ���������  m  Moduleָ��
  ���������  list  �������ŵ���������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::startupProcEmulationController(BaseModule *m, QVector<BaseGCode *>* list, ErrorInfo& e)//no one calls
{
    if(m_pParseModel->startupParseController(m->getSystemScheduleHandle()->getSystemParameterHandler(), m, list, e))
    {
        m_pModule = m;
        m_pDevice = ((EmulationControllerDevice *)(m->getDevice()));
        m_pDevice->restartDevice(e);

        m_EmulationCache = new EmulationInterVariableCache;
        if (NULL == m_EmulationCache)
        {
            e.insertDebug(DEBUG_CREAT_ERROR, "m_EmulationCache new error");
            return false;
        }
        m_bIsEmulationCacheNew = true;
        m_EmulationCache->m_MachineCoordinate.x = -m_pModule->getSystemScheduleHandle()->getSystemParameterHandler()->m_machineParm.m_machineOriginalPosition.x;
        m_EmulationCache->m_MachineCoordinate.y = -m_pModule->getSystemScheduleHandle()->getSystemParameterHandler()->m_machineParm.m_machineOriginalPosition.y;

	for(int i=1;i<21;i++)
	{
		m_EmulationCache->m_userParam[i]=m->getSystemScheduleHandle()->getSystemParameterHandler()->m_userParam[i];
	}
        //(EmulationControllerDevice *)m_pDevice->set
        //if(initToolInfo(e))
        if(initToolInfo(m_pModule->getSystemScheduleHandle()->getSystemParameterHandler(), e))
        {

            for(int i = 0; i < list->size(); i++)
            {

                if(!(list->at(i)->genrateAction(m_EmulationCache, m_actions, e)))
                {
                    return false;
                }
            }

            m_pModule->getSystemScheduleHandle()->getSystemParameterHandler()->setMachineCoordinate(m_EmulationCache->m_MachineCoordinate);//���ù�������ϵ���е����ϵ�Ķ�Ӧ��ϵ
            m_pDevice->drawBackground(m_pParseModel->getInterVariableCache()->m_RawMaterial, &m_actions, m_EmulationCache, e);
            m_nActionRunNum = 0;
            m_nBreakpointLine = -1;

//            for (int mm = 0; mm < m_actions.size(); mm++)
//            {
//                //qDebug()<<mm<<" action"<<m_actions.at(mm)->m_type<<" n"<<m_actions.at(mm)->m_nLineSerialNum;
//                switch (m_actions.at(mm)->m_type)
//                {
//                case ACTION_PUNCH_TYPE:
//                case ACTION_TOOL_CHANGE_TYPE:
//                case ACTION_TOOL_ROTATE_TYPE:
//                case ACTION_MOVE_TYPE:
//                    qDebug("action =%d, type=%d, ex=%g, ey=%g, c=%g", mm, m_actions.at(mm)->m_type, ((MoveAction *)m_actions.at(mm))->m_basePoint.x, ((MoveAction *)m_actions.at(mm))->m_basePoint.y, ((MoveAction *)m_actions.at(mm))->m_dAngle);
//                    break;
//                default:
//                    qDebug("action =%d, type=%d, line=%d, x=%g�� y=%g", mm, m_actions.at(mm)->m_type, m_actions.at(mm)->m_nLineSerialNum, m_actions.at(mm)->m_basePoint.x, m_actions.at(mm)->m_basePoint.y);
//                    break;
//                }
//            }
        }
        else
        {
            return  DONE_FAIL;
        }

        return  DONE_SUCCESS;
    }
    else
    {
        return  DONE_FAIL;
    }
}

/*************************************************
  �������ƣ�startupProcEmulationController(BaseModule *m, QVector<BaseGCode *>* list, ProcessInterVariableCache* cache, ErrorInfo& e)
  �������ܣ���������
  ���������  m  Moduleָ��
  ���������  list  �������ŵ���������
            cache  ��������еı�������
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::startupProcEmulationController(BaseModule *m, QVector<BaseGCode *>* list, ProcessInterVariableCache* cache, ErrorInfo& e)
{
    SystemParameter* para=m->getSystemScheduleHandle()->getSystemParameterHandler();
    if(m_pParseModel->startupParseController(m->getSystemScheduleHandle()->getSystemParameterHandler(), m, list, e))
    {
    	 qDebug("startupParseController end");
        m_pModule = m;
        m_pDevice = ((EmulationControllerDevice *)(m->getDevice()));
        m_pDevice->restartDevice(e);
	
        m_EmulationCache = new EmulationInterVariableCache;
        if (NULL == m_EmulationCache)
        {
            e.insertDebug(DEBUG_CREAT_ERROR, "m_EmulationCache new error");
            return false;
        }
        m_bIsEmulationCacheNew = true;
        m_EmulationCache->m_MachineCoordinate.x = -m_pModule->getSystemScheduleHandle()->getSystemParameterHandler()->m_machineParm.m_machineOriginalPosition.x;
        m_EmulationCache->m_MachineCoordinate.y = -m_pModule->getSystemScheduleHandle()->getSystemParameterHandler()->m_machineParm.m_machineOriginalPosition.y;

        m_EmulationCache->m_LastPosition.x = cache->m_curPoint.x;//��¼��һ���ļ�����ĵ�ǰ��λ��
        m_EmulationCache->m_LastPosition.y = cache->m_curPoint.y;
	 m_EmulationCache->m_LastPosition.z = cache->m_curPoint.z;//��¼��һ���ļ�����ĵ�ǰ��λ��
        m_EmulationCache->m_LastPosition.w = cache->m_curPoint.w;
	 m_EmulationCache->m_LastPosition.p = cache->m_curPoint.p;//��¼��һ���ļ�����ĵ�ǰ��λ��
        m_EmulationCache->m_LastPosition.r = cache->m_curPoint.r;
        m_EmulationCache->m_dToolAngle = cache->m_curPoint.z;
        m_EmulationCache->m_dModulesAngle = cache->m_curPoint.r;
        m_EmulationCache->m_nToolNum = cache->m_nCurToolNum;
        cache->m_newPoint = cache->m_curPoint;
        cache->m_newPosition = cache->m_curPosition;

	for(int i=1;i<21;i++)
	{
		m_EmulationCache->m_userParam[i]=para->m_userParam[i];
	}
	
        qDebug("\n new satrt: x=%g, y=%g, c=%g, tool=%d", m_EmulationCache->m_LastPosition.x, m_EmulationCache->m_LastPosition.y, m_EmulationCache->m_dToolAngle, m_EmulationCache->m_nToolNum);

        //if(initToolInfo(e))
        if(initToolInfo(m_pModule->getSystemScheduleHandle()->getSystemParameterHandler(), e))
        {

            for(int i = 0; i < list->size(); i++)
            {

                if(!(list->at(i)->genrateAction(m_EmulationCache, m_actions, e)))
                {
                    return false;
                }
            }
            m_pModule->getSystemScheduleHandle()->getSystemParameterHandler()->setMachineCoordinate(m_EmulationCache->m_MachineCoordinate);//���ù�������ϵ���е����ϵ�Ķ�Ӧ��ϵ
            m_pDevice->drawBackground(m_pParseModel->getInterVariableCache()->m_RawMaterial, &m_actions, m_EmulationCache, e);
            m_nActionRunNum = 0;
            m_nBreakpointLine = -1;

            /*Point temP = Point(0,0);
            Point delP;
            for (int mm = 0; mm < m_actions.size(); mm++)
            {
                delP = ((MoveAction *)m_actions.at(mm))->m_endPoint - temP;
                qDebug("action Type(v)=%d, x=%g, y=%g, c=%g", m_actions.at(mm)->m_type, delP.x, delP.y, ((MoveAction *)m_actions.at(mm))->m_dAngle);
                temP = ((MoveAction *)m_actions.at(mm))->m_endPoint;
            }*/
            qDebug()<<"action size="<<m_actions.size();
            for (int mm = 0; mm < m_actions.size(); mm++)
            {
                //qDebug()<<mm<<" action"<<m_actions.at(mm)->m_type<<" n"<<m_actions.at(mm)->m_nLineSerialNum;
                switch (m_actions.at(mm)->m_type)
                {
                case ACTION_PUNCH_TYPE:
                        qDebug("action =%d, n=%d, v=%d", mm, m_actions.at(mm)->m_type, ((PunchAction *)m_actions.at(mm))->m_nPunchType);
                        break;
                case ACTION_MOVE_TYPE:
                case ACTION_TOOL_ROTATE_TYPE:
                case ACTION_TOOL_CHANGE_TYPE:
                    qDebug("action =%d, n=%d, ex=%g, ey=%g", mm, m_actions.at(mm)->m_type, ((MoveAction *)m_actions.at(mm))->m_endPoint.x, ((MoveAction *)m_actions.at(mm))->m_endPoint.y);
                    break;
                default:
                    qDebug("action =%d, n=%d", mm, m_actions.at(mm)->m_type);
                    break;
                }
            }
        }
        else
        {
            return  DONE_FAIL;
        }

        return  DONE_SUCCESS;
    }
    else
    {
        return  DONE_FAIL;
    }
}

/*************************************************
  �������ƣ�emulateMotion(CMachiningSimulationUi* ui, ErrorInfo& e)
  �������ܣ����з��溯�����ϲ�ӿڣ���ʱ���ж��е���
  ���������ui �������UI��ָ��
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ�����浱ǰ״̬
  ��    ע��
**************************************************/
/*EmulateMotionResult ProcEmulationModel::emulateMotion(CMachiningSimulationUi* ui, ErrorInfo& e)
{
    if(m_nActionRunNum < m_actions.size())
    {
        m_pDevice->drawOneActions(m_actions[m_nActionRunNum], e);

        if (m_actions[m_nActionRunNum]->m_type == ACTION_PUNCH_TYPE || m_actions[m_nActionRunNum]->m_type == ACTION_MOVE_TYPE)
        {
            DAxis curP;
            curP.x = ((MoveAction *)m_actions[m_nActionRunNum])->m_endPoint.x;
            curP.y = ((MoveAction *)m_actions[m_nActionRunNum])->m_endPoint.y;
            curP.t = 0;
            curP.c = ((MoveAction *)m_actions[m_nActionRunNum])->m_dAngle;
            curP.v = 0;
            m_pDevice->getSimulationUiHandle()->setPostext(curP);
            m_pDevice->getSimulationUiHandle()->setToolText(m_actions[m_nActionRunNum]->m_tool->m_name, m_actions[m_nActionRunNum]->m_tool->getSerialNum());
        }

        if(ui != NULL)
        {
            ui->setProcessBarValue((m_nActionRunNum + 1) * 100 / m_actions.size());
            ui->setCurrentNGCodeLine(m_actions[m_nActionRunNum]->m_nLineSerialNum);
        }
        m_nActionRunNum++;

        if(m_nActionRunNum < m_actions.size())
        {
            if (m_actions[m_nActionRunNum]->m_nLineSerialNum == m_nBreakpointLine + 1)
            {
                m_nBreakpointLine = -2;
                return  EMULATE_PAUSE_BRAEKPOINT;
            }
        }


        return  EMULATE_RUNNING;
    }
    else
    {
        return EMULATE_FINISH;
    }

}*/

/*************************************************
  �������ƣ�emulateMotion(CAutoMachiningStateUi* ui, ErrorInfo& e)
  �������ܣ����з��溯�����ϲ�ӿڣ���ʱ���ж��е���
  ���������ui �����ӹ�����UI��ָ��
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ��
  ��    ע��
**************************************************/
EmulateMotionResult ProcEmulationModel::emulateMotion(CAutoMachiningStateUi* ui, ErrorInfo& e)
{
    if(m_nActionRunNum < m_actions.size())
    {
        //if (m_actions[m_nActionRunNum]->m_nLineSerialNum == m_nBreakpointLine+1)
        if (m_bIsBreakPointRun)
        {
            //if (m_actions[m_nActionRunNum]->m_nLineSerialNum > m_nBreakpointLine)
            if (m_actions[m_nActionRunNum]->m_nLineSerialNum == m_nBreakpointLine+1)
            {
                m_nBreakpointLine = -1;
                return  EMULATE_PAUSE_BRAEKPOINT;
            }
        }

        m_pDevice->drawOneActions(m_actions[m_nActionRunNum], e);
        if(ui != NULL)
        {
            ui->setProcessBarValue((m_nActionRunNum + 1) * 100 / m_actions.size());
            ui->setCurrentNGCodeLine(m_actions[m_nActionRunNum]->m_nLineSerialNum);
        }
        m_nActionRunNum++;

        return  EMULATE_RUNNING;
    }
    else
    {
        m_bIsBreakPointRun = false;
        return EMULATE_FINISH;
    }

}

/*************************************************
  �������ƣ�stepEmulateMotion(int& next, ErrorInfo &e)
  �������ܣ���������
  ���������
  ���������  next ��¼G�����к�
            e  Module�Ĵ�����Ϣ
  �� �� ֵ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::stepEmulateMotion(int& next, ErrorInfo &e)
{
    if(m_nActionRunNum < m_actions.size())
    {
        int nCurrentLineSerialNum = m_actions[m_nActionRunNum]->m_nLineSerialNum;
        int nn = m_nActionRunNum;
        while (nn < m_actions.size() &&
               nCurrentLineSerialNum == m_actions[nn]->m_nLineSerialNum)
        {
            nn++;
        }

        if(nn < m_actions.size())
        {
            next = m_actions[nn]->m_nLineSerialNum - 1;
            return true;
        }
        else
        {
            next = nn;//-1;
            return true;
        }
    }
    else
    {
        return false;
    }
}

/*************************************************
  �������ƣ�restartemulateMotion(ErrorInfo& e)
  �������ܣ����¿�ʼ��������
  ���������
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ��
  ��    ע��
**************************************************/
void ProcEmulationModel::restartemulateMotion(ErrorInfo& e)
{
    m_pDevice->clearCanvsData();
    m_pDevice->removeCarrierGraphical();
    m_pDevice->drawBackground(m_pParseModel->getInterVariableCache()->m_RawMaterial, &m_actions, m_EmulationCache, e);
    m_nActionRunNum = 0;
}

/*************************************************
  �������ƣ�setBreakpointLine(int nBreakpointLine)
  �������ܣ����öϵ�
  ���������nBreakpointLine  �ϵ��к�
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void ProcEmulationModel::setBreakpointLine(int nBreakpointLine)
{
    m_nBreakpointLine = nBreakpointLine;
    m_bIsBreakPointRun = true;
}

/*************************************************
  �������ƣ�closeBreakpoint()
  �������ܣ��رնϵ����й���
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void ProcEmulationModel::closeBreakpoint()
{
    m_bIsBreakPointRun = false;
}

/*************************************************
  �������ƣ�getBaseActionHandle()
  �������ܣ����BaseAction�б�
  ���������
  ���������
  �� �� ֵ��BaseAction�б�ָ��
  ��    ע��
**************************************************/
QVector<BaseAction *>* ProcEmulationModel::getBaseActionHandle()
{
    return &m_actions;
}

/*************************************************
  �������ƣ�unloadProcEmulationController(BaseModule *m, ErrorInfo& e)
  �������ܣ�ж�ط���Controller
  ���������  m  Moduleָ��
  ���������  e  Module�Ĵ�����Ϣ
  �� �� ֵ��  DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::unloadProcEmulationController(BaseModule *m, ErrorInfo& e)
{
    m_pParseModel->unloadParseController(m, e);
    if (m_bIsEmulationCacheNew)
    {
        delete  m_EmulationCache;
        m_bIsEmulationCacheNew = false;
    }
    clearActions();
    delete this;
  return 0;
}

/*************************************************
  �������ƣ�initToolInfo(ErrorInfo& e)
  �������ܣ���ʼ��������Ϣ
  ���������
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::initToolInfo(ErrorInfo& e)
{
    QVector<HeadToolInfo>* info = m_pParseModel->getToolsInfo();
    for(int i = 0; i < info->size(); i++)
    {
        ToolInfo* tool = new ToolInfo;
        tool->setName(info->at(i).name);
        tool->setSerialNum(info->at(i).toolSerialNum);
        if(tool->m_name == "SQ")
        {
            if(info->at(i).paraRecordNum >= 1)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[0]);
                tool->breakupGraph();
                tool->setToolType("SQ");
                if(info->at(i).paraRecordNum == 2)
                {
                    tool->setAngle(info->at(i).para[1]);
                }
                m_EmulationCache->m_ToolsInfo.append(tool);


            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel SQ tool no para!");
                return DONE_FAIL;
            }
        }
        else if(tool->m_name == "RE")
        {
            if(info->at(i).paraRecordNum >= 2)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                if(info->at(i).paraRecordNum == 3)
                {
                    tool->setAngle(info->at(i).para[2]);
                }
                tool->setToolType("RE");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);


            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel RE tool no para!");
                return DONE_FAIL;
            }
        }
        else if(tool->m_name == "RO")
        {
            if(info->at(i).paraRecordNum >= 1)
            {
                tool->setRadius(info->at(i).para[0]);
                tool->breakupGraph();
                tool->setToolType("RO");
                m_EmulationCache->m_ToolsInfo.append(tool);


            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel RO tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "OB")
        {
            if(info->at(i).paraRecordNum >= 2)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                if(info->at(i).paraRecordNum == 3)
                {
                    tool->setAngle(info->at(i).para[2]);
                }
                tool->setToolType("OB");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);


            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel OB tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "SD")
        {
            if(info->at(i).paraRecordNum >= 2)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                if(info->at(i).paraRecordNum == 3)
                {
                    tool->setAngle(info->at(i).para[2]);
                }
                tool->setToolType("SD");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);


            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel SD tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "WD")
        {
            if(info->at(i).paraRecordNum >= 2)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                if(info->at(i).paraRecordNum == 3)
                {
                    tool->setAngle(info->at(i).para[2]);
                }
                tool->setToolType("WD");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);


            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel WD tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "TR")
        {
            if(info->at(i).paraRecordNum >= 3)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                tool->setTrC(info->at(i).para[2]);
                if(info->at(i).paraRecordNum == 4)
                {
                    tool->setAngle(info->at(i).para[3]);
                }
                tool->setToolType("TR");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);

            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel TR tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "MJ")
        {
            if(info->at(i).paraRecordNum >= 3)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                tool->setMJD(info->at(i).para[2]);
                if(info->at(i).paraRecordNum == 4)
                {
                    tool->setAngle(info->at(i).para[3]);
                }
                tool->setToolType("MJ");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);

            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel MJ tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "RR")
        {
            if(info->at(i).paraRecordNum >= 3)
            {
                tool->settoolXY(info->at(i).para[0], info->at(i).para[1]);
                tool->setRadius(info->at(i).para[2]);
                if(info->at(i).paraRecordNum == 4)
                {
                    tool->setAngle(info->at(i).para[3]);
                }
                tool->setToolType("RR");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);

            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel RR tool no para!");
                return DONE_FAIL;
            }
        }
        else if (tool->m_name == "CR")
        {
            if(info->at(i).paraRecordNum >= 5)
            {
                tool->settoolXY(info->at(i).para[1], info->at(i).para[2]);
                tool->setCRR(info->at(i).para[0]);
                tool->setCRA(info->at(i).para[3]);
                tool->setCRB(info->at(i).para[4]);
                if(info->at(i).paraRecordNum == 6)
                {
                    tool->setAngle(info->at(i).para[5]);
                }
                tool->setToolType("CR");

                tool->breakupGraph();
                m_EmulationCache->m_ToolsInfo.append(tool);

            }
            else
            {
                delete tool;
                e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel CR tool no para!");
                return DONE_FAIL;
            }
        }
//        else
//        {
//            delete tool;
//            //e.insertDebug(DEBUG_CREAT_ERROR, "ProcEmulationModel RE tool no para!");
//            return DONE_FAIL;
//        }
    }
    return DONE_SUCCESS;
}

/*************************************************
  �������ƣ�initToolInfo(SystemParameter* para, ErrorInfo& e)
  �������ܣ���ʼ��������Ϣ
  ���������para ϵͳ����
  ��������� e  Module�Ĵ�����Ϣ
  �� �� ֵ�� DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::initToolInfo(SystemParameter* para, ErrorInfo& e)
{
    QVector<MouldsParameter>* mouldsParm = &para->m_mouldsParm;
    for(int i = 0; i < mouldsParm->size(); i++)
    {
        ToolInfo* tool = new ToolInfo;
        tool->setName(mouldsParm->at(i).m_sType);
        tool->setToolType(mouldsParm->at(i).m_sType);
        tool->setFrameNum(mouldsParm->at(i).m_nFrameNum);
        tool->setSerialNum(mouldsParm->at(i).m_nNum);

        tool->settoolXY(mouldsParm->at(i).m_dLength, mouldsParm->at(i).m_dWidth);
        tool->setRadius(mouldsParm->at(i).m_dDiameter / 2.0);
        tool->setAngle(mouldsParm->at(i).m_dAngle);
        tool->setCRR(mouldsParm->at(i).m_dR);
        tool->setCRA(mouldsParm->at(i).m_dA);
        tool->setCRB(mouldsParm->at(i).m_dB);
        tool->setTrC(mouldsParm->at(i).m_dC);
        tool->setMJD(mouldsParm->at(i).m_dD);
        tool->setRotaryFlag(mouldsParm->at(i).m_bIsRotary);

        tool->setPosAngle(mouldsParm->at(i).m_dPosAngle);

        tool->breakupGraph();
        m_EmulationCache->m_ToolsInfo.append(tool);

        if (mouldsParm->at(i).m_nFrameNum == 1)
        {
            m_EmulationCache->m_nToolNum = mouldsParm->at(i).m_nNum;
            m_EmulationCache->m_dToolAngle = mouldsParm->at(i).m_dAngle;
        }
    }

    return DONE_SUCCESS;
}

/*************************************************
  �������ƣ�clearActions()
  �������ܣ����BaseAction�б�
  ���������
  ���������
  �� �� ֵ��DONE_SUCCESS �ɹ�
            DONE_FAIL  ʧ��
  ��    ע��
**************************************************/
bool ProcEmulationModel::clearActions()
{
    for(int i = 0; i < m_actions.size(); i++)
    {
        delete m_actions.at(i);
    }
    m_actions.clear();
    return 0;
}

/*************************************************
  �������ƣ�getEmulationInterVariableCacheHandle()
  �������ܣ���÷�������Actions�б���̵ı�������
  ���������
  ���������
  �� �� ֵ�� ��������Actions�б���̵ı�������ָ��
  ��    ע��
**************************************************/
EmulationInterVariableCache* ProcEmulationModel::getEmulationInterVariableCacheHandle()
{
    return m_EmulationCache;
}

/*************************************************
  �������ƣ�initActionRunNum()
  �������ܣ���ʼ�����л���
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void ProcEmulationModel::initActionRunNum()
{
    m_nActionRunNum = 0;
}

/*************************************************
  �������ƣ�getRawMaterial()
  �������ܣ���ȡ�ӹ������ļ�ͷ����¼�ļӹ����ϳߴ����Ϣ
  ���������
  ���������
  �� �� ֵ���ӹ������ļ�ͷ����¼�ļӹ����ϳߴ����Ϣ
  ��    ע��
**************************************************/
Material ProcEmulationModel::getRawMaterial()
{
    return m_pParseModel->getInterVariableCache()->m_RawMaterial;
}










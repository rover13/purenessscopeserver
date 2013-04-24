// DlgServerConnect.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgServerConnect.h"


// CDlgServerConnect dialog

IMPLEMENT_DYNAMIC(CDlgServerConnect, CDialog)

CDlgServerConnect::CDlgServerConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerConnect::IDD, pParent)
{

}

CDlgServerConnect::~CDlgServerConnect()
{
}

void CDlgServerConnect::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST1, m_lcServerConnect);
}


BEGIN_MESSAGE_MAP(CDlgServerConnect, CDialog)
  ON_BN_CLICKED(IDC_BUTTON2, &CDlgServerConnect::OnBnClickedButton2)
END_MESSAGE_MAP()

CString CDlgServerConnect::GetPageTitle()
{
  return _T("��������������Ϣ");
}


// CDlgServerConnect message handlers

void CDlgServerConnect::OnBnClickedButton2()
{
  //�鿴TCP
  m_lcServerConnect.DeleteAllItems();

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "ServerConnectTCP -a");
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[100 * 1024] = {'\0'};
  int nRecvLen = 100 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    vecClientConnectInfo objvecClientConnectInfo;

    int nStrLen       = 0;
    int nPos          = 4;
    int nTCPCount     = 0;
    memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    for(int i = 0; i < nTCPCount; i++)
    {
      //��ʼ��ԭ���ݽṹ
      _ClientConnectInfo ClientConnectInfo;

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(ClientConnectInfo.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      ClientConnectInfo.m_szIP[nStrLen] = '\0';

      memcpy_s(&ClientConnectInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nRecvCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nSendCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nAllRecvSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nAllSendSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nAliveTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nRecvQueueCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(int), &szRecvBuff[nPos], sizeof(long long));
      nPos += sizeof(long long);

      memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(int), &szRecvBuff[nPos], sizeof(long long));
      nPos += sizeof(long long);

      //��ʾ�ڽ�����
      wchar_t szzTCPIP[50] = {'\0'};
      CString strConnectID;
      CString strSendPacketCount;
      CString strRecvPacketCount;
      CString strSendSize;
      CString strRecvSize;
      CString strCreateTime;
      CString strAliveSecond;
      CString strLogicCount;

      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, szzTCPIP, 50);

      strConnectID.Format(_T("%d"), ClientConnectInfo.m_nConnectID);
      strSendPacketCount.Format(_T("%d"), ClientConnectInfo.m_nSendCount);
      strRecvPacketCount.Format(_T("%d"), ClientConnectInfo.m_nRecvCount);
      strSendSize.Format(_T("%d"), ClientConnectInfo.m_nAllSendSize);
      strRecvSize.Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize);
      strCreateTime.Format(_T("%d"), ClientConnectInfo.m_nBeginTime);
      strAliveSecond.Format(_T("%d"), ClientConnectInfo.m_nAliveTime);
      strLogicCount.Format(_T("%d"), ClientConnectInfo.m_nRecvQueueCount);

      m_lcServerConnect.InsertItem(i, szzTCPIP);
      m_lcServerConnect.SetItemText(i, 1, strConnectID);
      m_lcServerConnect.SetItemText(i, 2, strSendPacketCount);
      m_lcServerConnect.SetItemText(i, 3, strRecvPacketCount);
      m_lcServerConnect.SetItemText(i, 4, strSendSize);
      m_lcServerConnect.SetItemText(i, 5, strRecvSize);
      m_lcServerConnect.SetItemText(i, 6, strCreateTime);
      m_lcServerConnect.SetItemText(i, 7, strAliveSecond);

      objvecClientConnectInfo.push_back(ClientConnectInfo);
    }
  }
}

void CDlgServerConnect::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
  m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgServerConnect::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_lcServerConnect.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 100);
  m_lcServerConnect.InsertColumn(1, _T("IP��ַ"), LVCFMT_CENTER, 100);
  m_lcServerConnect.InsertColumn(2, _T("���Ͱ���"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(3, _T("���հ���"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(4, _T("�����ֽ�"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(5, _T("�����ֽ�"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(6, _T("����ʱ��"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(7, _T("�������"), LVCFMT_CENTER, 80);

  return TRUE;
}
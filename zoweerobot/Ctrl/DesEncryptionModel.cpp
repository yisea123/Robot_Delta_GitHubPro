/************************************************************
  Copyright (C),
  �ļ�����ParseGCode.cpp
  ���ߣ�zhou    �汾��V1.0    �������ڣ�2018/07/10
  ģ��������ParseGCode���ʵ��
  ��ʷ�޸ļ�¼��<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      ���ע��
*************************************************************/

#include "DesEncryptionModel.h"
#include "memory.h"

/*************************************************
  �������ƣ�DesEncryptionModel()
  �������ܣ�DesEncryptionModel�Ĺ��캯��
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
DesEncryptionModel::DesEncryptionModel()
{
    char key[8]={1,9,8,0,9,1,7};
    Des_SetKey(key);
}

/*************************************************
  �������ƣ�Des_Run(QString In, bool Type)
  �������ܣ����ܻ��������
  ���������In ԭʼ�������ܺ������
          Type ��Ҫ���м��ܻ���ܵĲ�����־
  ���������
  �� �� ֵ�� ���ܺ���ַ��� �� ���ܺ������
  ��    ע��
**************************************************/
QString DesEncryptionModel::Des_Run(QString In, bool Type)
{
    //char Out[In.size()];
    char* Out=new char[In.size()];
    static bool M[64], Tmp[32], *Li = &M[0], *Ri = &M[32];

    QByteArray ba = In.toLatin1();
    const char *cstr = ba.data();

    ByteToBit(M, cstr, 64);
    Transform(M, M, IP_Table, 64);
    if( Type == ENCRYPT ){
        for(int i=0; i<16; i++) {
            memcpy(Tmp, Ri, 32);
            F_func(Ri, SubKey[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, Tmp, 32);
        }
    }else{
        for(int i=15; i>=0; i--) {
            memcpy(Tmp, Li, 32);
            F_func(Li, SubKey[i]);
            Xor(Li, Ri, 32);
            memcpy(Ri, Tmp, 32);
        }
    }
    Transform(M, M, IPR_Table, 64);

    BitToByte(Out, M, 64);
    return QString(QLatin1String( Out));
}

/*************************************************
  �������ƣ�Des_Run(char* Out, char* In, bool Type)
  �������ܣ����ܻ��������
  ���������In ԭʼ�������ܺ������
          Type ��Ҫ���м��ܻ���ܵĲ�����־
  ��������� Out ���ܺ���ַ��� �� ���ܺ������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::Des_Run(char* Out, char* In, bool Type)
{
    static bool M[64], Tmp[32], *Li = &M[0], *Ri = &M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP_Table, 64);
    if( Type == ENCRYPT ){
        for(int i=0; i<16; i++) {
            memcpy(Tmp, Ri, 32);
            F_func(Ri, SubKey[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, Tmp, 32);
        }
    }else{
        for(int i=15; i>=0; i--) {
            memcpy(Tmp, Li, 32);
            F_func(Li, SubKey[i]);
            Xor(Li, Ri, 32);
            memcpy(Ri, Tmp, 32);
        }
    }
    Transform(M, M, IPR_Table, 64);
    BitToByte(Out, M, 64);
}

/*************************************************
  �������ƣ�Des_SetKey(const char Key[8])
  �������ܣ����ü�����Կ
  ���������Key[8]  ��Կ
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::Des_SetKey(const char Key[8])
{
    static bool K[64], *KL = &K[0], *KR = &K[28];
    ByteToBit(K, Key, 64);
    Transform(K, K, PC1_Table, 56);
    for(int i=0; i<16; i++) {
        RotateL(KL, 28, LOOP_Table[i]);
        RotateL(KR, 28, LOOP_Table[i]);
        Transform(SubKey[i], K, PC2_Table, 48);
    }
}

/*************************************************
  �������ƣ�F_func(bool In[32], const bool Ki[48])
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::F_func(bool In[32], const bool Ki[48])
{
    static bool MR[48];
    Transform(MR, In, E_Table, 48);
    Xor(MR, Ki, 48);
    S_func(In, MR);
    Transform(In, In, P_Table, 32);
}

/*************************************************
  �������ƣ�S_func(bool Out[32], const bool In[48])
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::S_func(bool Out[32], const bool In[48])
{
    for(char i=0,j,k; i<8; i++,In+=6,Out+=4) {
        j = (In[0]<<1) + In[5];
        k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
        ByteToBit(Out, &S_Box[i][j][k], 4);
    }
}

/*************************************************
  �������ƣ�Transform(bool *Out, bool *In, const char *Table, int len)
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::Transform(bool *Out, bool *In, const char *Table, int len)
{
    static bool Tmp[256];
    for(int i=0; i<len; i++)
        Tmp[i] = In[ Table[i]-1 ];
    memcpy(Out, Tmp, len);
}

/*************************************************
  �������ƣ�Xor(bool *InA, const bool *InB, int len)
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::Xor(bool *InA, const bool *InB, int len)
{
    for(int i=0; i<len; i++)
        InA[i] ^= InB[i];
}

/*************************************************
  �������ƣ�RotateL(bool *In, int len, int loop)
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::RotateL(bool *In, int len, int loop)
{
    static bool Tmp[256];
    memcpy(Tmp, In, loop);
    memcpy(In, In+loop, len-loop);
    memcpy(In+len-loop, Tmp, loop);
}

/*************************************************
  �������ƣ�ByteToBit(bool *Out, const char *In, int bits)
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::ByteToBit(bool *Out, const char *In, int bits)
{
    for(int i=0; i<bits; i++)
        Out[i] = (In[i/8]>>(i%8)) & 1;
}

/*************************************************
  �������ƣ�BitToByte(char *Out, const bool *In, int bits)
  �������ܣ����ܻ���ܹ��̱���������
  ���������
  ���������
  �� �� ֵ��
  ��    ע��
**************************************************/
void DesEncryptionModel::BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, (bits+7)/8);
    for(int i=0; i<bits; i++)
        Out[i/8] |= In[i]<<(i%8);
}



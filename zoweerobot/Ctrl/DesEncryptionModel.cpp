/************************************************************
  Copyright (C),
  文件名：ParseGCode.cpp
  作者：zhou    版本：V1.0    创建日期：2018/07/10
  模块描述：ParseGCode类的实现
  历史修改记录：<author>    <time>    <version >    <desc>
                  zhou    2018/07/10     V1.0      添加注释
*************************************************************/

#include "DesEncryptionModel.h"
#include "memory.h"

/*************************************************
  函数名称：DesEncryptionModel()
  函数功能：DesEncryptionModel的构造函数
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
DesEncryptionModel::DesEncryptionModel()
{
    char key[8]={1,9,8,0,9,1,7};
    Des_SetKey(key);
}

/*************************************************
  函数名称：Des_Run(QString In, bool Type)
  函数功能：加密或解密运算
  输入参数：In 原始密码或加密后的密码
          Type 需要进行加密或解密的操作标志
  输出参数：
  返 回 值： 加密后的字符串 或 解密后的密码
  备    注：
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
  函数名称：Des_Run(char* Out, char* In, bool Type)
  函数功能：加密或解密运算
  输入参数：In 原始密码或加密后的密码
          Type 需要进行加密或解密的操作标志
  输出参数： Out 加密后的字符串 或 解密后的密码
  返 回 值：
  备    注：
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
  函数名称：Des_SetKey(const char Key[8])
  函数功能：设置加密秘钥
  输入参数：Key[8]  秘钥
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：F_func(bool In[32], const bool Ki[48])
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：S_func(bool Out[32], const bool In[48])
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
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
  函数名称：Transform(bool *Out, bool *In, const char *Table, int len)
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void DesEncryptionModel::Transform(bool *Out, bool *In, const char *Table, int len)
{
    static bool Tmp[256];
    for(int i=0; i<len; i++)
        Tmp[i] = In[ Table[i]-1 ];
    memcpy(Out, Tmp, len);
}

/*************************************************
  函数名称：Xor(bool *InA, const bool *InB, int len)
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void DesEncryptionModel::Xor(bool *InA, const bool *InB, int len)
{
    for(int i=0; i<len; i++)
        InA[i] ^= InB[i];
}

/*************************************************
  函数名称：RotateL(bool *In, int len, int loop)
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void DesEncryptionModel::RotateL(bool *In, int len, int loop)
{
    static bool Tmp[256];
    memcpy(Tmp, In, loop);
    memcpy(In, In+loop, len-loop);
    memcpy(In+len-loop, Tmp, loop);
}

/*************************************************
  函数名称：ByteToBit(bool *Out, const char *In, int bits)
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void DesEncryptionModel::ByteToBit(bool *Out, const char *In, int bits)
{
    for(int i=0; i<bits; i++)
        Out[i] = (In[i/8]>>(i%8)) & 1;
}

/*************************************************
  函数名称：BitToByte(char *Out, const bool *In, int bits)
  函数功能：加密或解密过程变量的运算
  输入参数：
  输出参数：
  返 回 值：
  备    注：
**************************************************/
void DesEncryptionModel::BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, (bits+7)/8);
    for(int i=0; i<bits; i++)
        Out[i/8] |= In[i]<<(i%8);
}



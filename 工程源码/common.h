#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QTableWidget>
//#include <QMessageBox>
//#include <QDebug>

class mysqlObj
{
private:
    QString iniFilePath;//数据库配置文件路径
    QString hostName;//数据库host
    int port;//数据库端口号
    QString databaseName;//数据库名称
    QString userName;//数据库用户名
    QString password;//数据库密码
    QSqlDatabase db;//数据库操作对象
    bool readConfigFile();//读取数据库配置文件
    bool addDBDriver();//加载数据库驱动
    bool setDB();//设置数据库配置信息
    bool connectDB();//连接数据库
public:
    QString localIP;//连接数据库的客户端本地IP
    bool initSuccess;//数据库是否初始化成功标志位
    mysqlObj();//构造函数(执行初始化操作)
    ~mysqlObj();//析构函数(释放资源)
    QSqlQuery query(QString sql,bool *result);//数据库查询
};

mysqlObj* getMysqlObj();//返回操作数据库的mysqlObj对象的指针
void delMysqlObj();//删除释放mysqlObj对象

class loginUserInfoObj
{
public:
    int id;
    QString name;
    QString password;
    int role;
    int last_login_time;
    QString last_login_ip;
    loginUserInfoObj();//构造函数
    ~loginUserInfoObj();//析构函数
};

loginUserInfoObj* getLoginUserInfoObj();//返回操作当前登录用户的loginUserInfoObj对象的指针
void delLoginUserInfoObj();//删除释放loginUserInfoObj对象

bool insert_log(QString content);//用于向数据库中插入日志
QString cutQString(QString in,int max_length);//用于截断字符串(可以针对于本地编码，避免直接截取后，最后一位出现乱码)
QString calMD5(QString str);//计算一个QString的MD5值，返回对应的QString

QList<int> getSelectedRowsIndex(QTableWidget* tableWidget);//返回一个多选QTableWidget控件选中的行所对应的index列表

//AES加解密部分(模式:ECB;部位方式:PKCS5) start
#define BLOCK_SIZE 16

class CRijndael
{
public:
    //Operation Modes
    //The Electronic Code Book (ECB), Cipher Block Chaining (CBC) and Cipher Feedback Block (CFB) modes
    //are implemented.
    //In ECB mode if the same block is encrypted twice with the same key, the resulting
    //ciphertext blocks are the same.
    //In CBC Mode a ciphertext block is obtained by first xoring the
    //plaintext block with the previous ciphertext block, and encrypting the resulting value.
    //In CFB mode a ciphertext block is obtained by encrypting the previous ciphertext block
    //and xoring the resulting value with the plaintext.
    enum { ECB=0, CBC=1, CFB=2 };

private:
    enum { DEFAULT_BLOCK_SIZE=16 };
    enum { MAX_BLOCK_SIZE=32, MAX_ROUNDS=14, MAX_KC=8, MAX_BC=8 };

    //Auxiliary Functions
    //Multiply two elements of GF(2^m)
    static int Mul(int a, int b)
    {
        return (a != 0 && b != 0) ? sm_alog[(sm_log[a & 0xFF] + sm_log[b & 0xFF]) % 255] : 0;
    }

    //Convenience method used in generating Transposition Boxes
    static int Mul4(int a, char b[])
    {
        if(a == 0)
            return 0;
        a = sm_log[a & 0xFF];
        int a0 = (b[0] != 0) ? sm_alog[(a + sm_log[b[0] & 0xFF]) % 255] & 0xFF : 0;
        int a1 = (b[1] != 0) ? sm_alog[(a + sm_log[b[1] & 0xFF]) % 255] & 0xFF : 0;
        int a2 = (b[2] != 0) ? sm_alog[(a + sm_log[b[2] & 0xFF]) % 255] & 0xFF : 0;
        int a3 = (b[3] != 0) ? sm_alog[(a + sm_log[b[3] & 0xFF]) % 255] & 0xFF : 0;
        return a0 << 24 | a1 << 16 | a2 << 8 | a3;
    }

public:
    //CONSTRUCTOR
    CRijndael();

    //DESTRUCTOR
    virtual ~CRijndael();

    //Expand a user-supplied key material into a session key.
    // key        - The 128/192/256-bit user-key to use.
    // chain      - initial chain block for CBC and CFB modes.
    // keylength  - 16, 24 or 32 bytes
    // blockSize  - The block size in bytes of this Rijndael (16, 24 or 32 bytes).
    void MakeKey(char const* key, char const* chain, int keylength=DEFAULT_BLOCK_SIZE, int blockSize=DEFAULT_BLOCK_SIZE);

private:
    //Auxiliary Function
    void Xor(char* buff, char const* chain)
    {
        if(false==m_bKeyInit)
             throw new QString(sm_szErrorMsg1);
        for(int i=0; i<m_blockSize; i++)
            *(buff++) ^= *(chain++);
    }

    //Convenience method to encrypt exactly one block of plaintext, assuming
    //Rijndael's default block size (128-bit).
    // in         - The plaintext
    // result     - The ciphertext generated from a plaintext using the key
    void DefEncryptBlock(char const* in, char* result);

    //Convenience method to decrypt exactly one block of plaintext, assuming
    //Rijndael's default block size (128-bit).
    // in         - The ciphertext.
    // result     - The plaintext generated from a ciphertext using the session key.
    void DefDecryptBlock(char const* in, char* result);

public:
    //Encrypt exactly one block of plaintext.
    // in           - The plaintext.
    // result       - The ciphertext generated from a plaintext using the key.
    void EncryptBlock(char const* in, char* result);

    //Decrypt exactly one block of ciphertext.
    // in         - The ciphertext.
    // result     - The plaintext generated from a ciphertext using the session key.
    void DecryptBlock(char const* in, char* result);

    void Encrypt(char const* in, char* result, size_t n, int iMode=ECB);

    void Decrypt(char const* in, char* result, size_t n, int iMode=ECB);

    //Get Key Length
    int GetKeyLength()
    {
        if(false==m_bKeyInit)
            throw new QString(sm_szErrorMsg1);
        return m_keylength;
    }

    //Block Size
    int	GetBlockSize()
    {
        if(false==m_bKeyInit)
            throw new QString(sm_szErrorMsg1);
        return m_blockSize;
    }

    //Number of Rounds
    int GetRounds()
    {
        if(false==m_bKeyInit)
            throw new QString(sm_szErrorMsg1);
        return m_iROUNDS;
    }

    void ResetChain()
    {
        memcpy(m_chain, m_chain0, m_blockSize);
    }

public:
    //Null chain
    static char const* sm_chain0;
public:
    //十六进制的字符串和char字符串的转化
    static void PaddingData2Block(std::string& str,char* szDataIn);
    static void Char2Hex(unsigned char ch, char* szHex);
    static void Hex2Char(char const* szHex, unsigned char& rch);
    static void CharStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize);
    static void HexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize);
    static void EncryptString(std::string& str,std::string& result);
    //初始化KEY和IV
    static void InitKeyIv(CRijndael &oRijndael);
    //填充数据



private:
    static const int sm_alog[256];
    static const int sm_log[256];
    static const char sm_S[256];
    static const char sm_Si[256];
    static const int sm_T1[256];
    static const int sm_T2[256];
    static const int sm_T3[256];
    static const int sm_T4[256];
    static const int sm_T5[256];
    static const int sm_T6[256];
    static const int sm_T7[256];
    static const int sm_T8[256];
    static const int sm_U1[256];
    static const int sm_U2[256];
    static const int sm_U3[256];
    static const int sm_U4[256];
    static const char sm_rcon[30];
    static const int sm_shifts[3][4][2];
    //Error Messages
    static char const* sm_szErrorMsg1;
    static char const* sm_szErrorMsg2;
    //Key Initialization Flag
    bool m_bKeyInit;
    //Encryption (m_Ke) round key
    int m_Ke[MAX_ROUNDS+1][MAX_BC];
    //Decryption (m_Kd) round key
    int m_Kd[MAX_ROUNDS+1][MAX_BC];
    //Key Length
    int m_keylength;
    //Block Size
    int	m_blockSize;
    //Number of Rounds
    int m_iROUNDS;
    //Chain Block
    char m_chain0[MAX_BLOCK_SIZE];
    char m_chain[MAX_BLOCK_SIZE];
    //Auxiliary private use buffers
    int tk[MAX_KC];
    int a[MAX_BC];
    int t[MAX_BC];
};

/**
 *该类实现加密解密
 */
class AESTools
{
private:
    CRijndael crijndael;
public:
    /**
     * @brief AESTools
     * @param key
     * 如果密匙长度不是16，自动纠正
     */
    AESTools(QString *key);

    /**
     * @brief Encrypt
     * @param data
     * @return
     * 将UTF-8字符串加密成HEX串
     */
    QString* Encrypt(QString const* data);

    /**
     * @brief Decrypt
     * @param hexData
     * @return
     * 将HEX串解密为UTF-8字符串
     */
    QString* Decrypt(QString const* hexData);
};

class myAES
{
private:
    AESTools* myAESTools;
public:
    myAES(QString key);
    ~myAES();
    QString myEncrypt(QString in);
    QString myDecrypt(QString in);
};
//AES加解密部分(模式:ECB;部位方式:PKCS5) end

#endif

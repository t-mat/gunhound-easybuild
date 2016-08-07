enum {
 enTexPageBase = enTexCEneH0303Valkylie,
};
gxSprite SprCEneH0303Valkylie[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,144,64,56,40},//胴体
    {enTexPageBase+0,168,0,40,32,8,20},//肩
    {enTexPageBase+0,168,40,72,32,32,16},//腕
    {enTexPageBase+0,0,72,32,24,16,16},//ふともも
    {enTexPageBase+0,56,72,72,40,24,16},//ふくろはぎ
    {enTexPageBase+0,136,72,48,32,16,8},//かかと
    {enTexPageBase+0,0,112,112,38,16,28},//ライフル
    {enTexPageBase+0,192,208,64,48,32,32},//ウイング
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0303Valkylie[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//胴体
    {-20,-5,4,0,0,-60,100,100,100,2},//右肩
    {0,20,5,0,0,0,100,100,100,3},//右腕
    {-10,25,1,0,0,-120,100,100,100,4},//右ふともも
    {-5,30,2,0,0,-60,100,100,100,5},//右ふくろはぎ
    {5,62,0,0,0,0,100,100,100,6},//右かかと
    {25,15,-2,0,0,-40,100,100,100,4},//左ふともも
    {37,30,-3,0,0,-40,100,100,100,5},//左ふくろはぎ
    {55,58,-5,0,0,0,100,100,100,6},//左かかと
    {15,5,3,0,0,0,100,100,100,7},//ライフル
    {-16,-16,1,0,0,0,100,100,100,8},//ウイング
    {0,0,0,0,0,0,0,0,0,9},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enKATA,
    enUDE,
    enMIGIHUTOMOMO,
    enMIGIHUKUROHAGI,
    enMIGIKAKATO,
    enHIDARIHUTOMOMO,
    enHIDARIHUKUROHAGI,
    enHIDARIKAKATO,
    enRAIHUL,
    enWING,
    enMax,
    
};
Sint32 m_sParentCEneH0303Valkylie[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    
};

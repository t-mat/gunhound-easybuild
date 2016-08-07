enum {
 enTexPageBase = enTexCEneEx0007Nozzle,
};
gxSprite SprCEneEx0007Nozzle[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,48,32,30,20},//バーニア根
    {enTexPageBase+0,48,0,64,48,20,18},//バーニア先
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneEx0007Nozzle[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,0,0,0,0,0,100,100,100,1},//バーニア根
    {0,0,0,0,0,0,100,100,100,2},//バーニア先
    {0,0,0,0,0,0,0,0,0,3},//0
};
enum {
    enPARENT,
    enBANIANE,
    enBANIASAKI,
    enMax,
    
};
Sint32 m_sParentCEneEx0007Nozzle[]={
    -1,
    enPARENT,
    enBANIANE,
    
};

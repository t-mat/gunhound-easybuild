enum {
 enTexPageBase = enTexCEneH0407Nautilus,
};
gxSprite SprCEneH0407Nautilus[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+1,0,40,256,184,256,88},//胴体後
    {enTexPageBase+0,0,0,256,256,0,152},//胴体前
    {enTexPageBase+2,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0407Nautilus[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-200,-60,0,0,0,0,100,100,100,2},//胴体前１
    {0,10,0,0,0,0,100,100,100,1},//胴体後
    {0,0,0,0,0,0,0,0,0,3},//0
};
enum {
    enPARENT,
    enDOUTAIMAE,
    enDOUTAIUSIRO,
    enMax,
    
};
Sint32 m_sParentCEneH0407Nautilus[]={
    -1,
    enPARENT,
    enDOUTAIMAE,
    
};

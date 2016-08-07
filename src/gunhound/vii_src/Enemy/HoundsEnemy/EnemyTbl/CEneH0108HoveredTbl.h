enum {
 enTexPageBase = enTexCEneH0108MainGate,
};
gxSprite SprCEneH0108MainGate[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,112,200,112,96},//基地門１
    {enTexPageBase+0,112,0,128,224,0,96},//基地門２
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0108MainGate[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {0,-123,0,0,0,0,100,100,100,1},//基地門１
    {0,0,-1,0,0,0,100,100,100,2},//基地門２
    {0,0,0,0,0,0,0,0,0,3},//0
};
enum {
    enPARENT,
    enKITIMON1,
    enKITIMON2,
    enMax,
    
};
Sint32 m_sParentCEneH0108MainGate[]={
    -1,
    enPARENT,
    enKITIMON1,
    
};

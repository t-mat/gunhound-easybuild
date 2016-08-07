enum {
 enTexPageBase = enTexCEneH0007YusouHeli,
};
gxSprite SprCEneH0007YusouHeli[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,168,64,84,60},//胴体後
    {enTexPageBase+0,0,0,2,2,1,1},//胴体前
    {enTexPageBase+0,0,0,2,2,1,1},//胴体ドアー
    {enTexPageBase+0,24,64,176,16,88,8},//プロペラ前後１
    {enTexPageBase+0,24,80,176,16,88,8},//プロペラ前後２
    {enTexPageBase+0,24,96,176,16,88,8},//プロペラ前後３
    {enTexPageBase+0,24,112,176,16,88,8},//プロペラ前後４
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0007YusouHeli[][16]={
    {0,0,0,0,0,0,100,100,100,0},//シーン
    {0,-10,2,0,0,0,100,100,100,2},//胴体前
    {46,0,0,0,0,0,100,100,100,1},//胴体後
    {0,0,0,0,0,0,100,100,100,3},//胴体ドアー１
    {96,-50,0,0,0,0,100,100,100,5},//プロペラ前１１
    {96,-50,0,0,0,0,0,100,100,6},//プロペラ前２２
    {96,-50,0,0,0,0,0,100,100,7},//プロペラ前３２
    {-20,-58,0,0,0,0,100,100,100,4},//プロペラ後１１
    {-20,-58,0,0,0,0,0,100,100,5},//プロペラ後２２
    {-20,-58,0,0,0,0,0,100,100,6},//プロペラ後３２
    {0,0,0,0,0,0,0,0,0,8},//0
};
enum {
    enPARENT,
    enDOUTAIMAE,
    enDOUTAIGO,
    enDOUTAIDOA,
    enPUROPERAMAE1,
    enPUROPERAMAE2,
    enPUROPERAMAE3,
    enPUROPERAUSIRO1,
    enPUROPERAUSIRO2,
    enPUROPERAUSIRO3,
    enMax,
    
};
Sint32 m_sParentCEneH0007YusouHeli[]={
    -1,
    enPARENT,
    enPARENT,
    enDOUTAIGO,
    enDOUTAIMAE,
    enDOUTAIMAE,
    enDOUTAIMAE,
    enDOUTAIMAE,
    enDOUTAIMAE,
    enDOUTAIMAE,
    
};

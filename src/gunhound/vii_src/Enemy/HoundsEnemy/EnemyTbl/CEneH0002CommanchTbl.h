enum {
 enTexPageBase = enTexCEneH0002Commanch,
};
gxSprite SprCEneH0002Commanch[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,104,48,64,24},//胴体
    {enTexPageBase+0,0,48,104,48,64,24},//回転１
    {enTexPageBase+0,0,96,104,48,64,24},//回転２
    {enTexPageBase+0,0,144,104,48,64,24},//回転３
    {enTexPageBase+0,56,248,24,8,12,4},//後ろプロペラ１
    {enTexPageBase+0,0,200,112,16,56,16},//プロペラ１
    {enTexPageBase+0,0,216,112,16,56,16},//プロペラ２
    {enTexPageBase+0,0,232,112,16,56,16},//プロペラ３
    {enTexPageBase+0,0,248,24,8,8,4},//ミサイルポッド
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0002Commanch[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {0,-22,1,0,0,0,100,100,100,1},//胴体
    {-55,-10,0,0,0,0,100,100,100,5},//後ろプロペラ１１
    {0,-20,0,0,0,0,100,100,100,6},//プロペラ１１
    {0,-20,0,0,0,0,0,100,100,7},//プロペラ２２
    {0,-20,0,0,0,0,0,100,100,8},//プロペラ３２
    {5,16,-1,0,0,20,100,100,100,9},//ミサイルポッド
    {0,0,0,0,0,0,0,0,0,10},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enUSIROPUROPERA1,
    enPUROPERA1,
    enPUROPERA2,
    enPUROPERA3,
    enMISAILPOD,
    enMax,
    
};
Sint32 m_sParentCEneH0002Commanch[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    
};

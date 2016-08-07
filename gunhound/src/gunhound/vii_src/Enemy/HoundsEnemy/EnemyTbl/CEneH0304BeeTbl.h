enum {
 enTexPageBase = enTexCEneH0304Bee,
};
gxSprite SprCEneH0304Bee[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,192,192,64,40,44,7},//胴体
    {enTexPageBase+0,200,136,56,16,28,16},//プロペラ１
    {enTexPageBase+0,200,152,56,17,28,17},//プロペラ２
    {enTexPageBase+0,200,168,56,18,28,18},//プロペラ３
    {enTexPageBase+0,232,240,24,16,8,0},//足
    {enTexPageBase+0,200,240,32,16,0,8},//マシンガン
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0304Bee[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {0,-35,0,0,0,0,100,100,100,1},//胴体
    {0,0,0,0,0,0,100,100,100,2},//プロペラ１１
    {0,0,0,0,0,0,0,100,100,3},//プロペラ２２
    {0,0,0,0,0,0,0,100,100,4},//プロペラ３２
    {-5,20,0,0,0,0,100,100,100,5},//足
    {5,20,-1,0,0,0,100,100,100,6},//マシンガン
    {0,0,0,0,0,0,0,0,0,7},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enPUROPERA1,
    enPUROPERA2,
    enPUROPERA3,
    enASI,
    enMASHINGUN,
    enMax,
    
};
Sint32 m_sParentCEneH0304Bee[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    
};

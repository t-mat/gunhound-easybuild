enum {
 enTexPageBase = enTexCEneH0401MovingCannon,
};
gxSprite SprCEneH0401MovingCannon[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,32,16,0,7},//砲台
    {enTexPageBase+0,40,0,64,40,24,32},//胴体
    {enTexPageBase+0,0,40,72,16,40,8},//キャタピラー１
    {enTexPageBase+0,0,64,72,16,40,8},//キャタピラー２
    {enTexPageBase+0,0,88,72,16,40,8},//キャタピラー３
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0401MovingCannon[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-10,-20,0,0,0,0,100,100,100,2},//胴体１
    {40,-9,-1,0,0,0,100,100,100,1},//砲台１１
    {0,-9,1,0,0,0,100,100,100,3},//キャタピラー１１
    {0,-9,1,0,0,0,0,100,100,4},//キャタピラー２２
    {0,-9,1,0,0,0,0,100,100,5},//キャタピラー３２
    {0,0,0,0,0,0,0,0,0,6},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enHOUDAI,
    enKYATAPIRA1,
    enKYATAPIRA2,
    enKYATAPIRA3,
    enMax,
    
};
Sint32 m_sParentCEneH0401MovingCannon[]={
    -1,
    enPARENT,
    enDOUTAI,
    enPARENT,
    enPARENT,
    enPARENT,
    
};

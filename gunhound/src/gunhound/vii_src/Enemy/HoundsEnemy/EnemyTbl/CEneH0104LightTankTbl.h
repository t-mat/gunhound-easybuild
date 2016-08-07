enum {
 enTexPageBase = enTexCEneH0104LightTank,
};
gxSprite SprCEneH0104LightTank[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,88,0,64,16,28,16},//砲台１
    {enTexPageBase+0,88,16,56,16,28,16},//砲台２
    {enTexPageBase+0,88,32,48,16,28,16},//砲台３
    {enTexPageBase+0,88,48,72,16,36,8},//胴体
    {enTexPageBase+0,88,64,72,16,40,8},//足
    {enTexPageBase+0,88,88,72,16,40,8},//キャタピラー１
    {enTexPageBase+0,88,112,72,16,40,8},//キャタピラー２
    {enTexPageBase+0,88,136,72,16,40,8},//キャタピラー３
    {enTexPageBase+0,88,160,16,16,8,8},//キャタイヤ
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0104LightTank[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {0,-21,0,0,0,0,100,100,100,4},//胴体
    {0,-2,1,0,0,0,100,100,100,1},//砲台１１
    {0,0,1,0,0,0,0,100,100,2},//砲台２２
    {0,0,0,0,0,0,0,100,100,3},//砲台３２
    {0,10,0,0,0,0,100,100,100,5},//足
    {0,11,1,0,0,0,100,100,100,6},//キャタピラー１１
    {0,11,1,0,0,0,0,100,100,7},//キャタピラー２２
    {0,11,1,0,0,0,0,100,100,8},//キャタピラー３２
    {-32,5,0,0,0,0,100,50,50,9},//キャタイヤ１１
    {-26,-1,0,0,0,0,100,70,70,9},//キャタイヤ２１
    {-17,3,0,0,0,0,100,70,70,9},//キャタイヤ３１
    {-8,-1,0,0,0,0,100,70,70,9},//キャタイヤ４１
    {1,3,0,0,0,0,100,70,70,9},//キャタイヤ５１
    {10,-1,0,0,0,0,100,70,70,9},//キャタイヤ６１
    {17,3,0,0,0,0,100,50,50,9},//キャタイヤ７１
    {0,0,0,0,0,0,0,0,0,10},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enHOUDAI1,
    enHOUDAI2,
    enHOUDAI3,
    enASI,
    enKYATAPIRA1,
    enKYATAPIRA2,
    enKYATAPIRA3,
    enKYATIYA1,
    enKYATIYA2,
    enKYATIYA3,
    enKYATIYA4,
    enKYATIYA5,
    enKYATIYA6,
    enKYATIYA7,
    enMax,
    
};
Sint32 m_sParentCEneH0104LightTank[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    
};

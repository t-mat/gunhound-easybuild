enum {
 enTexPageBase = enTexCEneH0004Tachikoma,
};
gxSprite SprCEneH0004Tachikoma[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,136,0,96,40,64,24},//キャビン
    {enTexPageBase+0,136,40,72,16,32,16},//足
    {enTexPageBase+0,216,40,16,16,8,8},//タイヤ１
    {enTexPageBase+0,136,64,72,16,36,8},//キャタピラー１
    {enTexPageBase+0,136,88,72,16,36,8},//キャタピラー２
    {enTexPageBase+0,136,112,72,16,36,8},//キャタピラー３
    {enTexPageBase+0,136,136,16,16,8,8},//キャタイヤ
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0004Tachikoma[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {10,-20,0,0,0,0,100,100,100,1},//キャビン
    {-28,20,0,0,0,0,100,100,100,2},//足
    {24,10,0,0,0,0,100,100,100,3},//タイヤ１１
    {3,-9,0,0,0,0,100,100,100,4},//キャタピラー１１
    {3,-9,0,0,0,0,0,100,100,5},//キャタピラー２２
    {3,-9,0,0,0,0,0,100,100,6},//キャタピラー３２
    {-23,-7,0,0,0,0,100,70,70,7},//キャタイヤ１１
    {-14,-11,0,0,0,0,100,70,70,7},//キャタイヤ２１
    {-7,-5,0,0,0,0,100,70,70,7},//キャタイヤ３１
    {0,-11,0,0,0,0,100,70,70,7},//キャタイヤ４１
    {7,-5,0,0,0,0,100,70,70,7},//キャタイヤ５１
    {14,-11,0,0,0,0,100,70,70,7},//キャタイヤ６１
    {22,-5,0,0,0,0,100,70,70,7},//キャタイヤ７１
    {29,-11,0,0,0,0,100,70,70,7},//キャタイヤ８１
    {0,0,0,0,0,0,0,0,0,8},//0
};
enum {
    enPARENT,
    enCABIN,
    enASI,
    enTIYA1,
    enKYATAPIRA1,
    enKYATAPIRA2,
    enKYATAPIRA3,
    enKYATIRE1,
    enKYATIRE2,
    enKYATIRE3,
    enKYATIRE4,
    enKYATIRE5,
    enKYATIRE6,
    enKYATIRE7,
    enKYATIRE8,
    enMax,
    
};
Sint32 m_sParentCEneH0004Tachikoma[]={
    -1,
    enPARENT,
    enCABIN,
    enCABIN,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    enASI,
    
};

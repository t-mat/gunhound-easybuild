enum {
 enTexPageBase = enTexCEneH0301ArmoredCar,
};
gxSprite SprCEneH0301ArmoredCar[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,104,96,40,64,24},//キャビン
    {enTexPageBase+0,104,120,16,8,8,8},//砲台１
    {enTexPageBase+0,104,104,48,16,16,12},//砲台２
    {enTexPageBase+0,0,144,72,16,32,16},//足
    {enTexPageBase+0,80,144,16,16,8,8},//タイヤ１
    {enTexPageBase+0,0,168,72,16,36,8},//キャタピラー１
    {enTexPageBase+0,0,192,72,16,36,8},//キャタピラー２
    {enTexPageBase+0,0,216,72,16,36,8},//キャタピラー３
    {enTexPageBase+0,0,240,16,16,8,8},//キャタイヤ
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0301ArmoredCar[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {10,-20,0,0,0,0,100,100,100,1},//キャビン
    {-36,-10,0,0,0,0,100,100,100,2},//砲台１１
    {0,-5,0,0,0,0,100,100,100,3},//砲台２１
    {-28,20,0,0,0,0,100,100,100,4},//足
    {24,10,0,0,0,0,100,100,100,5},//タイヤ１１
    {3,-9,0,0,0,0,100,100,100,6},//キャタピラー１１
    {3,-9,0,0,0,0,0,100,100,7},//キャタピラー２２
    {3,-9,0,0,0,0,0,100,100,8},//キャタピラー３２
    {-23,-7,0,0,0,0,100,70,70,9},//キャタイヤ１１
    {-14,-11,0,0,0,0,100,70,70,9},//キャタイヤ２１
    {-7,-5,0,0,0,0,100,70,70,9},//キャタイヤ３１
    {0,-11,0,0,0,0,100,70,70,9},//キャタイヤ４１
    {7,-5,0,0,0,0,100,70,70,9},//キャタイヤ５１
    {14,-11,0,0,0,0,100,70,70,9},//キャタイヤ６１
    {22,-5,0,0,0,0,100,70,70,9},//キャタイヤ７１
    {29,-11,0,0,0,0,100,70,70,9},//キャタイヤ８１
    {0,0,0,0,0,0,0,0,0,10},//0
};
enum {
    enPARENT,
    enCABIN,
    enHOUDAI1,
    enHOUDAI2,
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
Sint32 m_sParentCEneH0301ArmoredCar[]={
    -1,
    enPARENT,
    enCABIN,
    enHOUDAI1,
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

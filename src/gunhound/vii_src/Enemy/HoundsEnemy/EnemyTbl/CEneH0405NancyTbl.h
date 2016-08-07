enum {
 enTexPageBase = enTexCEneH0405Nancy,
};
gxSprite SprCEneH0405Nancy[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,96,72,32,56},//胴体
    {enTexPageBase+0,0,72,88,80,32,16},//右足
    {enTexPageBase+0,88,72,88,80,32,16},//左足
    {enTexPageBase+0,176,0,40,56,16,8},//肩
    {enTexPageBase+0,176,56,8,32,3,0},//腕接続パーツ
    {enTexPageBase+0,184,56,48,32,0,16},//腕
    {enTexPageBase+0,192,88,40,8,0,4},//伸縮パーツ
    {enTexPageBase+0,0,152,16,16,0,8},//ミサイル１
    {enTexPageBase+0,16,152,16,16,8,0},//ミサイル２
    {enTexPageBase+0,16,168,16,16,8,0},//ミサイル３
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0405Nancy[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {0,-65,0,0,0,0,100,100,100,1},//胴体
    {0,0,0,0,0,0,100,100,100,2},//右足
    {0,0,0,0,0,0,100,100,100,3},//左足
    {0,-20,4,0,0,0,100,100,100,4},//肩
    {10,45,-1,0,0,0,100,100,100,6},//腕１
    {0,50,-3,0,0,0,100,140,150,7},//伸縮パーツ１
    {0,50,-3,0,0,0,100,140,100,7},//伸縮パーツ２
    {0,32,-2,0,0,0,100,100,100,5},//腕接続パーツ
    {0,0,0,0,0,0,0,0,0,11},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enMIGIASI,
    enHIDARIASI,
    enKATA,
    enUDE,
    enSINSYUKUPARTS1,
    enSINSYUKUPARTS2,
    enUDESETUZOKU,
    enMax,
    
};
Sint32 m_sParentCEneH0405Nancy[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    enKATA,
    enKATA,
    enKATA,
    enKATA,
    
};

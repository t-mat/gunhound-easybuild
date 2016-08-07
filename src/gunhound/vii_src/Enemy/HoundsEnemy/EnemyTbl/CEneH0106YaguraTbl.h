enum {
 enTexPageBase = enTexCEneH0106Yagura,
};
gxSprite SprCEneH0106Yagura[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,0,0,64,152,32,152},//監視塔
    {enTexPageBase+0,8,160,40,15,11,11},//銃
    {enTexPageBase+0,8,176,16,16,8,8},//銃立て
    {enTexPageBase+0,64,0,32,32,16,16},//ガンナー1
    {enTexPageBase+0,96,0,32,32,16,16},//ガンナー2
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0106Yagura[][16]={
    {0,80,0,0,0,0,100,100,100,0},//シーン
    {8,-114,0,0,0,0,100,100,100,4},//ガンナーディスプ
    {0,0,1,0,0,0,100,100,100,1},//監視塔
    {20,-105,2,0,0,0,100,100,100,3},//銃立て
    {-5,-4,2,0,0,0,100,100,100,2},//銃１
    {0,0,0,0,0,0,0,0,0,6},//0
};
enum {
    enPARENT,
    enGUNNER,
    enKANSITOU,
    enGUNTATE,
    enGUN,
    enMax,
    
};
Sint32 m_sParentCEneH0106Yagura[]={
    -1,
    enPARENT,
    enPARENT,
    enKANSITOU,
    enGUNTATE,
};

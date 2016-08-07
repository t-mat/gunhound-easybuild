enum {
 enTexPageBase = enTexCEneH0411Stunbarrier,
};
gxSprite SprCEneH0411Stunbarrier[]={
    {enTexPageBase+0,0,0,1,1,0,0},//ダミー
    {enTexPageBase+0,-144+144,0,56,32,16,32},//スタンオブジェ１
    {enTexPageBase+0,-144+200,0,56,32,40,32},//スタンオブジェ２
    {enTexPageBase+0,-144+144,32,24,24,12,12},//雷１
    {enTexPageBase+0,-144+168,32,24,24,12,12},//雷２
    {enTexPageBase+0,-144+192,32,24,24,12,12},//雷３
    {enTexPageBase+0,-144+216,32,24,24,12,12},//雷４
    {enTexPageBase+1,-144+0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0411Stunbarrier[][16]={
    {160,200,0,0,0,0,100,100,100,0},//シーン
    {-80,0,0,0,0,0,100,100,100,1},//スタンオブジェ１
    {80,0,0,0,0,0,100,100,100,2},//スタンオブジェ２
    {-30,-12,0,0,0,0,100,100,100,3},//雷１１
    {-30,-12,0,0,0,0,100,100,100,4},//雷２１
    {-30,-12,0,0,0,0,100,100,100,5},//雷３１
    {-30,-12,0,0,0,0,100,100,100,6},//雷４１
    {0,0,0,0,0,0,0,0,0,7},//0
};
enum {
    enPARENT,
    enSTUNOBJ1,
    enSTUNOBJ2,
    enKAMINARI1,
    enKAMINARI2,
    enKAMINARI3,
    enKAMINARI4,
    enMax,
    
};
Sint32 m_sParentCEneH0411Stunbarrier[]={
    -1,
    enPARENT,
    enPARENT,
    enPARENT,
    enPARENT,
    enPARENT,
    enPARENT,
    
};

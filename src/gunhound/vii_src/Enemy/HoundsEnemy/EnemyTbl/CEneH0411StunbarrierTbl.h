enum {
 enTexPageBase = enTexCEneH0411Stunbarrier,
};
gxSprite SprCEneH0411Stunbarrier[]={
    {enTexPageBase+0,0,0,1,1,0,0},//�_�~�[
    {enTexPageBase+0,-144+144,0,56,32,16,32},//�X�^���I�u�W�F�P
    {enTexPageBase+0,-144+200,0,56,32,40,32},//�X�^���I�u�W�F�Q
    {enTexPageBase+0,-144+144,32,24,24,12,12},//���P
    {enTexPageBase+0,-144+168,32,24,24,12,12},//���Q
    {enTexPageBase+0,-144+192,32,24,24,12,12},//���R
    {enTexPageBase+0,-144+216,32,24,24,12,12},//���S
    {enTexPageBase+1,-144+0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0411Stunbarrier[][16]={
    {160,200,0,0,0,0,100,100,100,0},//�V�[��
    {-80,0,0,0,0,0,100,100,100,1},//�X�^���I�u�W�F�P
    {80,0,0,0,0,0,100,100,100,2},//�X�^���I�u�W�F�Q
    {-30,-12,0,0,0,0,100,100,100,3},//���P�P
    {-30,-12,0,0,0,0,100,100,100,4},//���Q�P
    {-30,-12,0,0,0,0,100,100,100,5},//���R�P
    {-30,-12,0,0,0,0,100,100,100,6},//���S�P
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

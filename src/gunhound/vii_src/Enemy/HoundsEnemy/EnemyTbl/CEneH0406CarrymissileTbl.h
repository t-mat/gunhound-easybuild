enum {
 enTexPageBase = enTexCEneH0405Nancy,
};
gxSprite SprCEneH0406Carrymissile[]={
    {enTexPageBase+0,0,0,1,1,0,0},//�_�~�[
    {enTexPageBase+0,0,0,96,64,64,32},//����
    {enTexPageBase+0,96,0,128,32,16,16},//�J�����P
    {enTexPageBase+0,96,32,128,32,16,16},//�J�����Q
    {enTexPageBase+0,224,0,32,64,8,32},//��[
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0406Carrymissile[][16]={
    {0,0,0,0,0,0,100,100,100,0},//�V�[��
    {0,0,0,0,0,0,100,100,100,1},//���̂P
    {16,-10,0,0,0,0,100,100,100,2},//�J�����P�P
    {16,10,0,0,0,0,100,100,100,3},//�J�����Q�P
    {120,0,-1,0,0,0,100,100,100,4},//��[�ʒu
    {0,0,0,0,0,0,0,0,0,5},//0
};
enum {
    enPARENT,
    enDOUTAI,
    enKAIKOUBU1,
    enKAIKOUBU2,
    enSENTAN,
    enMax,
    
};
Sint32 m_sParentCEneH0406Carrymissile[]={
    -1,
    enPARENT,
    enDOUTAI,
    enDOUTAI,
    enDOUTAI,
    
};

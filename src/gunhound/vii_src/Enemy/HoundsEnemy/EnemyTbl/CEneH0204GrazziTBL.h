enum {
 enTexPageBase = enTexCEneH0204Grazzi,
};
gxSprite SprCEneH0204Grazzi[]={
    {enTexPageBase+0,0,0,1,1,0,0},//�_�~�[
    {enTexPageBase+0,0,0,88,64,56,41},//�{�f�B
    {enTexPageBase+0,96,0,40,40,10,4},//�E��
    {enTexPageBase+0,0,96,48,24,12,12},//�E������
    {enTexPageBase+0,48,112,120,48,24,16},//�E����
    {enTexPageBase+0,168,96,64,16,32,0},//�E������
    {enTexPageBase+0,176,8,72,24,0,12},//�o���J���E
    {enTexPageBase+0,136,56,120,32,24,32},//�����P�b�g
    {enTexPageBase+1,0,0,0,0,0,0},//0
};
Sint32 sPosCEneH0204Grazzi[][16]={
    {0,0,0,0,0,0,100,100,100,0},//�V�[��
    {0,-100,0,0,0,0,100,100,100,1},//�{�f�B
    {-20,0,2,0,0,0,100,100,100,2},//�E��
    {10,10,-4,0,0,0,100,100,100,2},//����
    {-30,0,1,0,0,-140,100,100,100,3},//�E������
    {-42,30,2,0,0,-60,100,100,100,4},//�E����
    {-5,100,1,0,0,0,100,100,100,5},//�E������
    {-2,0,-2,0,0,-90,100,100,100,3},//��������
    {-5,35,-1,0,0,-60,100,100,100,4},//������
    {45,100,-2,0,0,0,100,100,100,5},//��������
    {0,30,4,0,0,0,100,100,100,6},//�o���J���E
    {30,40,-3,0,0,0,100,100,100,6},//�o���J����
    {-25,-35,0,0,0,0,100,100,100,7},//�����P�b�g
    {0,0,0,0,0,0,0,0,0,8},//0
};
enum {
    enPARENT,
    enBODY,
    enMIGIKATA,
    enHIDARIKATA,
    enMIGIHUTOMOMO,
    enMIGISUNE,
    enMIGIKAKATO,
    enHIDARIHUTOMOMO,
    enHIDARISUNE,
    enHIDARIKAKATO,
    enBARUKANMIGI,
    enBARUKANHIDARI,
    enATAMAROKETTO,
    enMax,
    
};
Sint32 m_sParentCEneH0204Grazzi[]={
    -1,
    enPARENT,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    enBODY,
    
};

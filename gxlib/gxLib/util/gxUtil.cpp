//--------------------------------------------------
//
// gxTexdManager.h
// CRC値や、円と円、線と線の当たり判定など
// よく使うコードをまとめています
//
//--------------------------------------------------

#include <gxLib.h>
#include "../gx.h"

//---------------------------------------
// 暗号化キー(鍵長は32～448bit)
// プロジェクトごとに変えたほうがいいと思う
//---------------------------------------
Uint8   PASS_KEY[] =	"gxlib_encryption";
#define PASS_KEY_SIZE  	sizeof(PASS_KEY)

typedef struct
{
	Uint8   hk[1];
	Uint8   zh[2];
	Uint8   zk[2];
}   hz1;

typedef struct
{
	Uint8   hk[2];
	Uint8   zh[2];
	Uint8   zk[2];
}   hz2;


#define HZ2SIZE 26
#define HZ1SIZE 65

const Uint32 crctable[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

Uint32 gxUtil::CalcCrc32(void *dat, int siz)
{
	Uint8  *s = (Uint8*)dat;
	Uint32 r;

	r = 0xFFFFFFFFUL;
	while (--siz >= 0)
		r = (r >> 8) ^ crctable[( (Uint8) r) ^ *s++];

	Uint32 crc=(int) (r ^ 0xFFFFFFFFUL);

	return crc;
}


Uint32 gxUtil::atox( gxChar *p )
{
	int n,ret=0;

	for(int i=strlen(p)-1,j=0 ;i>=0;i--,j++)
	{	
		switch(p[i]){
		 case '0':  n=0;	break;
		 case '1':  n=1;	break;
		 case '2':  n=2;	break;
		 case '3':  n=3;	break;
		 case '4':  n=4;	break;
		 case '5':  n=5;	break;
		 case '6':  n=6;	break;
		 case '7':  n=7;	break;
		 case '8':  n=8;	break;
		 case '9':  n=9;	break;

		 case 'A':  case 'a':   n=10;   break;
		 case 'B':  case 'b':   n=11;   break;
		 case 'C':  case 'c':   n=12;   break;
		 case 'D':  case 'd':   n=13;   break;
		 case 'E':  case 'e':   n=14;   break;
		 case 'F':  case 'f':   n=15;   break;
		 default:
			return 0;
		}

		ret |= (n<<(4*j));
	}

	return ret;
}

static  hz2 t2[HZ2SIZE] =
{
	{   0xB3,0xDE   /* ｳﾞ */,   0x83,0x94   /* ヴ */,   0x83,0x94   /* ヴ */	},
	{   0xB6,0xDE   /* ｶﾞ */,   0x82,0xAA   /* が */,   0x83,0x4B   /* ガ */	},
	{   0xB7,0xDE   /* ｷﾞ */,   0x82,0xAC   /* ぎ */,   0x83,0x4D   /* ギ */	},
	{   0xB8,0xDE   /* ｸﾞ */,   0x82,0xAE   /* ぐ */,   0x83,0x4F   /* グ */	},
	{   0xB9,0xDE   /* ｹﾞ */,   0x82,0xB0   /* げ */,   0x83,0x51   /* ゲ */	},
	{   0xBA,0xDE   /* ｺﾞ */,   0x82,0xB2   /* ご */,   0x83,0x53   /* ゴ */	},
	{   0xBB,0xDE   /* ｻﾞ */,   0x82,0xB4   /* ざ */,   0x83,0x55   /* ザ */	},
	{   0xBC,0xDE   /* ｼﾞ */,   0x82,0xB6   /* じ */,   0x83,0x57   /* ジ */	},
	{   0xBD,0xDE   /* ｽﾞ */,   0x82,0xB8   /* ず */,   0x83,0x59   /* ズ */	},
	{   0xBE,0xDE   /* ｾﾞ */,   0x82,0xBA   /* ぜ */,   0x83,0x5B   /* ゼ */	},
	{   0xBF,0xDE   /* ｿﾞ */,   0x82,0xBC   /* ぞ */,   0x83,0x5D   /* ゾ */	},
	{   0xC0,0xDE   /* ﾀﾞ */,   0x82,0xBE   /* だ */,   0x83,0x5F   /* ダ */	},
	{   0xC1,0xDE   /* ﾁﾞ */,   0x82,0xC0   /* ぢ */,   0x83,0x61   /* ヂ */	},
	{   0xC2,0xDE   /* ﾂﾞ */,   0x82,0xC3   /* づ */,   0x83,0x64   /* ヅ */	},
	{   0xC3,0xDE   /* ﾃﾞ */,   0x82,0xC5   /* で */,   0x83,0x66   /* デ */	},
	{   0xC4,0xDE   /* ﾄﾞ */,   0x82,0xC7   /* ど */,   0x83,0x68   /* ド */	},
	{   0xCA,0xDE   /* ﾊﾞ */,   0x82,0xCE   /* ば */,   0x83,0x6F   /* バ */	},
	{   0xCB,0xDE   /* ﾋﾞ */,   0x82,0xD1   /* び */,   0x83,0x72   /* ビ */	},
	{   0xCC,0xDE   /* ﾌﾞ */,   0x82,0xD4   /* ぶ */,   0x83,0x75   /* ブ */	},
	{   0xCD,0xDE   /* ﾍﾞ */,   0x82,0xD7   /* べ */,   0x83,0x78   /* ベ */	},
	{   0xCE,0xDE   /* ﾎﾞ */,   0x82,0xDA   /* ぼ */,   0x83,0x7B   /* ボ */	},
	{   0xCA,0xDF   /* ﾊﾟ */,   0x82,0xCF   /* ぱ */,   0x83,0x70   /* パ */	},
	{   0xCB,0xDF   /* ﾋﾟ */,   0x82,0xD2   /* ぴ */,   0x83,0x73   /* ピ */	},
	{   0xCC,0xDF   /* ﾌﾟ */,   0x82,0xD5   /* ぷ */,   0x83,0x76   /* プ */	},
	{   0xCD,0xDF   /* ﾍﾟ */,   0x82,0xD8   /* ぺ */,   0x83,0x79   /* ペ */	},
	{   0xCE,0xDF   /* ﾎﾟ */,   0x82,0xDB   /* ぽ */,   0x83,0x7C   /* ポ */	}
};


static  hz1 t1[HZ1SIZE] =
{
	{   0xA0	/*   */,	0x81,0x40   /*	*/,   0x81,0x40   /*	*/	},
	{   0xA4	/* ､ */,	0x81,0x41   /* 、 */,   0x81,0x41   /* 、 */	},
	{   0xA1	/* ｡ */,	0x81,0x42   /* 。 */,   0x81,0x42   /* 。 */	},
	{   0xA5	/* ･ */,	0x81,0x45   /* ・ */,   0x81,0x45   /* ・ */	},
	{   0xDE	/* ﾞ */,	0x81,0x4A   /* ゛ */,   0x81,0x4A   /* ゛ */	},
	{   0xDF	/* ﾟ */,	0x81,0x4B   /* ゜ */,   0x81,0x4B   /* ゜ */	},
	{   0xB0	/* ｰ */,	0x81,0x5B   /* ー */,   0x81,0x5B   /* ー */	},
	{   0xA2	/* ｢ */,	0x81,0x75   /* 「 */,   0x81,0x75   /* 「 */	},
	{   0xA3	/* ｣ */,	0x81,0x76   /* 」 */,   0x81,0x76   /* 」 */	},
	{   0xA7	/* ｧ */,	0x82,0x9F   /* ぁ */,   0x83,0x40   /* ァ */	},
	{   0xB1	/* ｱ */,	0x82,0xA0   /* あ */,   0x83,0x41   /* ア */	},
	{   0xA8	/* ｨ */,	0x82,0xA1   /* ぃ */,   0x83,0x42   /* ィ */	},
	{   0xB2	/* ｲ */,	0x82,0xA2   /* い */,   0x83,0x43   /* イ */	},
	{   0xA9	/* ｩ */,	0x82,0xA3   /* ぅ */,   0x83,0x44   /* ゥ */	},
	{   0xB3	/* ｳ */,	0x82,0xA4   /* う */,   0x83,0x45   /* ウ */	},
	{   0xAA	/* ｪ */,	0x82,0xA5   /* ぇ */,   0x83,0x46   /* ェ */	},
	{   0xB4	/* ｴ */,	0x82,0xA6   /* え */,   0x83,0x47   /* エ */	},
	{   0xAB	/* ｫ */,	0x82,0xA7   /* ぉ */,   0x83,0x48   /* ォ */	},
	{   0xB5	/* ｵ */,	0x82,0xA8   /* お */,   0x83,0x49   /* オ */	},
	{   0xB6	/* ｶ */,	0x82,0xA9   /* か */,   0x83,0x4A   /* カ */	},
	{   0xB7	/* ｷ */,	0x82,0xAB   /* き */,   0x83,0x4C   /* キ */	},
	{   0xB8	/* ｸ */,	0x82,0xAD   /* く */,   0x83,0x4E   /* ク */	},
	{   0xB9	/* ｹ */,	0x82,0xAF   /* け */,   0x83,0x50   /* ケ */	},
	{   0xBA	/* ｺ */,	0x82,0xB1   /* こ */,   0x83,0x52   /* コ */	},
	{   0xBB	/* ｻ */,	0x82,0xB3   /* さ */,   0x83,0x54   /* サ */	},
	{   0xBC	/* ｼ */,	0x82,0xB5   /* し */,   0x83,0x56   /* シ */	},
	{   0xBD	/* ｽ */,	0x82,0xB7   /* す */,   0x83,0x58   /* ス */	},
	{   0xBE	/* ｾ */,	0x82,0xB9   /* せ */,   0x83,0x5A   /* セ */	},
	{   0xBF	/* ｿ */,	0x82,0xBB   /* そ */,   0x83,0x5C   /* ソ */	},
	{   0xC0	/* ﾀ */,	0x82,0xBD   /* た */,   0x83,0x5E   /* タ */	},
	{   0xC1	/* ﾁ */,	0x82,0xBF   /* ち */,   0x83,0x60   /* チ */	},
	{   0xAF	/* ｯ */,	0x82,0xC1   /* っ */,   0x83,0x62   /* ッ */	},
	{   0xC2	/* ﾂ */,	0x82,0xC2   /* つ */,   0x83,0x63   /* ツ */	},
	{   0xC3	/* ﾃ */,	0x82,0xC4   /* て */,   0x83,0x65   /* テ */	},
	{   0xC4	/* ﾄ */,	0x82,0xC6   /* と */,   0x83,0x67   /* ト */	},
	{   0xC5	/* ﾅ */,	0x82,0xC8   /* な */,   0x83,0x69   /* ナ */	},
	{   0xC6	/* ﾆ */,	0x82,0xC9   /* に */,   0x83,0x6A   /* ニ */	},
	{   0xC7	/* ﾇ */,	0x82,0xCA   /* ぬ */,   0x83,0x6B   /* ヌ */	},
	{   0xC8	/* ﾈ */,	0x82,0xCB   /* ね */,   0x83,0x6C   /* ネ */	},
	{   0xC9	/* ﾉ */,	0x82,0xCC   /* の */,   0x83,0x6D   /* ノ */	},
	{   0xCA	/* ﾊ */,	0x82,0xCD   /* は */,   0x83,0x6E   /* ハ */	},
	{   0xCB	/* ﾋ */,	0x82,0xD0   /* ひ */,   0x83,0x71   /* ヒ */	},
	{   0xCC	/* ﾌ */,	0x82,0xD3   /* ふ */,   0x83,0x74   /* フ */	},
	{   0xCD	/* ﾍ */,	0x82,0xD6   /* へ */,   0x83,0x77   /* ヘ */	},
	{   0xCE	/* ﾎ */,	0x82,0xD9   /* ほ */,   0x83,0x7A   /* ホ */	},
	{   0xCF	/* ﾏ */,	0x82,0xDC   /* ま */,   0x83,0x7D   /* マ */	},
	{   0xD0	/* ﾐ */,	0x82,0xDD   /* み */,   0x83,0x7E   /* ミ */	},
	{   0xD1	/* ﾑ */,	0x82,0xDE   /* む */,   0x83,0x80   /* ム */	},
	{   0xD2	/* ﾒ */,	0x82,0xDF   /* め */,   0x83,0x81   /* メ */	},
	{   0xD3	/* ﾓ */,	0x82,0xE0   /* も */,   0x83,0x82   /* モ */	},
	{   0xAC	/* ｬ */,	0x82,0xE1   /* ゃ */,   0x83,0x83   /* ャ */	},
	{   0xD4	/* ﾔ */,	0x82,0xE2   /* や */,   0x83,0x84   /* ヤ */	},
	{   0xAD	/* ｭ */,	0x82,0xE3   /* ゅ */,   0x83,0x85   /* ュ */	},
	{   0xD5	/* ﾕ */,	0x82,0xE4   /* ゆ */,   0x83,0x86   /* ユ */	},
	{   0xAE	/* ｮ */,	0x82,0xE5   /* ょ */,   0x83,0x87   /* ョ */	},
	{   0xD6	/* ﾖ */,	0x82,0xE6   /* よ */,   0x83,0x88   /* ヨ */	},
	{   0xD7	/* ﾗ */,	0x82,0xE7   /* ら */,   0x83,0x89   /* ラ */	},
	{   0xD8	/* ﾘ */,	0x82,0xE8   /* り */,   0x83,0x8A   /* リ */	},
	{   0xD9	/* ﾙ */,	0x82,0xE9   /* る */,   0x83,0x8B   /* ル */	},
	{   0xDA	/* ﾚ */,	0x82,0xEA   /* れ */,   0x83,0x8C   /* レ */	},
	{   0xDB	/* ﾛ */,	0x82,0xEB   /* ろ */,   0x83,0x8D   /* ロ */	},
	{   0xDC	/* ﾜ */,	0x82,0xED   /* わ */,   0x83,0x8F   /* ワ */	},
	{   0xA6	/* ｦ */,	0x82,0xF0   /* を */,   0x83,0x92   /* ヲ */	},
	{   0xDD	/* ﾝ */,	0x82,0xF1   /* ん */,   0x83,0x93   /* ン */	},
	{   0x2D	/* - */,	0x81,0x7C   /* － */,   0x81,0x7C   /* － */	},
};


/*
	シングルバイト			  (0x20 - 0x7E、0xA1 - 0xDF)
	マルチバイト文字の先行バイト(0x81 - 0x9F、0xE0 - 0xFC)
	マルチバイト文字の後続バイト(0x40 - 0x7E、0x80 - 0xFC)
	無効な文字				  (0x20 - 0x7E、0xA1 - 0xDF、
								 0x81 - 0x9F、0xE0 - 0xFC 以外の文字)
*/


//半角カナを全角カタカナにする
void gxUtil::han2zen( gxChar* pSrc , gxChar *pDst)
{
   // pSrc:入力文字列へのポインタ。ヌル終端文字列
   // pDst:出力文字列へのポインタ。最大入力文字列の２倍のサイズが必要。ヌル終端文字列にして返す
	static gxChar *pin , *pout,c[2],b2;
	static int slen,i;

	slen = strlen(pSrc);
	pin  = pSrc ;
	pout = pDst;

	while(1)
	{
		if ( pin >= pSrc + slen ) break;

		if	  ( ( 0x81 <= *pin && *pin <= 0x9F ) ||
				  ( 0xE0 <= *pin && *pin <= 0xFC ) )  /* 全角文字の１バイト目 */
		{
			*pout++ = *pin++;
			*pout++ = *pin++;
		}
		else if ( (0xA0 <= *pin && *pin <= 0xDF) || (*pin == 0x2D) )  /* 半角カナ 「-」に対応させました(2004.02.19:TOMI)*/
		{
			c[0] = *pin;
			c[1] = *(pin+1);
			b2   = 0;
			for( i = 0 ; i < HZ2SIZE ; i++ )
			{
				if ( c[0] == t2[i].hk[0] &&
					 c[1] == t2[i].hk[1]  )
				{
					*pout++ = t2[i].zk[0];
					*pout++ = t2[i].zk[1];
					pin += 2;
					b2 = 1;
					break;
				}
			}

			if ( b2 ) continue;

			for( i = 0 ; i < HZ1SIZE ; i++ )
			{
				if ( c[0] == t1[i].hk[0]  )
				{
					*pout++ = t1[i].zk[0];
					*pout++ = t1[i].zk[1];
					pin ++;
					break;
				}
			}
		}
		else	/* 半角カナ以外の半角文字 */
		{
			*pout++ = *pin++;
		}
	}

	*pout = 0x00;
}

gxBool gxUtil::GetFileNameWithoutPath( gxChar * in , gxChar *out)
{
	//ファイル名だけを取り出す

	for(int i=strlen(in);i>0;i--)
	{
		if( in[i]=='\\' || in[i]=='/')
		{
			sprintf( out,"%s",&in[i+1]);
			return gxTrue;
		}
	}
	return gxFalse;
}


gxBool gxUtil::GetPath( gxChar *in , gxChar *out )
{
	//ファイルのパスだけを取り出す

	for(int i=strlen(in);i>0;i--)
	{
		if( in[i]=='\\' || in[i]=='/')
		{
			sprintf(out,"%s",&in[0]);
			out[i] = 0x00;
			return gxTrue;
		}
	}
	return gxFalse;
}

gxBool gxUtil::GetExt( gxChar *in , gxChar* out )
{
	//拡張子を得る

	for(int i=strlen(in);i>0;i--)
	{
		if(in[i]=='.')
		{
			sprintf(out,"%s",&in[i]);
			out[i] = 0x00;
			/*doku*///strupr(out);
			return gxTrue;
		}
	}

	return gxFalse;

}

gxBool gxUtil::GetFileNameWithoutExt( gxChar *in , gxChar *out)
{
	//拡張子をカットする

	for(int i=strlen(in);i>0;i--)
	{
		if(in[i]=='.')
		{
			sprintf(out,"%s",in);
			out[i] = 0x00;
			return gxTrue;
		}
	}

	out[0] = 0x00;
	return gxFalse;
}




int gxGetHomingDirection(Sint32 my_angle , Sint32 target_angle)
{
	return (Sint32)( gxGetHomingDirection(Float32(my_angle) , (Float32)target_angle ) );
}

Float32 gxGetHomingDirection(Float32 my_angle,Float32 target_angle)
{
	//自分の向いている方向に対して
	//ターゲット角度を－１８０～０～１８０の角度で返す
	Float32 sabun;

	while(my_angle>360){
		my_angle-=360;
	}
	while(my_angle<0){
		my_angle+=360;
	}
	while(target_angle>360){
		target_angle-=360;
	}
	while(target_angle<0){
		target_angle+=360;
	}

	sabun = target_angle-my_angle;

	if(sabun<-180){
		sabun = 360+sabun;
	}else if(sabun>180){
		sabun = (180-(sabun-180))*-1;
	}

	return sabun;
}


CircleCircle::CircleCircle()
{

	m_Circle[0].x = 0;
	m_Circle[0].y = 0;
	m_Circle[0].r = 1;

	m_Circle[1].x = 0;
	m_Circle[1].y = 0;
	m_Circle[1].r = 1;

	m_Near.x = 0;
	m_Near.y = 0;

	m_Kouten[0].x = 0;
	m_Kouten[0].y = 0;

	m_Kouten[1].x = 0;
	m_Kouten[1].y = 0;

	m_sNearest = -1;
}


CircleCircle::~CircleCircle()
{


}


Sint32 CircleCircle::Calc()
{
	//-------------------------------------
	//交点を計算
	//-------------------------------------

	Float32	rksq, rlsq, xlk, ylk, distsq, delrsq, sumrsq, root, dstinv,
			scl, xfac, yfac, x, y, x1, y1, x2, y2, sqdst1, sqdst2;
	Float32 accy = 1.0f;//E-15;

    rksq = pow(m_Circle[0].r, 2.0f);
    rlsq = pow(m_Circle[1].r, 2.0f);

    xlk = m_Circle[1].x - m_Circle[0].x;
    ylk = m_Circle[1].y - m_Circle[0].y;

    distsq = pow(xlk, 2.0f) + pow(ylk, 2.0f);
    
    if(distsq < accy)
	{
        return (0);	//同じだった？
    }
	else
	{
        delrsq = rlsq - rksq;
        sumrsq = rksq + rlsq;
        root = 2.0f*sumrsq*distsq - pow(distsq, 2.f) - pow(delrsq, 2.f);
        
        if (root < -accy)
		{
	        return (0);	//同じだった？
        }
		else
		{
            dstinv = 0.5f / distsq;
            scl = 0.5f - delrsq*dstinv;
            x = xlk*scl + m_Circle[0].x;
            y = ylk*scl + m_Circle[0].y;
            
            if (root < accy)
			{
			   /* 2つの円は接する。*/
				m_Kouten[0].x = (Sint32)x;
				m_Kouten[0].y = (Sint32)y;
				m_sNearest = 0;
                return (1);
            }
			else
			{
                root = dstinv * sqrt(root);
                xfac = xlk * root;
                yfac = ylk * root;
                x1 = x - yfac;
                y1 = y + xfac;
                x2 = x + yfac;
                y2 = y - xfac;
            }
         }
    }

	//交点が２つある場合

    sqdst1 = pow((m_Near.x-x1), 2.0f) + pow((m_Near.y-y1), 2.0f);	//交点との距離１
    sqdst2 = pow((m_Near.x-x2), 2.0f) + pow((m_Near.y-y2), 2.0f);	//交点との距離２

    if (sqdst1 <= sqdst2)
	{
		m_Kouten[0].x = (Sint32)x1;
		m_Kouten[0].y = (Sint32)y1;

		m_Kouten[1].x = (Sint32)x2;
		m_Kouten[1].y = (Sint32)y2;

		m_sNearest = 0;
    }
	else
	{
		m_Kouten[0].x = (Sint32)x2;
		m_Kouten[0].y = (Sint32)y2;

		m_Kouten[1].x = (Sint32)x1;
		m_Kouten[1].y = (Sint32)y1;

		m_sNearest = 1;
	}

    return (2);
}


void gxUtil::RotationPoint( gxPoint *pPoint, Float32 fRot )
{
	Float32 fSin = gxLib::Sin(fRot);
	Float32 fCos = gxLib::Cos(fRot);

	Float32 fPx = (fCos * pPoint->x) - (fSin * pPoint->y);
	Float32 fPy = (fSin * pPoint->x) + (fCos * pPoint->y);

	pPoint->x = fPx;
	pPoint->y = fPy;
}

void gxUtil::StrUpr( gxChar* pStr )
{
	Uint32 len = strlen(pStr);

	for( Sint32 ii=0; ii<len; ii++ )
	{
		pStr[ii] = toupper( pStr[ii] );
	}
}



void gxUtil::MemSet( void* pMem ,Uint8 val , Uint32 sz )
{
	Uint8 *pMem8 = (Uint8*)pMem;

	for( Sint32 ii=0; ii<sz; ii++ )
	{
		pMem8[ii] = val;
	}
}


void gxUtil::MemCpy(void* pDst , void* pSrc , Uint32 sz )
{
	Uint8 *pMemSrc = (Uint8*)pSrc;
	Uint8 *pMemDst = (Uint8*)pDst;

	for( Sint32 ii=0; ii<sz; ii++ )
	{
		pMemDst[ii] = pMemSrc[ii];
	}
}


//多角形を描画します
Uint32 gxUtil::DrawPolygon(
		Sint32 num,
		gxVector2D *pXY,
		gxVector2D *pUV,
		Uint32 *argb,
		Sint32 prio,
		Uint32 atr ,
		Sint32 page,
		gxBool bFill )
{

	//三角形分割

	//

	return 0;
}



Uint32 gxUtil::PutFreeSprite(
	Sint32 x,Sint32 y,Sint32 prio,
	gxVector2D *pXY,
	Sint32 tpg,
	Sint32 u,Sint32 v , Sint32 w , Sint32 h,
	Sint32 divNum,
	Uint32 atr,
	Uint32  argb,
	Float32 fRot,
	Float32 fx,
	Float32 fy
	)
{
	gxPoint pos[4];

	Sint32 div = divNum+1;

	if( div < 2  ) div = 2;
	if( div > 16 ) div = 16;

	gxBool bDebug = gxFalse;
	Uint32 debARGB = 0xff00ff00;

	for( Sint32 ii=0; ii<4; ii++ )
	{
		pos[ii].x = pXY[ii].x;
		pos[ii].y = pXY[ii].y;

		pos[ii].x *= fx;
		pos[ii].y *= fy;

		gxUtil::RotationPoint( &pos[ii] , fRot );

		if( bDebug )
		{
			gxLib::DrawBox( pos[ii].x -3 +x, pos[ii].y -3+y , pos[ii].x +3 +x, pos[ii].y +3 +y, PRIORITY_MAX,gxTrue , ATR_DFLT , 0xffffffff );
		}
	}

	Sint32 max = div*div;

	gxPoint* pMat = new gxPoint[max];
	gxPoint* pTx  = new gxPoint[max];

	//四辺の分割座標を確定

	for( Sint32 ii=0;ii<div;ii++ )
	{
		pMat[ ii ].x = pos[0].x + 1.0f * ii * ( pos[1].x - pos[0].x ) /( div-1 );
		pMat[ ii ].y = pos[0].y + 1.0f * ii * ( pos[1].y - pos[0].y ) /( div-1 );

		pMat[ (div-1)*div+ii ].x = pos[3].x + 1.0f * ii * ( pos[2].x - pos[3].x ) /( div-1 );
		pMat[ (div-1)*div+ii ].y = pos[3].y + 1.0f * ii * ( pos[2].y - pos[3].y ) /( div-1 );

		pMat[ ii*div ].x = pos[0].x + 1.0f * ii * ( pos[3].x - pos[0].x ) /( div -1 );
		pMat[ ii*div ].y = pos[0].y + 1.0f * ii * ( pos[3].y - pos[0].y ) /( div -1 );

		pMat[ ii*div+(div-1) ].x = pos[1].x + 1.0f * ii * ( pos[2].x - pos[1].x ) /( div -1 );
		pMat[ ii*div+(div-1) ].y = pos[1].y + 1.0f * ii * ( pos[2].y - pos[1].y ) /( div -1 );

		if( bDebug )
		{
			gxLib::DrawPoint( pMat[ ii             ].x+x , pMat[ ii                ].y+y , prio+1 ,ATR_DFLT , debARGB );
			gxLib::DrawPoint( pMat[ (div-1)*div+ii ].x+x , pMat[(div - 1)*div + ii ].y+y , prio+1 ,ATR_DFLT , debARGB );
			gxLib::DrawPoint( pMat[ ii*div         ].x+x , pMat[ ii*div ].y+y            , prio+1 ,ATR_DFLT , debARGB );
			gxLib::DrawPoint( pMat[ ii*div+(div-1) ].x+x , pMat[ ii*div+(div-1) ].y+y    , prio+1 ,ATR_DFLT , debARGB );
		}
	}

	//中の補完座標を策定

	for( Sint32 ii=1;ii<div-1;ii++ )
	{
		for( Sint32 xx=1;xx<div-1;xx++ )
		{
			Float32 x1 = pMat[ ii*div+0 ].x;
			Float32 y1 = pMat[ ii*div+0 ].y;
			Float32 x2 = pMat[ ii*div+(div-1) ].x;
			Float32 y2 = pMat[ ii*div+(div-1) ].y;

			pMat[ ii*div+xx ].x = x1 + xx * (x2-x1)/(div-1);
			pMat[ ii*div+xx ].y = y1 + xx * (y2-y1)/(div-1);

			if( bDebug )
			{
				gxLib::DrawPoint( pMat[ ii*div+xx ].x+x , pMat[ ii*div+xx ].y+y , prio+1 ,ATR_DFLT , debARGB );
			}
		}
	}

	for( Sint32 yy=0;yy<div;yy++)
	{
		for( Sint32 xx=0;xx<div;xx++)
		{
			pTx[ yy*div + xx ].x = u + 1.0f * xx *  w  / (div-1);
			pTx[ yy*div + xx ].y = v + 1.0f * yy *  h  / (div-1);
		}
	}


	for( Sint32 yy=0;yy<div-1;yy++)
	{
		for( Sint32 xx=0;xx<div-1;xx++)
		{
			Sint32 p1,p2,p3,p4;
			p1 = yy*div + xx;
			p2 = yy*div + xx+1;
			p3 = yy*div + xx+1+div;
			p4 = (yy+1)*div + xx;


			if( bDebug )
			{
				gxLib::DrawTriangle(
						pMat[ p1 ].x+x , pMat[ p1 ].y+y ,
						pMat[ p2 ].x+x , pMat[ p2 ].y+y ,
						pMat[ p4 ].x+x , pMat[ p4 ].y+y ,
						prio+1 , gxFalse,ATR_DFLT , debARGB );

				gxLib::DrawTriangle(
						pMat[ p2 ].x+x , pMat[ p2 ].y+y ,
						pMat[ p4 ].x+x , pMat[ p4 ].y+y ,
						pMat[ p3 ].x+x , pMat[ p3 ].y+y ,
						prio+1 ,gxFalse,ATR_DFLT , debARGB );
			}

			gxLib::PutTriangle(
					pMat[ p1 ].x+x , pMat[ p1 ].y+y ,	pTx[p1].x , pTx[p1].y,
					pMat[ p2 ].x+x , pMat[ p2 ].y+y ,	pTx[p2].x , pTx[p2].y,
					pMat[ p4 ].x+x , pMat[ p4 ].y+y ,	pTx[p4].x , pTx[p4].y,
					tpg,
					prio ,atr , argb );

			gxLib::PutTriangle(
					pMat[ p2 ].x+x , pMat[ p2 ].y+y ,	pTx[p2].x , pTx[p2].y,
					pMat[ p4 ].x+x , pMat[ p4 ].y+y ,	pTx[p4].x , pTx[p4].y,
					pMat[ p3 ].x+x , pMat[ p3 ].y+y ,	pTx[p3].x , pTx[p3].y,
					tpg,
					prio ,atr , argb );

		}
	}


	SAFE_DELETES( pMat );
	SAFE_DELETES( pTx );

	return 0;
}


Uint32 gxUtil::GetMemoryRemain( Uint32* uUse, Uint32* uTotal , Uint32* uMax )
{
	//メモリ残量を調べる

	Uint32 _uUse,_uMax,_uTotal;

	CGameGirl::GetInstance()->GetMemoryRemain( &_uUse , &_uTotal , &_uMax );

	*uUse   = _uUse / 1024;
	*uMax   = _uMax / 1024;
	*uTotal = _uTotal / 1024;

	return _uUse / 1024;
}


gxBool gxUtil::IsOverProc()
{
	return CGameGirl::GetInstance()->IsSyoriochi();
}



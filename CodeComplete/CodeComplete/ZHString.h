#ifndef _ZHSTRING_201310251709_
#define _ZHSTRING_201310251709_


namespace ZHStringLib
{
#define _MAX_LEN_ 10000

    
}
namespace ZHStringLib
{
    int ahtoi(char *s);
    int whtoi(wchar_t *s);
	//get one line from string
	wchar_t* getlineW(wchar_t *src, wchar_t *des);
	/**��des��ȡ��div�ָ�֮ǰ���ַ���,����div֮���ַ���ָ��*/
	wchar_t* getstringW(wchar_t *src, wchar_t *des,const wchar_t div);
	/**wchar_t[] to int*/
	int wcs2int(const wchar_t *src);
	/**int to wchar_t[]*/
	int int2wcs(int inputInt, wchar_t* wcsRet,int Bit = 10);
	/**delete all wchar_t c in string s, return count of deleted char.*/
	int squeezeW(wchar_t *s, wchar_t c);
	/**delete all wchar_t which des contained in src.*/
	int squeezeW(wchar_t *src, wchar_t *des);
	/**reverse string,which should end by EOF*/
	void reverseStrW(wchar_t *src);
	/**ת��ΪСд*/
	inline wchar_t lowerW(wchar_t src)
	{
			return ((src >= L'A') && (src <= L'Z')) ? src + L'a' - L'A' : src;
	}
	/**ת��Ϊ��д*/
	inline wchar_t uperW(wchar_t src) {
		return ((src >= L'a') && (src <= L'z')) ? src + L'A' - L'a' : src;
	}
	/**��չa-cΪabc*/
	int expandW(const wchar_t* src, wchar_t* des);
	/**ɾ��β���Ŀո񡢻س����Ʊ��(' ','\n','\t','\r')*/
	int trimW(wchar_t *lpRet);
	/**�����ַ���InputStr���Ƿ�ÿ���ַ������ַ�������UnionsStr��*/
	int IsWhatW(const wchar_t* InputStr,const wchar_t* UnionsStr);
}
#endif


















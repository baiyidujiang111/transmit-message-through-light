#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"code.h"
#include"FileConvert.h"
#include<algorithm>

using namespace std;
using namespace cv;

int main()
{
	cout << "This is the encode program." << endl;
	cout << "Please enter the command" << endl;
	char encodetake[20];//���ڽ����ַ���encode
	char filename[20];//����������ļ�����
	char outvideo[20];//�����Ƶ������
	unsigned int videolength_ms;//��Ƶ����
	cin >> encodetake >> filename >> outvideo >> videolength_ms;
	double times;
	times = videolength_ms / 1000.0;
	unsigned long size;
	unsigned int fps = 25;
	unsigned int bitofeachpicture = 1018;
	size = (int)(times * fps * bitofeachpicture);

	unsigned char* bytes = NULL;

	FileConvert converter;
	converter.GenerateRandFile(filename, size);
	bytes = converter.FileToByte(filename, &size);//��ȡ�ļ����ɵ�Byte����

	Code::Main(bytes, size, "imageSrc\\", "png", 1);
	converter.PicTransToVideo(fps,outvideo);
}
#include "qtdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;

void qtdecode::mydecode()
{
	int indexptr = 0;//用于存储目的数组的当前下标
	Decode dec;
	string path = "imageSrc\\";
	FileConvert converter;
	Mat img;
	int numOfPic = converter.GetFilesNumber(path);
	char filename[10];
	sprintf_s(filename, "%05d.png", numOfPic);//最后一张
	img = imread(path + filename);//最后张图片单独处理
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type);//为了获取最后一张的长度
	int total_length = (numOfPic - 1) * 1054 + length;
	//Mat image;//源图片
	Mat dst;//操作对象
	int i, j;
	char outfilename[] = "out.bin";
	int count=0;//图像文件计数
	char fileName[128];

	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//如果只有单张图
	{
		output = tmp;//length为总文件的有效数据长度
	}//正常退出
	else //if (type == BEGIN)//多张图片的第一张图片
	{
		//length为总文件的长度
		int tmplen = MAXSIZE;
		output = new unsigned char[total_length];//用于储存数据的数组
		//memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
		//indexptr += tmplen;
		delete[]tmp;
	}//不可能是end和normal了

	int count = 0;
	int tmplen;
	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		//image = imread(fileName);//读入图像；
		if (img.data == NULL) break;
		//dec.findQranchor(image, dst);
		/*for (i = 0; i <= 96; i++)
		{
			for (j = 0; j <= 96; j++)
			{
				output
				fwrite;
			}
		}*/
		unsigned char* tmp = dec.decode(img, tmplen, type);
		if (tmplen > 1054) tmplen = 1054;
		cout << filename << " " << tmplen << endl;
		memcpy(output + indexptr, tmp, tmplen);//复制到最终的数组里
		indexptr += tmplen;
		delete[]tmp;
	}
	converter.ByteToFile(output, outfilename, total_length);

}
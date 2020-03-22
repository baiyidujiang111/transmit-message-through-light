#include "Qrdecode.h"
#include"Decode.h"
#include"FileConvert.h"
#include<iostream>
using namespace std;
using namespace cv;


void Qrdecode::mydecode(char outputname[],char outfilename[],char vfname[])
{
	char fileName[128];
	char tmpName[128];
	int indexptr = 0;//���ڴ洢Ŀ������ĵ�ǰ�±�
	Decode dec;
	string path = "imageOutput\\";
	FileConvert converter;
	converter.VideoTransToPic(outputname);

	vector<Mat> dstTmp;

	/*
	�Ȱ�����ͼƬת��ΪROI
	*/
	int count = 1;
	Mat img;
	Mat dst;
	int foundindex = 85;
	while (true)
	{
		sprintf_s(fileName, "imageOutput\\%05d.png", count++);
		img = imread(fileName);
		if (img.data == NULL) break;
		int found = dec.findQranchor(img, dst);
		if (found == 1)
		{
			foundindex = count - 1;
			dstTmp.push_back(dst);
		}
		else
		{
		}
	}

	//foundindex = 87;
	//����ÿ��ͼƬ�����ͬ�����Ƿ����仯
		//������Ҫ�����ִ��봫��array�У���ȡ��ά������
	vector<unsigned char> valid;
	int currentFlag = 0;
	int originFlag = 0;
	char beginPic[120];
	vector<int> validIndex;
	validIndex.push_back(0);//��0��ͼƬ
	img = dstTmp[0];

	currentFlag = dec.getFlag(img);
	originFlag = currentFlag;
	for (int i = 1; i < foundindex; i++)
	{
		img = dstTmp[i];
		if (img.data == NULL) continue;
		currentFlag = dec.getFlag(img);
		if (currentFlag == 2 && originFlag == 1 || currentFlag == 1 && originFlag == 2)
		{
			if (dec.getRate(img) - 0.51 < 0)
			{
				validIndex.push_back(i);//�±�Ϊi��ͼƬ��Ч
				originFlag = currentFlag;
			}
		}
		else
		{
			continue;
		}

	}



	int numOfPic = validIndex.size();//converter.GetFilesNumber(path);
	cout << "num of true pic " << numOfPic << endl;
	char filename[10];
	img = dstTmp[validIndex[numOfPic-1]];//�����ͼƬ��������
	int length;
	unsigned char* output = NULL;
	int type;
	unsigned char* tmp = dec.decode(img, length, type, valid);//Ϊ�˻�ȡ���һ�ŵĳ���
	int total_length = (numOfPic - 1) * 1018 + length;
	//Mat image;//ԴͼƬ
	int i, j;
	//char outfilename[] =outfile[];
	count = 0;//ͼ���ļ�����


	cout << total_length << endl;
	cout << indexptr;

	if (type == SINGLE)//���ֻ�е���ͼ
	{
		output = tmp;//lengthΪ���ļ�����Ч���ݳ���
	}//�����˳�
	else //if (type == BEGIN)//����ͼƬ�ĵ�һ��ͼƬ
	{
		//lengthΪ���ļ��ĳ���
		int tmplen = MAXSIZE;
		output = new unsigned char[total_length];//���ڴ������ݵ�����
		//memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
		//indexptr += tmplen;
		delete[]tmp;
	}//��������end��normal��

	count = 0;
	int tmplen;
	valid.clear();
	while (true)
	{
		if (count == numOfPic) break;
		img = dstTmp[validIndex[count++]];//����ͼ��
		unsigned char* tmp = dec.decode(img, tmplen, type, valid);
		if (tmplen > 1018) tmplen = 1018;
		memcpy(output + indexptr, tmp, tmplen);//���Ƶ����յ�������
		indexptr += tmplen;
		delete[]tmp;
	}
	while (valid.size() < total_length)
	{
		valid.push_back(0xff);
	}
	converter.ByteToFile(output, outfilename, total_length);
	unsigned char* v = new unsigned char[valid.size() * sizeof(unsigned char)];
	memcpy(v, &valid[0], valid.size() * sizeof(unsigned char));
	converter.ByteToFile(output, outfilename, total_length);

	int total_byte = valid.size();
	int total_bit = valid.size() * 8;
	int giveUp = 0;
	for (int i = 0; i < total_byte; i++)
	{
		if (valid[i] == 0)
		{
			giveUp += 8;
		}
	}

	cout << giveUp << endl; 


	//char vfname[] = "v.bin";
	converter.ByteToFile(v, vfname, total_length);
} 
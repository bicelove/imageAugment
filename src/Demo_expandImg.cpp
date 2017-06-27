#include <stdio.h>  // for snprintf
#include <string>
#include <vector>
#include <iostream>
#include <sstream> // stringstream
#include <fstream> // NOLINT (readability /streams)
#include <utility> // Create key-value pair (there could be not used)
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <unistd.h>
#include <time.h>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "API_colorLevel.h"
#include "API_imgBrightness.h"
#include "API_imgContrast.h"

using namespace std;
using namespace cv;


API_COLORLEVEL api_colorLevel;
API_IMGCONTRAST api_imgcontrast;
API_IMGBRIGHTNESS api_imgbrightness;
string	svPath;

//dirs to save predict patch logo results
int mkDir(
	const string									keyfile)			//[In]: Input dictionary path
{
	string line;
	string IDdict = keyfile + "dirList.txt";

	/**************************************/	
	std::ifstream IDifs(IDdict);
	if(!IDifs){
		cout<<"[Open dictionary failed:]"<<IDdict;
		return -1;
	}
	
	while(getline(IDifs, line)){
		if(line.length() != 0){
			std::string foldname = svPath + line;
			//cout<<foldname<<endl;
			if(access(foldname.c_str(), 0) == -1 && mkdir(foldname.c_str(),0777))
				std::cout<<"create FIRST file bag failed!"<<endl;
		}
	}	
	
	IDifs.close();
	
	return 0;
}

//
int getStringIDFromFilePath(
	const string							imageString,
	string									&imageID,
	string									&rectLabel)
{
	string imageIDTemp;
	int sour_pos, postfix_pos;
	string sour_name;
	int rectLabelPos;
	string rectLabelString, rectLabelNew;

	//
	sour_pos 	= imageString.find_last_of('/');
	//cout<<"sour_pos = "<<sour_pos<<endl;	
	sour_name	= imageString.substr(sour_pos + 1);
	postfix_pos = sour_name.find_last_of('.');
	imageIDTemp = sour_name.erase(postfix_pos, 4);  		

	//labelName
	rectLabelString = imageString.substr(0, sour_pos);	
	rectLabelPos = rectLabelString.find_last_of('/');
	rectLabel = rectLabelString.substr(rectLabelPos + 1);		
	
	//combine the same labels (eg. Armani_1 & Armani_2)
	sour_pos = rectLabel.find_last_of('_');
	rectLabelString = rectLabel.substr(sour_pos + 1);
	if(rectLabelString == "1" || rectLabelString == "2" ){
		rectLabelNew = rectLabel.substr(0, sour_pos);
		rectLabel = rectLabelNew;		
	}

	imageID  = imageIDTemp;

	return 0;
}

//
int saveIMg(
	const Mat									outImg,
	const string								imgPath,
	const string								methodName,
	const int									numth)
{
	int nRet;
	string strImageID, strLabelName;
	char window_Name[500];
	
	nRet = getStringIDFromFilePath(imgPath, strImageID, strLabelName);
	if(nRet != 0){
		cout<<"test data error!"<<imgPath<<endl;
		return -1;
	}
	
	sprintf(window_Name,"%s/%s/%s_%s_%d.jpg", svPath.c_str(), strLabelName.c_str(), 
		strImageID.c_str(), methodName.c_str(), numth);	
	imwrite( window_Name, outImg );

	return 0;
}

//
int imgHorizontalRotary(
	const string								imgPath)
{
	Mat srcImg = imread(imgPath.c_str());
	if(!srcImg.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}
	
	int nRet = 0;		
	int j_r, i_c;
	int srcWidth, srcHeight;
	int numth;

	//
	numth = 1;
	srcWidth = srcImg.cols;
	srcHeight = srcImg.rows;	
	Mat HoriImg = Mat::zeros(srcHeight, srcWidth, CV_8UC3);
	
	//
	for(j_r = 0; j_r < srcHeight; j_r++){		
		auto ptr = srcImg.ptr<uchar>(j_r);  
		auto qtr = HoriImg.ptr<uchar>(j_r);  
		ptr += srcWidth * 3;
		for(i_c = 0; i_c < srcWidth; i_c++)	{	
			qtr[0] = ptr[0]; 
			qtr[1] = ptr[1]; 
			qtr[2] = ptr[2]; 

			ptr -= 3;
			qtr += 3;
		}
	}
	
	/********************** save image ***************************/	
	nRet = saveIMg(HoriImg, imgPath, "hr", numth);
	if(nRet != 0){
		cout<<"Fail to save HorizontalRotary image!"<<endl;
		return -1;
	}	

	return 0;
}

//
int imgBrightness(
	const string								imgPath)
{
	Mat source = imread(imgPath.c_str());
	if(!source.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}

	int nRet;	
	int numth;
	float brightness;
	Mat outImg;
	
	//
	nRet = 0;
	/********************* debug *********************/
	numth = 3;
	brightness = 0.6;

	nRet = api_imgbrightness.imgBrightness(source, brightness, outImg);
	if(nRet != 0){
		cout<<"[InitColorLevelData error!]"<<endl;
		return -1; 
	}

	/********************** save image ***************************/	
	nRet = saveIMg(outImg, imgPath, "bt", numth);
	if(nRet != 0){
		cout<<"Fail to save Brightness image!"<<endl;
		return -1;
	}	

	return 0;
}


//
int imgContrast(
	const string								imgPath)
{
	Mat source = imread(imgPath.c_str());
	if(!source.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}

	int nRet;	
	string strImageID;	
	string strLabelName;

	int numth;
	float contrast;
	Mat outImg;
	
	//
	nRet = 0;
	/********************* debug *********************/
	numth = 1;
	contrast = 0.05;

	nRet = api_imgcontrast.imgContrast(source, contrast, outImg);
	if(nRet != 0){
		cout<<"[InitColorLevelData error!]"<<endl;
		return -1; 
	}

	/********************** save image ***************************/
	nRet = saveIMg(outImg, imgPath, "ct", numth);
	if(nRet != 0){
		cout<<"Fail to save Contrast image!"<<endl;
		return -1;
	}	

	return 0;
}

//
int imgColorLevel(
	const string								imgPath)
{
	//cout<<"imgPath: "<<imgPath<<endl;
	Mat source = imread(imgPath.c_str());
	if(!source.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}

	int nRet;	
	int numth;	
	ColorLevelData clData;

	/********************* debug *********************/
	nRet = 0;
	numth = 1;
	clData.RGB.Shadow = 10;
	clData.RGB.Midtones = 1.2;
	clData.RGB.Highlight = 240;
	clData.RGB.OutShadow = 50;
	clData.RGB.OutHighlight = 200;

	nRet = api_colorLevel.initColorLevelData(&clData);
	if(nRet != 0){
		cout<<"[InitColorLevelData error!]"<<endl;
		return -1; 
	}
	
	//
	Mat dest = Mat::zeros(source.rows, source.cols, CV_8UC3);
	nRet = api_colorLevel.imageColorLevel(source, dest, &clData);
	if(nRet != 0 || !dest.data){
		cout<<"[ImageColorLevel error!]"<<endl;
		return -1; 
	}
	
	/********************** save image ***************************/
	nRet = saveIMg(dest, imgPath, "ct", numth);
	if(nRet != 0){
		cout<<"Fail to save Contrast image!"<<endl;
		return -1;
	}	

	return 0;
}

//
int imgExpand(
	const string						dataList)
{
	int nRet = 0;

	//
	char loadImgPath[1000];
		
	long nCount;
	FILE *fpListFile = 0;
	int index;
	
	/********************************Open Query List*****************************/
	fpListFile = fopen(dataList.c_str(), "r");
	if (!fpListFile){
		cout << "0.can't open " << dataList << endl;
		return -1;
	}
	
	nCount = 0;
	while(EOF != fscanf(fpListFile, "%s", loadImgPath))
	{
		//cout<<"loadImgPath: "<<loadImgPath<<endl;

		nRet = imgBrightness(loadImgPath);
		if(nRet != 0){
			cout<<"Fail to get brightness image!"<<endl;	
		}		

		/*
		nRet = imgColorLevel(loadImgPath);
		if(nRet != 0){
			cout<<"Fail to get colorLevel image!"<<endl;	
		}
		
		nRet = imgContrast(loadImgPath);
		if(nRet != 0){
			cout<<"Fail to get contrast image!"<<endl;	
		}

		nRet = imgHorizontalRotary(loadImgPath);
		if(nRet != 0){
			cout<<"Fail to get HorizontalRotary image!"<<endl;	
		}
		*/

		nCount++;
		if(nCount % 100 == 0)
			printf("Loaded %ld img...\n", nCount);
		}

	return 0;
}

//
int main(int argc, char** argv) {

	const int num_required_args = 3;
	if( argc < num_required_args ){
	    cout<<
	    "This program extracts features of an image and predict its label and score.\n"
	    "Usage: Demo_mainboby szQueryList outputList\n";
	    return 1;
  }	
	
	/***********************************Init**********************************/
	string imgList = argv[1];		
	svPath = argv[2];	
	string keyfilePath = argv[3];
	
	int nRet = 0;		
	/**************************** getAllDataQueryList *************************/
	nRet = mkDir(keyfilePath);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	nRet = imgExpand(imgList);
	if(nRet != 0){
		cout<<"fail to getQueryList!"<<endl;
		return -1;
	}

	//
	api_colorLevel.release();
	api_imgcontrast.release();
	api_imgbrightness.release();
	
	cout<<"deal end!"<<endl;
	
	return 0;
}


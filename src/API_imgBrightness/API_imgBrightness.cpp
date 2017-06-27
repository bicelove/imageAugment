/*
 * =====================================================================================
 *
 *		 filename:	API_imgBrightness.cpp
 *
 *	  description:	adjust image brightness interface 
 *
 *		  version:	1.0
 *		  created:	2017-03-02
 *		 revision:	none
 *		 compiler:	g++
 *
 *		   author:	tutu
 *		  company:	in66.com
 *
 *		copyright:	2017 itugo Inc. All Rights Reserved.
 *		
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <math.h>
#include <highgui.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "opencv2/opencv.hpp"

#include "API_imgBrightness.h"

//
API_IMGBRIGHTNESS::API_IMGBRIGHTNESS()
{
}

//
API_IMGBRIGHTNESS::~API_IMGBRIGHTNESS(void)
{
}

//
int API_IMGBRIGHTNESS::init()	 		
{	
	int nRet = 0;	
	
	return nRet;
}

//
int API_IMGBRIGHTNESS::clamp(
	const int 									x)
{
    if (x > 255)
        return 255;
    if (x < 0)
        return 0;
	
    return x;
}

//
int API_IMGBRIGHTNESS::imgBrightness(
	const Mat									srcImg,
	const float									brightness,
	Mat 										&outImg)
{
	if(!srcImg.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}

	//
	int nRet = 0;
	int row, col;
	int srcWidth, srcHeight;
	int rgbmeans[3];
	double redSum, greenSum, blueSum;
	double total;
	int pixelValue;

	//
	redSum = 0;
	greenSum = 0;
	blueSum = 0;
	//
	srcWidth = srcImg.cols;
	srcHeight = srcImg.rows;	
	total = srcWidth * srcHeight;

	//
	for(row = 0; row < srcHeight; row++){
		auto ptr = srcImg.ptr<uchar>(row);  
		int tr = 0, tg = 0, tb = 0;
		for(col = 0; col < srcWidth; col++){
			tr =  ptr[2];
			tg =  ptr[1];
			tb =  ptr[0];
			
			redSum += tr;
			greenSum += tg;
			blueSum +=tb;

			ptr += 3;
		}
	}
	rgbmeans[0] = (int)(redSum / total);
	rgbmeans[1] = (int)(greenSum / total);
	rgbmeans[2] = (int)(blueSum / total);

	// 
	outImg = srcImg.clone();
	for(row = 0; row < srcHeight; row++) {
		auto ptr = srcImg.ptr<uchar>(row);  
		auto qtr = outImg.ptr<uchar>(row);  
		int tr = 0, tg = 0, tb = 0;
		for(col = 0; col < srcWidth; col++) {
			//
			tr =  ptr[2];
			tg =  ptr[1];
			tb =  ptr[0];	
	        
	        //
	        tr -=rgbmeans[0];
	        tg -=rgbmeans[1];
	        tb -=rgbmeans[2];
	        	        
	        //
	        tr += (int)(rgbmeans[0] * brightness);
	        tg += (int)(rgbmeans[1] * brightness);
	        tb += (int)(rgbmeans[2] * brightness);
	
			qtr[0] = clamp(tb);
			qtr[1] = clamp(tg);
			qtr[2] = clamp(tr);

			ptr += 3;
			qtr += 3;
		}
    }
	
	return nRet;
}

//
int API_IMGBRIGHTNESS::release()
{	
	int nRet = 0;
	
	return nRet;
}


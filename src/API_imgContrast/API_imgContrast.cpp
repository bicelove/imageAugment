/*
 * =====================================================================================
 *
 *		 filename:	API_imgContrast.cpp
 *
 *	  description:	adjust image contrast interface 
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
#include <highgui.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <sstream> 
#include "opencv2/opencv.hpp"

#include "API_imgContrast.h"

//
API_IMGCONTRAST::API_IMGCONTRAST()
{
}

//
API_IMGCONTRAST::~API_IMGCONTRAST(void)
{
}

//
int API_IMGCONTRAST::init()	 		
{	
	int nRet = 0;	
	
	return nRet;
}

//
int API_IMGCONTRAST::clamp(
	const int 									x)
{
    if (x > 255)
        return 255;
    if (x < 0)
        return 0;
	
    return x;
}

//
int API_IMGCONTRAST::imgContrast(
	const Mat									srcImg,
	const float 								contrast,
	Mat 										&outImg)
{
	if(!srcImg.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}

	//
	int nRet = 0;
	int row, col, k;
	int srcWidth, srcHeight;
	int rgbmeans[3];
	double redSum, greenSum, blueSum;
	double total;
	int pixelValue;

	//
	redSum = 0;
	greenSum = 0;
	blueSum = 0;
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
			
			tr =  ptr[2];
			tg =  ptr[1];
			tb =  ptr[0];
        
      		tr -= rgbmeans[0];
       		tg -= rgbmeans[1];
       		tb -= rgbmeans[2];        
        	
      		tr = (int)(tr * contrast);
       		tg = (int)(tg * contrast);
    		tb = (int)(tb * contrast);        
     		
			qtr[0] = clamp(tb);
			qtr[1] = clamp(tg);
			qtr[2] = clamp(tr);
			
			/*
			if(tb < 0 || tb > 255 || tg < 0 || tg > 255 || tr < 0 || tr > 255)
				cout<<"tb = "<<tb<<", tg = "<<tg<<", tr = "<<tr<<endl;
     		*/
     		
			ptr += 3;
			qtr += 3;
		}
    }

	return nRet;
}

//
int API_IMGCONTRAST::release()
{	
	int nRet = 0;
	
	return nRet;
}


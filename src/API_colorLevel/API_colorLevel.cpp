/*
 * =====================================================================================
 *
 *		 filename:	API_colorLevel.cpp
 *
 *	  description:	adjust image color level interface 
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

#include "API_colorLevel.h"

//
API_COLORLEVEL::API_COLORLEVEL()
{
}

//
API_COLORLEVEL::~API_COLORLEVEL(void)
{
}

//
int API_COLORLEVEL::init()	 		
{	
	int nRet = 0;	
	
	return nRet;
}

//
int API_COLORLEVEL::initColorLevelData(
	PColorLevelData 								clData)
{
	int nRet = 0;
	
	PColorLevelItem item = &clData->Blue;
	for (int i = 0; i < 4; i++, item++){
		item->Shadow = item->OutShadow = 0;
		item->Highlight = item->OutHighlight = 255;
		item->Midtones = 1.0;
	}

	return nRet;
}

//
int API_COLORLEVEL::getDataCopyParams(
	const Mat										source,
	const Mat										dest,
 	int 												&width, 
  	int 												&height)    
{  
	int nRet = 0;
	
 	 width = dest.cols < source.cols? dest.cols : source.cols;   
  	height = dest.rows < source.rows? dest.rows : source.rows;  

	return nRet;
}  

//
bool API_COLORLEVEL::getColorLevelTable(
	PColorLevelItem 									item, 
	unsigned char* 									clTable)
{
	int diff = (int)(item->Highlight - item->Shadow);
	int outDiff = (int)(item->OutHighlight - item->OutShadow);

	if (!((item->Highlight <= 255 && diff < 255 && diff >= 2) ||
		(item->OutShadow <= 255 && item->OutHighlight <= 255 && outDiff < 255) ||
		(!(item->Midtones > 9.99 && item->Midtones > 0.1) && item->Midtones != 1.0)))
		return false;

	double coef = 255.0 / diff;
	double outCoef = outDiff / 255.0;
	double exponent = 1.0 / item->Midtones;

	for (int i = 0; i < 256; i++){
		int v;
		if (clTable[i] <= (unsigned char)item->Shadow)
			v = 0;
		else{
			v = (int)((clTable[i] - item->Shadow) * coef + 0.5);
			if (v > 255)
				v = 255;
		}		
		v = (int)(pow(v / 255.0, exponent) * 255.0 + 0.5);
		clTable[i] = (unsigned char)(v * outCoef + item->OutShadow + 0.5);
	}
	
	return true;
}

//
bool API_COLORLEVEL::checkColorLevelData(
	PColorLevelData 									clData, 
	unsigned char 									clTables[][256])
{
	bool result = false;
	int i, j;
	for (i = 0; i < 3; i++){
		for (j = 0; j < 256; j++)
			clTables[i][j] = (unsigned char)j;
	}
	
	PColorLevelItem item = &clData->Blue;
	for (i = 0; i < 3; i++, item++){
		if (getColorLevelTable(item, clTables[i]))
			result = true;
	}
	
	for (i = 0; i < 3; i++){
		if (!getColorLevelTable(item, clTables[i]))
			break;
		result = true;
	}
	
	return result;
}

//
int API_COLORLEVEL::imageColorLevel(
	const Mat										source,
	Mat 												&dest,
	PColorLevelData 									clData)
{
	if(!source.data){
		cout<<"[image error!]"<<endl;
		return -1; 
	}

	int nRet;
	int width, height;

	nRet = 0;
	nRet = getDataCopyParams(source, dest, width, height);
	if(nRet != 0){
		cout<<"[GetDataCopyParams error!]"<<endl;
		return -1; 
	}

	unsigned char clTables[3][256];
	if (checkColorLevelData(clData, clTables)){
		for (unsigned int y = 0; y < height; y++){
			auto ptr = source.ptr<uchar>(y);          
    		        auto qtr = dest.ptr<uchar>(y);    
			for (unsigned int x = 0; x < width; x++){
				qtr[0] = clTables[0][ptr[0]];	
				qtr[1] = clTables[1][ptr[1]];
				qtr[2] = clTables[2][ptr[2]];
				
				ptr += 3; 
				qtr += 3; 
			}
		}
	}else{
		dest = source;
    }

	return nRet;
}

//
int API_COLORLEVEL::release()
{	
	int nRet = 0;
	
	return nRet;
}


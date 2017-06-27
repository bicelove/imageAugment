/*
 * =====================================================================================
 *
 *		 filename:	API_colorLevel.h
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
 
#ifndef _API_COLORLEVEL_H
#define _API_COLORLEVEL_H

using namespace std;
using namespace cv;

//
typedef struct
{
	int Shadow;
	float Midtones;
	int Highlight;
	int OutShadow;
	int OutHighlight;
}ColorLevelItem, *PColorLevelItem;

//
typedef struct
{
	ColorLevelItem Blue;
	ColorLevelItem Green;
	ColorLevelItem Red;
	ColorLevelItem RGB;
}ColorLevelData, *PColorLevelData;

class API_COLORLEVEL{
	
	public:
				
		API_COLORLEVEL();
		
		~API_COLORLEVEL(void);
		
		int init();
		
		int initColorLevelData(
			PColorLevelData 								clData);
		
		int imageColorLevel(
			const Mat										source,
			Mat 											&dest,
			PColorLevelData 								clData);
				
		int release();

	private:		
		
		int getDataCopyParams(
			const Mat										source,
			const Mat										dest,
			int 											&width, 
			int 											&height);
		
		bool getColorLevelTable(
			PColorLevelItem 								item, 
			unsigned char*									clTable);
		
		bool checkColorLevelData(
			PColorLevelData 								clData, 
			unsigned char									clTables[][256]);

};

#endif

/*
 * =====================================================================================
 *
 *		 filename:	API_imgBrightness.h
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
 
#ifndef _API_IMGBRIGHTNESS_H
#define _API_IMGBRIGHTNESS_H

using namespace std;
using namespace cv;


class API_IMGBRIGHTNESS{
	
	public:
				
		API_IMGBRIGHTNESS();
		
		~API_IMGBRIGHTNESS(void);
		
		int init();
		
		int imgBrightness(
			const Mat									srcImg,
			const float 								brightness,
			Mat 										&outImg);
				
		int release();

	private:		
		
		int clamp(
			const int 									x);

};

#endif

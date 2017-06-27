/*
 * =====================================================================================
 *
 *		 filename:	API_imgContrast.h
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
 
#ifndef _API_IMGCONTRAST_H
#define _API_IMGCONTRAST_H

using namespace std;
using namespace cv;


class API_IMGCONTRAST{
	
	public:
				
		API_IMGCONTRAST();
		
		~API_IMGCONTRAST(void);
		
		int init();
		
		int imgContrast(
			const Mat										srcImg,
			const float 									contrast,
			Mat 											&outImg);
				
		int release();

	private:		
		
		int clamp(
			const int 										x);

};

#endif

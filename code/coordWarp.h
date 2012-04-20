#ifndef _COORD_WARPING_H
#define _COORD_WARPING_H

//#include "ofxOpenCv.h"
////we use openCV to calculate our transform matrix
#include "ofxCvConstants.h"
//#include "ofxCvContourFinder.h"

class CvMat;
class CvPoint2D32f;

class coordWarping{
	
	
public:
	
	//---------------------------
	coordWarping();
	~coordWarping();
	
	void calculateMatrix(ofVec2f src[4], ofVec2f dst[4]);
	
	ofVec2f transform(float xIn, float yIn);
	ofVec2f inversetransform(float xIn, float yIn);

	ofVec2f transform(ofVec2f p);
	ofVec2f inversetransform(ofVec2f p);
	
	CvMat *translate;
	CvMat *itranslate;
	
protected:
	CvPoint2D32f * cvsrc;
	CvPoint2D32f * cvdst;
	
};

#endif
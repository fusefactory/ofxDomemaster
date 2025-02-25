#pragma once

#include "ofMain.h"

class ofxDomemaster{

	public:
        ofxDomemaster();
		void setup();
		void draw();
        void begin(int i);
        void end(int i);
        void resize(int w, int h);
        void setCameraPosition(float x, float y, float z);
        void setCameraLookAt(float x, float y, float z);
        void setMeshScale(float s);

        static const int renderCount = 5;
    
        vector<ofCamera> renderCamera;
    
    private:
        ofRectangle view;
        vector<ofFbo> renderFbo;
        vector<ofVboMesh> renderMesh;


        ofCamera fisheyeCamera;
        ofRectangle fisheyeView;

        ofImage mask;
    
        int domeWidth;
        int domeHeight;
    
        float meshScale;
        float meshScaleExt;
    
        ofNode cameraNode;  
    
        enum positions{
            bottom,
            front,
            left,
            right,
            top
        };
    
    ofFbo myTestRenderFbo;

};

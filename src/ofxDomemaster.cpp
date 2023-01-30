#include "ofxDomemaster.h"

ofxDomemaster::ofxDomemaster(){
    meshScaleExt = 1;
}

//--------------------------------------------------------------
void ofxDomemaster::setup(){    
    domeWidth = 1280;
    domeHeight = 1280;
        
    for (int i=0; i<renderCount; i++){
        ofCamera c;
        c.setNearClip(.001);
        renderCamera.push_back(c);
        ofFbo f;
        ofFbo::Settings settings;
        settings.internalformat = GL_RGBA;
        settings.width = domeWidth;
        settings.height = domeHeight;
        f.allocate(settings);
        renderFbo.push_back(f);
        ofVboMesh m;
        renderMesh.push_back(m);
    }
    
    resize(domeWidth, domeHeight);
    

    renderMesh[bottom].load("domemaster/bottom.ply");
    renderMesh[front].load("domemaster/front.ply");
    renderMesh[left].load("domemaster/left.ply");
    renderMesh[right].load("domemaster/right.ply");
    renderMesh[top].load("domemaster/top.ply");

    // mask
    mask.load("domemaster/mask.png");
    mask.setUseTexture(true);
    
    
}

//--------------------------------------------------------------
void ofxDomemaster::begin(int i){
    renderFbo[i].begin();
    //ofClear(0);
    ofSetColor(0, 0, 0, 15);
    ofDrawRectangle(0, 0, renderFbo[i].getWidth(), renderFbo[i].getHeight());
    renderCamera[i].begin(view);
    
}

//--------------------------------------------------------------
void ofxDomemaster::end(int i){
    renderFbo[i].end();
    renderCamera[i].end();

}

//--------------------------------------------------------------
void ofxDomemaster::draw(){
    fisheyeCamera.enableOrtho();

    fisheyeCamera.begin(fisheyeView);

        ofEnableNormalizedTexCoords();

        for (int i=0; i<renderCount; i++){
            renderFbo[i].getTexture().bind();
            ofPushMatrix();
            ofTranslate(-domeWidth/2,-domeHeight/2,0);
            ofRotate(90,1,0,0);
            ofRotate(180,0,0,1);
            ofRotate(180,0,1,0);
            ofScale(meshScale, meshScale, meshScale);
            ofSetColor(255);
            renderMesh[i].drawFaces();
            ofPopMatrix();
            renderFbo[i].getTexture().unbind();
        }

        ofDisableNormalizedTexCoords();

    fisheyeCamera.end();

//    fisheyeCamera.rotateDeg(0.2, ofVec3f(0,0,1));
    
    fisheyeCamera.draw();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mask.draw(0, 0, domeWidth, domeHeight);
    

}

//--------------------------------------------------------------
void ofxDomemaster::resize(int w, int h){
    domeWidth = w;
    domeHeight = h;

    // set view for cube cameras
    view.setWidth(domeWidth);
    view.setHeight(domeHeight);
    
    // cube camera
    renderCamera[bottom].setOrientation(ofVec3f(90,0,0));
    renderCamera[bottom].setFov(90);
    
    renderCamera[front].setOrientation(ofVec3f(0,0,0));
    renderCamera[front].setFov(90);
    
    renderCamera[left].setOrientation(ofVec3f(0,90,0));
    renderCamera[left].setFov(90);
    
    renderCamera[right].setOrientation(ofVec3f(0,-90,0));
    renderCamera[right].setFov(90);
    
    renderCamera[top].setOrientation(ofVec3f(-90,0,0));
    renderCamera[top].setFov(90);

    
    // cube camera fbos
    for (int i=0; i<renderCount; i++){
        renderFbo[i].setUseTexture(true);
        renderFbo[i].allocate(domeWidth, domeHeight, GL_RGBA32F);
        renderFbo[i].begin();
        //ofClear(0);
        renderFbo[i].end();
    }
    
    // fisheye domemaster
    fisheyeView.setWidth(domeWidth);
    fisheyeView.setHeight(domeHeight);
    fisheyeCamera.setPosition(-domeWidth/2, -domeHeight/2, 10);
    meshScale = domeWidth*meshScaleExt;
}

void ofxDomemaster::setCameraPosition(float x, float y, float z){
    for (int i=0; i<renderCount; i++){
        renderCamera[i].setPosition(x,y,z);
    }
}

void ofxDomemaster::setCameraLookAt(float x, float y, float z) {
 
    renderCamera[front].lookAt(ofVec3f(x, y, z));
    ofQuaternion q = renderCamera[front].getOrientationQuat();
    ofQuaternion q1;
    q1.makeRotate(-90, 1, 0, 0);
    renderCamera[top].setOrientation(q1 * q);
    q1.makeRotate(90, 1, 0, 0);
    renderCamera[bottom].setOrientation(q1 * q);
    q1.makeRotate(90, 0, 1, 0);
    renderCamera[left].setOrientation(q1 * q);
    q1.makeRotate(-90, 0, 1, 0);
    renderCamera[right].setOrientation(q1 * q);
}

void ofxDomemaster::setMeshScale(float s){
    meshScaleExt = s;
    meshScale = domeWidth*meshScaleExt;
}


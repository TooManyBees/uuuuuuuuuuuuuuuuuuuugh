#pragma once

#include "ofMain.h"
#include "wires.h"
#include "ofxImageSequenceRecorder.h"
#include "OpenNI.h"
#include "NiTE.h"
#include "OniManager.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void reloadShaders();
		bool shadersDirty = false;

		OniManager oniManager;
		ofImage depthFrame;

		glm::vec2 focus;
		Wires wires;
		void drawWires();

		void drawNoise();
		ofShader noiseShader;
		ofShader noiseShader2;

		ofxImageSequenceRecorder recorder;
		bool recording = false;
		ofFbo canvas;
};

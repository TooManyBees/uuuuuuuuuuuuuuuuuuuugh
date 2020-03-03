#include "ofMain.h"

class Wires {
public:
	void drawAround(glm::vec2 focus, size_t n) {
		for (size_t i = 0; i < n; i++) {
			drawRandomLine(focus);
		}
	}

	void drawAroundMulti(vector<glm::vec2> &const focuses, size_t n) {
		for (size_t i = 0; i < n; i++) {
			glm::vec2 p(ofRandomWidth(), ofRandomHeight());
			glm::vec2 slope = p * focuses.size();
			for (auto &focus : focuses) {
				slope -= focus;
			}

			{ float temp = slope.x; slope.x = -slope.y; slope.y = temp; }
			float add = ofRandomf() * 0.8; // FIXME: use perlin noise to get jitter
			float newSlope = (slope.y + add) / (slope.x + add);

			glm::vec2 p1, p2;
			lineThroughPoint(p, newSlope, p1, p2);
			ofDrawLine(p1, p2);
		}
	}

private:
	void lineThroughPoint(const glm::vec2 &i, float slope, glm::vec2 &p1, glm::vec2 &p2) {
		float x1 = i.x - i.y / slope;
		float x2 = i.x + (ofGetHeight() - i.y) / slope;
		p1.x = x1; p1.y = 0;
		p2.x = x2; p2.y = ofGetHeight();
	}

	void drawRandomLine(const glm::vec2 &focus) {
		glm::vec2 p(ofRandomWidth(), ofRandomHeight());
		glm::vec2 slope = glm::normalize(p - focus);
		{ float temp = slope.x; slope.x = -slope.y; slope.y = temp; }
		float add = ofRandomf() * 0.8;
		float newSlope = (slope.y + add) / (slope.x + add);


		glm::vec2 p1, p2;
		lineThroughPoint(p, newSlope, p1, p2);
		ofDrawLine(p1, p2);
	}
};

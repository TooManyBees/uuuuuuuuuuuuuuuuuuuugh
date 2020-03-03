#include "ofMain.h"

class Wires {
public:
	void drawAround(glm::vec2 focus, size_t n);

	void drawAroundMulti(vector<glm::vec2> &const focuses, size_t n);
};

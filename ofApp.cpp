#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->cap.open("D:\\MP4\\Pexels Videos 2441168.mp4");
	this->cap_size = cv::Size(720, 1080);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	float radius = (this->cap_size.width / PI);
	int deg_span = 1;
	int y_span = 120;
	for (int y = 0; y < this->cap_size.height; y += y_span) {

		ofMesh mesh, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		for (int deg = 0; deg < 360; deg += deg_span) {

			int index = mesh.getNumVertices();

			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), y - this->cap_size.height * 0.5));
			vertices.push_back(glm::vec3(radius * cos((deg + deg_span) * DEG_TO_RAD), radius * sin((deg + deg_span) * DEG_TO_RAD), y - this->cap_size.height * 0.5));
			vertices.push_back(glm::vec3(radius * cos((deg + deg_span) * DEG_TO_RAD), radius * sin((deg + deg_span) * DEG_TO_RAD), y + y_span - this->cap_size.height * 0.5));
			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), y + y_span - this->cap_size.height * 0.5));

			mesh.addVertices(vertices);
			frame.addVertices(vertices);

			int x = ofMap(deg, 0, 360, 0, this->cap_size.width);
			int next_x = ofMap(deg + deg_span, 0, 360, 0, this->cap_size.width);

			mesh.addTexCoord(glm::vec3(x, y, 0));
			mesh.addTexCoord(glm::vec3(next_x, y, 0));
			mesh.addTexCoord(glm::vec3(next_x, y + y_span, 0));
			mesh.addTexCoord(glm::vec3(x, y + y_span, 0));

			mesh.addIndex(index + 0); mesh.addIndex(index + 1); mesh.addIndex(index + 2);
			mesh.addIndex(index + 0); mesh.addIndex(index + 2); mesh.addIndex(index + 3);

			frame.addIndex(index + 0); frame.addIndex(index + 1);
			frame.addIndex(index + 2); frame.addIndex(index + 3);
		}
		this->mesh_list.push_back(mesh);
		this->frame_list.push_back(frame);
	}
	
	this->cap.set(cv::CAP_PROP_POS_FRAMES, 45 * 24);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(cv::CAP_PROP_POS_FRAMES, 45 * 24);
		return;
	}

	cv::cvtColor(src, src, cv::COLOR_BGR2RGB);

	cv::Mat(src, cv::Rect(600, 0, this->cap_size.width, this->cap_size.height)).copyTo(this->frame);
	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(90);

	for (int x = -600; x <= 600; x += 600) {

		ofPushMatrix();
		ofTranslate(x, 0, 0);

		for (int i = 0; i < this->mesh_list.size(); i++) {

			float z = (this->mesh_list[i].getVertex(0).z + this->mesh_list[i].getVertex(2).z) / 2;

			ofPushMatrix();
			ofRotateZ(ofRandom(1000) - ofGetFrameNum() * ofRandom(5, 10));

			this->image.bind();
			ofSetColor(255);
			this->mesh_list[i].draw();
			this->image.unbind();

			ofSetColor(239);
			this->frame_list[i].drawWireframe();

			ofPopMatrix();
		}

		ofPopMatrix();
	}


	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}

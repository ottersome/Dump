//
//
// Student Name:
// Student ID:
// Student Email Address:
//
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2018/10/08
//
// Select and move characters
#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_
 
#include "BaseApplication.h"
#include "selection_rectangle.h"
#include "sound.h"

using namespace Ogre;
#define MOTION_TYPE_NONE 0
#define MOTION_TYPE_IDLE 1
#define MOTION_TYPE_WALK 2

class BasicTutorial_00 : public BaseApplication
{
private:
	void initParticleSystemForExplosion();
	void setOffParticleSystem(Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos); 


	//myvars
	int const static lightRotRadius = 400;
	int const static robotSpeed = 80;
	int const static stopDistanceThreshold = 10;
	int const static robotHeight = 5;
	int const static robotRadius = 20;
	int robotCount;
	SOUND *mSound;
	PlaneBoundedVolumeListSceneQuery* mVolQuery;
	SelectionRectangle* mSelRect;
	SceneNode *mParticleNode;
	std::map<std::string,AnimationState*> animStates;
	std::map<std::string,SceneNode*> selectedRobots;
	std::map<std::string,Ogre::Entity *> robotEntities;
	Vector3 spherePos;
	Plane plano;
	float lightAngle;
	Light *lighto;
	bool pressedAlready;
	bool robotSelected;
	bool startWalking;
	Vector3 walkingTarget;

	Ogre::Camera * mCamera2;
	ParticleSystem* elParticulo;


	virtual void createCamera();
	virtual void createViewports();
	//My stuff
	Real left, right, top , bottom;
	virtual void mouseMovedRect();
	virtual void mousePressedRect();
	virtual void mouseReleasedRect();
	virtual void onSpotClickRect();
	virtual void getWalkingTarget();
	virtual void moveRobot(Vector3,const FrameEvent&);
	virtual void animsAddTime(const FrameEvent&);
	virtual void selectedRobotsWalkNow();

	virtual void setUpVars();
	virtual void setUpSceneBases();//such as light fog etc
	virtual void setUpNodesNEnts();
	virtual void createRobotCircles(bool,int);
	virtual void moveLight(const FrameEvent&);
	virtual void checkRectSect(const FrameEvent&);
	virtual void checkForCollision(const FrameEvent&);
	virtual void stopRoboto(std::map<std::string,SceneNode*>::iterator);
	virtual void stopAllRobots();
	//END my stuff

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
virtual bool frameStarted(const FrameEvent &evt);
public:
	BasicTutorial_00(void);
	virtual void createScene(void);
};
 
#endif // #ifndef __BasicTutorial_00_h_
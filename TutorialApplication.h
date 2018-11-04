// Student Name: Luis Fernando Garcia
// Student ID: 0616115
// Student Email Address: luisgarcia26284@gmail.com
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


/**
*	3D Game Programming.\n
 *  Name: Luis Fernando Garcia \n
 *	ID:	0616115\n
 *	Email:	luisgarcia26284@gmail.com\n
 *	This is an Assignment of 3D Game Programming\n
 */
class BasicTutorial_00 : public BaseApplication
{
private:
	void initParticleSystemForExplosion();
	void setOffParticleSystem(Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos); 
public:
	//vafs
	int const static lightRotRadius = 300;
	int const static lightHeight = 400;
	int const static robotSpeed = 80;
	int const static stopDistanceThreshold = 10;
	int const static robotHeight = 5;
	int const static robotRadius = 20;
	int const static sphereRadius = 70;
	int const static animSpeed = 2.5;

	SOUND *mSound;
protected:
	//myvars
	int robotCount;
	PlaneBoundedVolumeListSceneQuery* mVolQuery;
	SelectionRectangle* mSelRect;
	SceneNode *mParticleNode;
	std::map<std::string,AnimationState*> animStates;
	std::map<std::string,SceneNode*> selectedRobots;
	std::map<std::string,SceneNode*> robotMap;
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

	/*! \brief Creates Camera that will be used for the viewports*/	
	virtual void createCamera();
	/*! \brief Creates two viewports, the main one and the one on the top right coner*/	
	virtual void createViewports();
	//My stuff
	Real left, right, top , bottom;
	/*! \brief Updates Top and Left coordinates when mouse is moved as long as flag pressedAlready is set to true*/	
	virtual void mouseMovedRect();
	/*! \brief Called when on left click. Updates all corners with mouse press location and sets them as such for the selection rectangle*/	
	virtual void mousePressedRect();
	/*! \brief Sets mouse pressed flags to false and sets selection rectangle visibility to false. If released on same cordinates
	*		as pressed then it runs the logic for single click. Else the logic for selection rectangle is run 
	*/	
	virtual void mouseReleasedRect();
	/*! \brief Follow from mouseReleasedRect() if release coordinates are equal to press coordinates
	*		Checks Ray Scene Query and selects the first movable object that is identified as a robot.
	*/	
	virtual void onSpotClickRect();
	/*! \brief Run on right click.
	*		Again iterates through movable objects until plane is found and then the collision coordinates are returned.
	*/	
	virtual void getWalkingTarget();
	/*! \brief Called everyframe.
	*		Iterates through selected robots and moves them towards their target position.
	*/	
	virtual void moveRobot(Vector3,const FrameEvent&);
	/*! \brief Called everyframe.
	*		Adds time to the loop animation of the robots
	*/	
	virtual void animsAddTime(const FrameEvent&);
	/*! \brief Starts walking animation on selected robots
	*/	
	virtual void selectedRobotsWalkNow();

	/*! \brief Called at scene creation. Sets up necessary variables
	*/	
	virtual void setUpVars();
	/*! \brief Called at scene creation. Sets up Lights and Shadows
	*/
	virtual void setUpSceneBases();//such as light fog etc
	/*! \brief Called at scene creation. Sets up Nodes and Entities
	*/
	virtual void setUpNodesNEnts();
	/*! \brief Creates The robot circles, the first parameter when set to true will scale the first robot to (2,2,2)
	*		Second parameter is the radius of the drawn circle
	*/
	virtual void createRobotCircles(bool,int);
	/*! \brief Called every frame. Will move the light above the scene
	*/
	virtual void moveLight(const FrameEvent&);
	/*! \brief Check Collision for exactly two objects
	*/
	virtual bool checkIndividualCollision(const Vector3 &, const Vector3 &,bool);
	/*! \brief Replaced
	*/	
	virtual void checkRectSect(const FrameEvent&);
	/*! \brief Checks Robots Collisions
	*/
	virtual void checkForCollision(SceneNode *);
	/*! \brief Stops movement of the walking robot selected and stops its animations back to idle
	*/
	virtual void stopRoboto(std::map<std::string,SceneNode*>::iterator);
	/*! \brief Iterates through moving robots and stops them all 
	*/
	virtual void stopAllRobots();
	//ENDo my stuff

	/*! \brief Responds to mouse movement*/	
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	/*! \brief Responds to pressing mouse*/	
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	/*! \brief Responds to releaseing mouse click*/	
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	/*! \brief Called every frame, everything related to movement will be changed here*/	
	virtual bool frameStarted(const FrameEvent &evt);
public:
	BasicTutorial_00(void);
/*! \brief This function is called at the creation of the scene, its main purpose its to abstract the creation of circles of robots of given radius circle_radius..
 *         Its setOneBig boolean parameter indicates wether the first spawned robot should be scaled.
 *
 */	
	virtual void createScene(void);
};
 
#endif // #ifndef __BasicTutorial_00_h_
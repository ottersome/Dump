
//
// Student Name: Luis Fernando Garcia
// Student ID: 0616115	
// Student Email Address: luisgarcia26284@gmail.com
//
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
//
// Date: 2018/10/08
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include "OgreMath.h"
using namespace Ogre;
using namespace std;
BasicTutorial_00::BasicTutorial_00(void) {
	lightAngle = 0;
	mSound = new SOUND();
	mSound->init();
	
}

void BasicTutorial_00::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCamera"); //Aready created

	mCamera->setPosition(Ogre::Vector3(0,700,800));
	mCamera->lookAt(Ogre::Vector3(0,0,0));

	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

	//Second camera
	mCamera2 = mSceneMgr->createCamera("CameraDos");
	mCamera2->setPosition(0,1400,0);
	mCamera2->lookAt(0,0,0.0001);

	mCamera->setNearClipDistance(5);

}

void BasicTutorial_00::createViewports(void)
{
	//Already done for us
	Ogre::Viewport* vp;
	// Create one viewport, entire window
	vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	//Create second Viewport
	Ogre::Viewport* vp2;
	vp2 = mWindow->addViewport(mCamera2,1,0.75,0,.25,.25);
	mCamera2->setAspectRatio(Real(4*(vp2->getActualWidth()/vp2->getActualHeight())));
	vp2->setBackgroundColour(Ogre::ColourValue(1,1,0));
	vp2->setOverlaysEnabled(false);
	vp2->setSkiesEnabled(false);
}

void BasicTutorial_00::createRobotCircles(bool setOneBig,int circle_radius){
	//create cubed-circle
	//lets start with 10 cubes
	//set center
	int numCubes = 22;
	int L = 255;
	float pi = 3.141592654;
	//TODO this is creating some weird not complete circle so just polish that up 
	for(int i=0;i<numCubes;i++){//this will look weird when we make the penguin "lookat" the camera
		//im going to hanle de x and z coordinates, given an x 
		//y^2 = r^2 - x^2
		Entity *beam = mSceneMgr->createEntity("robot.mesh");
		beam->setCastShadows(true);
		AxisAlignedBox bb = beam->getBoundingBox();
		int cubesize = bb.getMaximum().x-bb.getMinimum().x;
		int x = 0,y=0,z=-125;

		//int zpos = sqrt(pow(radius,2.0)-pow(i,2.0));
		SceneNode *cubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNo"+Ogre::StringConverter::toString(robotCount++));
		cubeNode->attachObject(beam);
		double fx = i/(double)(numCubes-1);
		//double h = (1+sin(fx*pi*4))*50;
		double x1 =	circle_radius*cos(fx*pi*2);
		double z1 = circle_radius*sin(fx*pi*2);
		float unitf = 1.0/cubesize/numCubes*L*0.8;
		//beNode->scale(unitf,unitf,unitf);
		if(i == 0 && setOneBig == true){
			elParticulo = mSceneMgr->createParticleSystem("elParticulo","Examples/GreenyNimbus");
			cubeNode->scale(2,2,2);
			cubeNode->attachObject(elParticulo);
		}
		cubeNode->setPosition(x1,robotHeight,z1);
		cubeNode->lookAt(Vector3(0,0,0),Node::TS_WORLD);
		//cubeNode->lookAt(spherePos,Node::TS_LOCAL);
		cubeNode->yaw(Radian(pi/2),Node::TS_LOCAL);
		//cubeNode->setR
		//set idle animation
		//orientate properly
		Vector3 localY = cubeNode->getOrientation() * Vector3::UNIT_Y;
		Quaternion quat = localY.getRotationTo(Vector3::UNIT_Y);                        
		cubeNode->rotate(quat, Node::TS_PARENT);

		robotMap.insert(pair<std::string,SceneNode*>(cubeNode->getName(),cubeNode));
		AnimationState * animState = beam->getAnimationState("Idle");
		animState->setLoop(true);
		animState->setEnabled(true);
		animStates.insert(pair<std::string,AnimationState*>(cubeNode->getName(),animState));
		robotEntities.insert(pair<std::string,Entity*>(cubeNode->getName(),beam));
	}
}
/// Brief description.
/** Detailed description. */
void BasicTutorial_00::setUpSceneBases(){
	mSceneMgr->setAmbientLight( ColourValue( 0.4, 0.4, 0.4 ) ); 
	//Now we create the shadow
	mSceneMgr->setFog(FOG_LINEAR,ColourValue(1,1,1),0,1400,1600);
	//Set Skybox
	mSceneMgr->setSkyBox(true,"Examples/SpaceSkyBox");
	//setShadow
	mSceneMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
	
	//Create rotating light
	lighto = mSceneMgr->createLight("rotLight");
	lighto->setType(Light::LT_POINT);
	lighto->setPosition(Vector3(300,20,0));
	lighto->setDiffuseColour(ColourValue(0.8,0.8,0.8));
	lighto->setSpecularColour(ColourValue(0.8,0.8,0.8));
}
void BasicTutorial_00::setUpNodesNEnts(){
	//create Plane
	plano = Plane(Vector3::UNIT_Y,0);
	Ogre::MeshManager::getSingleton().createPlane("planoplane",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,plano,1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
	Entity * planoEnt = mSceneMgr->createEntity("planoEnt","planoplane");
	planoEnt->setMaterialName("Examples/Rockwall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planoEnt);
	planoEnt->setCastShadows(false);

	//create sphere
	Entity * sphereEnto = mSceneMgr->createEntity("sphere.mesh");
	SceneNode * sphere = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sphere->scale(0.68,0.68,0.68);//to make it itnot 70 radius 
	sphere->attachObject(sphereEnto);
	spherePos = sphere->getPosition();
	/*
	AxisAlignedBox boxo = sphereEnto->getBoundingBox();
	int size = boxo.getMaximum().x - boxo.getMinimum().x;
	Ogre::LogManager::getSingletonPtr()->logMessage("*************That size is : "+	StringConverter::toString(size));
	*/
	

}
void BasicTutorial_00::setUpVars(){
	//bool
	pressedAlready = false;
	robotSelected = false;
	robotCount = 0;
	startWalking = false;

	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);

	mSelRect = new SelectionRectangle("selRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelRect);	
	mSelRect->setLightMask(0);

	mSelRect->setCastShadows(false);
}
void BasicTutorial_00::createScene(void) 
{
	setUpVars();
	setUpSceneBases();
	setUpNodesNEnts();
	createRobotCircles(false,200);
	createRobotCircles(true,300);

}
bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{
	mouseMovedRect();
	return BaseApplication::mouseMoved( arg);

}

//void BasicTutorial_00::singleClickSelect ( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
//{
//}

//void BasicTutorial_00::volumeSelect(const OIS::MouseEvent &arg, OIS::MouseButtonID id )
//{
//}

bool BasicTutorial_00::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{

	Ogre::LogManager::getSingletonPtr()->logMessage("Actually realeased my boi************************");
	if(id==OIS::MB_Left){
		mouseReleasedRect();
	}
	if (id == OIS::MB_Right && !selectedRobots.empty()) {
		getWalkingTarget();//i might want to change this to simply return the vector and pass this function as parameter in moveRobot function
		//now enable walking animations
		startWalking = true;
		selectedRobotsWalkNow();

		//return BaseApplication::mousePressed( arg, id ); i dont htink that should be tehre
	}
	return BaseApplication::mouseReleased( arg, id );
}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(id==OIS::MB_Left){
		//make sure when pressing left no robots are walking 
		if(!selectedRobots.empty())
			stopAllRobots();
		mousePressedRect();
	}
	return BaseApplication::mousePressed( arg, id );
}

bool BasicTutorial_00::frameStarted(const FrameEvent &evt)
{
	//rotate the light
	moveLight(evt);
	checkRectSect(evt);
	moveRobot(walkingTarget,evt);//will check inside if there are any robots to be moved
	animsAddTime(evt);
    return BaseApplication::frameStarted(evt);
	

}

void BasicTutorial_00::initParticleSystemForExplosion() {
	mParticleNode = static_cast<SceneNode*>(
		mSceneMgr->getRootSceneNode()->createChild());
	ParticleSystem* p = mSceneMgr->createParticleSystem(
		"explosion", "Examples/GreenyNimbus");

	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(false);
	mParticleNode->attachObject(p);
	mParticleNode->setPosition(Vector3::ZERO);
	mParticleNode->setVisible(false);
}

void BasicTutorial_00::setOffParticleSystem(
	Ogre::SceneNode *fNode, const Ogre::String &pname, const Ogre::Vector3 &pos) 
{
	fNode->setPosition(pos);
	fNode->setVisible(true);
	MovableObject *s = fNode->getAttachedObject(pname);
	ParticleSystem* p =static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);
	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(true);
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
void BasicTutorial_00::moveLight(const FrameEvent &evt){
	int speed = 5;
	lightAngle += speed * evt.timeSinceLastFrame;

	Vector3 newOffset(Ogre::Math::Cos(lightAngle),10,Ogre::Math::Sin(lightAngle)); 
	newOffset *= lightRotRadius;
	newOffset.y = lightHeight;
	lighto->setPosition(newOffset);
}
void BasicTutorial_00::checkRectSect(const FrameEvent &){/*
	Ray mRay = mTrayMgr->getCursorRay(mCamera);
	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
	int left = scn.x;
	int top = scn.y;

	left = 

	mSelRect->setVisible(true);*/
}
void BasicTutorial_00::mousePressedRect(){
	Ogre::LogManager::getSingletonPtr()->logMessage("*********Mouse pressed");
	pressedAlready = true;
	Ray mRay = mTrayMgr->getCursorRay(mCamera);
	Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());

	right = left = scn.x;
	bottom = top = scn.y;

	mSelRect->setCorners(left,top,right,bottom);
	
	//donezo	
}
void BasicTutorial_00::mouseMovedRect(){
	if(pressedAlready == true){
		Ray mRay = mTrayMgr->getCursorRay(mCamera);
		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		left = scn.x;
		top = scn.y;

		mSelRect->setCorners(left,top,right,bottom);
		mSelRect->setVisible(true);
	}
}
void BasicTutorial_00::mouseReleasedRect(){
	//unset some stuff
	pressedAlready = false;
	mSelRect->setVisible(false);
	if(left > right)
		std::swap(left,right);
	if(top > bottom)
		std::swap(top,bottom);
	if ((left==right && top==bottom)) {
	//call single click action 
		onSpotClickRect();
		return;
		
	}else{
	}

	/* CEGUI
	Ray topLeft = mCamera->getCameraToViewportRay(left, top);
	Ray topRight = mCamera->getCameraToViewportRay(right, top);
	Ray bottomLeft = mCamera->getCameraToViewportRay(left, bottom);
	Ray bottomRight = mCamera->getCameraToViewportRay(right, bottom);
	*/
	

	Ogre::LogManager::getSingletonPtr()->logMessage("*************Mouse Relseased");
	//USING mTrayMgr=============

	Real nleft = left;
	Real nright = right;
	Real ntop = 1+top;
	Real nbottom = 1+bottom;
	cout << "nleft:" << nleft << endl;
	cout << "nright:" << nright << endl;
	cout << "nbottom:" << nbottom << endl;
	cout << "ntop:" << ntop << endl;
	Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, ntop));
	Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
	Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, nbottom));
	Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));
	//End Using mTrayMgr=============

	// The plane faces the counter clockwise position.
	PlaneBoundedVolume vol;
	int np = 100;
	vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), 			bottomRight.getPoint(3)));         // front plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), 	topRight.getPoint(np)));         // top plane
	vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), 	topLeft.getPoint(np)));       // left plane
	vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), 	bottomLeft.getPoint(np)));   // bottom plane
	vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), 	bottomRight.getPoint(np)));     // right plane 

	PlaneBoundedVolumeList volList;
	volList.push_back(vol);
	mVolQuery->setVolumes(volList);

	SceneQueryResult result = mVolQuery->execute();

	SceneQueryResultMovableList::iterator itr = result.movables.begin();


	// Get the results, set the camera height
	// We are interested in the first intersection. It is ok to traverse all the results.
	for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
	{
		if ((*itr)->getParentSceneNode()->getName().substr(0,7) == "RobotNo")
		{
			SceneNode * collisionWith = (*itr)->getParentSceneNode();
			bool flgShow = collisionWith->getShowBoundingBox();
			collisionWith->showBoundingBox(!flgShow);

			if(selectedRobots.find(collisionWith->getName()) == selectedRobots.end())
				selectedRobots.insert(pair<std::string,SceneNode*>(collisionWith->getName(),collisionWith));
			else
				selectedRobots.erase(collisionWith->getName());
		}
	}

}
void BasicTutorial_00::onSpotClickRect(){
	Ogre::LogManager::getSingletonPtr()->logMessage("******************We are on spot click rect");
	Ray mRay = mTrayMgr->getCursorRay(mCamera);
	RaySceneQuery *rsq = mSceneMgr->createRayQuery(Ray());

	rsq->setSortByDistance(true);
	rsq->setRay(mRay);
	RaySceneQueryResult &result = rsq->execute();
	RaySceneQueryResult::iterator itr = result.begin();
	
	for(itr = result.begin(); itr != result.end();itr++){
		//Ogre::LogManager::getSingletonPtr()->logMessage("Before: "+itr->movable->getParentSceneNode()->getName()+"\nInspecting :"+itr->movable->getParentSceneNode()->getName().substr(0,7));
		if(itr->movable->getParentSceneNode()->getName().substr(0,7) == "RobotNo"){
			SceneNode * collisionWith  = itr->movable->getParentSceneNode();
		
			bool flgShow = collisionWith->getShowBoundingBox();
			collisionWith->showBoundingBox(!flgShow);

			
			if(selectedRobots.find(collisionWith->getName()) == selectedRobots.end())//if empty
				selectedRobots.insert(pair<std::string,SceneNode*>(collisionWith->getName(),collisionWith));
			else
				selectedRobots.erase(collisionWith->getName());
			/*
			mCurrentObject = itr->movable->getParentSceneNode();
			bool flgShow = mCurrentObject->getShowBoundingBox();
			//since when showBoundingBox is false it means we are turning it on and also that we are moving the robot then 
			robotSelected = !flgShow;
			mCurrentObject->showBoundingBox(!flgShow);*/

			break;
		}
	}
}
void BasicTutorial_00::getWalkingTarget(){
	Ray mRay = mTrayMgr->getCursorRay(mCamera);
	RaySceneQuery *rsq = mSceneMgr->createRayQuery(Ray());

	rsq->setSortByDistance(true);
	rsq->setRay(mRay);
	RaySceneQueryResult result = rsq->execute();
	RaySceneQueryResult::iterator itr = result.begin();
	for(itr;itr != result.end();itr++){
		//Ogre::LogManager::getSingletonPtr()->logMessage("Collision with : "+itr->movable->getParentSceneNode()->getName());
		if(itr->movable){
			walkingTarget = mRay.getPoint(itr->distance);
			walkingTarget.y = robotHeight;
			Ogre::LogManager::getSingletonPtr()->logMessage("We have gotten a new walking target : "+Ogre::StringConverter::toString(walkingTarget.x)+","+Ogre::StringConverter::toString(walkingTarget.y));
			break;//we just need the first one
		}
	}

}
void BasicTutorial_00::moveRobot(Vector3 moveTo,const FrameEvent & evt){
	//we are using mCurrent object
	//check if there is a robot selected 

	if(selectedRobots.size() != 0 && startWalking == true){
		//Ogre::LogManager::getSingletonPtr()->logMessage("About to move robot, these are the amount of selectedRobots: "+ Ogre::StringConverter::toString(selectedRobots.size()));
		std::map<string,SceneNode*>::iterator it;
		for(it = selectedRobots.begin();it!=selectedRobots.end();it++){
			//it->second->setPosition(moveTo);
			Vector3 curPos = it->second->getPosition();
			float distance = curPos.distance(walkingTarget);
			if(distance > stopDistanceThreshold){
				Vector3 direction = (walkingTarget - curPos);
				direction.normalise();
				direction.y = 0;
				//Ogre::LogManager::getSingletonPtr()->logMessage("Our normalized vector is x:"+Ogre::StringConverter::toString(direction.x)+" y:"+Ogre::StringConverter::toString(direction.y)+" z:"+Ogre::StringConverter::toString(direction.z));
			
				it->second->lookAt(walkingTarget,Node::TS_WORLD);
				
				//orientate properly
				Vector3 localY = it->second->getOrientation() * Vector3::UNIT_Y;
				Quaternion quat = localY.getRotationTo(Vector3::UNIT_Y);                        
				it->second->rotate(quat, Node::TS_PARENT);
				
				it->second->yaw(Radian(3.1415/2),Node::TS_LOCAL);
				curPos += robotSpeed*direction*evt.timeSinceLastFrame;
				//check for collision before assigning new pos
				it->second->setPosition(curPos);
				checkForCollision(it->second);
			}else{	
				it->second->setPosition(walkingTarget);
				stopRoboto(it);
				if(selectedRobots.empty()){
					startWalking = false;
					Ogre::LogManager::getSingletonPtr()->logMessage("Got empty");
				}

				

			}
		}
	}
}
void BasicTutorial_00::animsAddTime(const FrameEvent & evt){
	std::map<std::string,AnimationState*>::iterator it = animStates.begin();
	while(it != animStates.end()){
		if(it->second->getEnabled() == true){
			it->second->addTime(evt.timeSinceLastFrame*animSpeed);
		}
		it++;
	}
}
void BasicTutorial_00::selectedRobotsWalkNow(){//Basically only does animation 
	Ogre::LogManager::getSingletonPtr()->logMessage("We are loading "+Ogre::StringConverter::toString(selectedRobots.size())+" Walking animation states");
	std::map<string,SceneNode*>::iterator it = selectedRobots.begin();
	while(it != selectedRobots.end()){
		//now for every selected robot we change the animation state to walking
		string name = it->first;
		Ogre::LogManager::getSingletonPtr()->logMessage("Before Enntitiy");
		Entity * curEnt = (Entity*)it->second->getAttachedObject(0);
		Ogre::LogManager::getSingletonPtr()->logMessage("Pulling Walking from "+ it->second->getName());
		Ogre::LogManager::getSingletonPtr()->logMessage("After Enntitiy");
		animStates.find(name)->second = curEnt->getAnimationState("Walk");
		animStates.find(name)->second->setEnabled(true);
		animStates.find(name)->second->setLoop(true);

		Ogre::LogManager::getSingletonPtr()->logMessage("Still good it seems");
		it++;
	}
}
bool BasicTutorial_00::checkIndividualCollision(const Vector3 & a, const Vector3 & b,bool bIsSphere){
	
	float distance = sqrt(((a.x - b.x) * (a.x-b.x))+ ((a.z - b.z) * (a.z-b.z)));
	int otherRadius = (bIsSphere == true)? sphereRadius : robotRadius;
	if(distance < robotRadius + otherRadius)
	{
    //AABBs are overlapping
		//Ogre::LogManager::getSingletonPtr()->logMessage("Colliding Perrito");
		return true;
	}
	return false;
}
void BasicTutorial_00::checkForCollision(SceneNode * mRobot){
	//we shall go through the robot scenenodes and check if 
	Vector3 robotPos = mRobot->getPosition();
	Vector3 otherRobotPos;
	bool flago=false;
	float dx, dz,radisum,length,unitx,unitz;
	if(!selectedRobots.empty()){
		std::map<std::string,SceneNode*>::iterator it = robotMap.begin();
		while(it != robotMap.end()){	
			otherRobotPos = it->second->getPosition();
			flago = checkIndividualCollision(robotPos,it->second->getPosition(),false);
			if(flago){
				//check collision point  
				dx = otherRobotPos.x - robotPos.x;
				dz = otherRobotPos.z - robotPos.z;
				radisum = robotRadius + robotRadius;
				length = sqrt(dx*dx+dz*dz);
				length = (length == 0)?1:length;
				unitx = dx/length;
				unitz = dz/length;

				otherRobotPos.x = robotPos.x + (radisum +2)*unitx;
				otherRobotPos.z = robotPos.z + (radisum +2)*unitz;
				it->second->setPosition(otherRobotPos);
			}
			//check with sphere
			it++;
			}
	}
	flago = checkIndividualCollision(robotPos,spherePos,true);
	if(flago){
			//check collision point  
			dx = robotPos.x - spherePos.x;
			dz = robotPos.z - spherePos.z;
			radisum = robotRadius + sphereRadius;
			length = sqrt(dx*dx+dz*dz);
			length = (length == 0)?1:length;
			unitx = dx/length;
			unitz = dz/length;

			robotPos.x = spherePos.x + (radisum +2)*unitx;
			robotPos.z = spherePos.z + (radisum +2)*unitz;
			mRobot->setPosition(robotPos);
	}
	
}

void BasicTutorial_00::stopRoboto(std::map<std::string,SceneNode*>::iterator it){
				Entity * ento = robotEntities.find(it->first)->second;//(Entity*) it->second->getAttachedObject(0);
				AnimationState* as = animStates.find(it->first)->second;

				it->second->showBoundingBox(false);
				as->setLoop(false);
				as->setEnabled(false);
				as =  ento->getAnimationState("Idle");
				as->setEnabled(true);
				as->setLoop(true);
				animStates.find(it->first)->second = as;
				selectedRobots.erase(it);
				

				mSound->play();
				
}
void BasicTutorial_00::stopAllRobots(){
	startWalking = false;
	std::map<std::string,SceneNode*>::iterator it = selectedRobots.begin();
	while(it != selectedRobots.end()){
		stopRoboto(it);
		it++;
	}
}
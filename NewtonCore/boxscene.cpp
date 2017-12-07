#include "boxscene.h"


void BoxScene::buildWorld(){
    b2Vec2 gravity;
    gravity.Set(0.0f,-9.0f);
    b2BodyDef* bodydef = new b2BodyDef();
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetWarmStarting(true);
}


void BoxScene::buildWalls(){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();

    b2BodyDef wallDef ;
    wallDef.position.Set(0,0);
    wallBody = world->CreateBody(&wallDef);
    b2EdgeShape wallShape;

    // bottom wall
    wallShape.Set(b2Vec2(0,0+wallLineOffset), b2Vec2(s.width/pixelToMeter,0+wallLineOffset));
    wallBody->CreateFixture(&wallShape,0);

    // upper wall
    wallShape.Set(b2Vec2(0,s.height/pixelToMeter-wallLineOffset),b2Vec2(s.width/pixelToMeter,s.height/pixelToMeter-wallLineOffset));
    wallBody->CreateFixture(&wallShape,0);

    // left wall
    wallShape.Set(b2Vec2(0+wallLineOffset,s.height/pixelToMeter),b2Vec2(0+wallLineOffset,0));
    wallBody->CreateFixture(&wallShape,0);

    // right wall
    wallShape.Set(b2Vec2(s.width/pixelToMeter-wallLineOffset,s.height/pixelToMeter),b2Vec2(s.width/pixelToMeter-wallLineOffset,0));
    wallBody->CreateFixture(&wallShape,0);
}

// hard coded the shape so far, maybe combined with other file later
void BoxScene::buildRect(){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();
    b2BodyDef rectDef;
    rectDef.type = b2_staticBody;
    b2Body* rect = world->CreateBody(&rectDef);
}

void BoxScene::buildCirc(){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();
    b2BodyDef circDef;
    circDef.type = b2_staticBody;
    b2Body* circ = world->CreateBody(&circDef);
}

void BoxScene::buildDynamicRect(){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();
    b2BodyDef rectDynamicDef;   // predefine the shape here to be type
    rectDynamicDef.type = b2_dynamicBody;  // can be either static or dynamic
    b2Body* dynamicRect = world->CreateBody(&rectDynamicDef);
}

void BoxScene::buildDynamicCirc(){
    CCSize s = CCDirector::sharedDirector()->getVisibleSize();
    b2BodyDef circDynamicDef;
    circDynamicDef.type = b2_dynamicBody;
    b2Body* dynamicCirc = world->CreateBody(&circDynamicDef);
}


bool BoxScene::RunScene(){
    if (!CCLayer::init())
        return false;

    // launch the methods to build shapes.
    wallLineOffset = 0.5;
    this->buildWorld();
    this->buildWalls();
    this->buildRect();
    this->buildCirc();
    this->buildDynamicRect();
    this->buildDynamicCirc();

    return true;
}


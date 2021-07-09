#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<chrono>
#include "engine.h"
#include "structs.h"
#include "globals.h"
#include "click.h"
#include "clickfunctions.h"
#include "graph.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int GAME_LENGTH = 5*60;

struct Labels {
	vector<char *> *genders = nullptr;
	vector<affectionTrait *> *romance = nullptr;
	vector<affectionTrait *> *sexuality = nullptr;
};

/*
	TODO: Maybe make this function load data from a level file?
	TODO: Divide this function up to different functions that each return a vector/list_intializer,
	that we finally put into the CreateClickableArea function and into the objects vector.
	
	NOTE: Currently just a collection of used game objects collected in a singular
	space for ease of use
*/
bool loadLevel(vector<GameObject *>* objects, Media* media,
	const char *path, CurrentClick* cc, Labels* labels, MouseStruct* mouse,
	Graph<GameObject *, int> *pG){

#pragma region ROOMS
	/* ROOMS */
	vector<Room *> rooms;
	// Proper Room 1
	roomPopupPars* rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r1 = new Room(2418, 520, media->images.at(13), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r1;
	rooms.push_back(r1);

	// Proper Room 2
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r2 = new Room(3161, 650, media->images.at(14), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r2;
	rooms.push_back(r2);

	// Proper Room 3
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r3 = new Room(6314, 721, media->images.at(15), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r3;
	rooms.push_back(r3);

	// Proper Room 4
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r4 = new Room(1614, 674, media->images.at(16), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r4;
	rooms.push_back(r4);

	// Proper Room 5
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r5 = new Room(3203, 797, media->images.at(17), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r5;
	rooms.push_back(r5);

	// Proper Room 6
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r6 = new Room(4775, 1270, media->images.at(18), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r6;
	rooms.push_back(r6);

	// Proper Room 7
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r7 = new Room(5491, 1110, media->images.at(19), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r7;
	rooms.push_back(r7);

	// Proper Room 8
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r8 = new Room(6244, 1094, media->images.at(20), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r8;
	rooms.push_back(r8);

	// Proper Room 9
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r9 = new Room(1001, 1092, media->images.at(21), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r9;
	rooms.push_back(r9);

	// Proper Room 10
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r10 = new Room(2591, 1219, media->images.at(22), roomPopup, (void *)(rPup), BRIDGE);
	rPup->room = r10;
	rooms.push_back(r10);

	// Proper Room 11
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r11 = new Room(5225, 1531, media->images.at(23), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r11;
	rooms.push_back(r11);

	// Proper Room 12
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r12 = new Room(6244, 1295, media->images.at(24), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r12;
	rooms.push_back(r12);

	// Proper Room 13
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r13 = new Room(1305, 1442, media->images.at(25), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r13;
	rooms.push_back(r13);

	// Proper Room 14
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r14 = new Room(3186, 1839, media->images.at(26), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r14;
	rooms.push_back(r14);

	// Proper Room 15
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r15 = new Room(6244, 1707, media->images.at(27), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r15;
	rooms.push_back(r15);

	// Proper Room 16
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r16 = new Room(6387, 1709, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r16;
	rooms.push_back(r16);

	// Proper Room 17
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r17 = new Room(6599, 1709, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r17;
	rooms.push_back(r17);

	// Proper Room 18
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r18 = new Room(6811, 1709, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r18;
	rooms.push_back(r18);

	// Proper Room 19
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r19 = new Room(7021, 1709, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r19;
	rooms.push_back(r19);

	// Proper Room 20
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r20 = new Room(1373, 2574, media->images.at(29), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r20;
	rooms.push_back(r20);

	// Proper Room 21
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r21 = new Room(2900, 2521, media->images.at(30), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r21;
	rooms.push_back(r21);

	// Proper Room 22
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r22 = new Room(6387, 1929, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r22;
	rooms.push_back(r22);

	// Proper Room 23
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r23 = new Room(6599, 1929, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r23;
	rooms.push_back(r23);

	// Proper Room 24
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r24 = new Room(6811, 1929, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r24;
	rooms.push_back(r24);

	// Proper Room 25
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r25 = new Room(7021, 1929, media->images.at(28), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r25;
	rooms.push_back(r25);

	// Proper Room 26
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r26 = new Room(2900, 2696, media->images.at(31), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r26;
	rooms.push_back(r26);

	// Proper Room 27
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r27 = new Room(4385, 2780, media->images.at(32), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r27;
	rooms.push_back(r27);

	// Proper Room 28
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r28 = new Room(2900, 2867, media->images.at(33), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r28;
	rooms.push_back(r28);

	// Proper Room 29
	rPup = new roomPopupPars(popPopUpPars{cc, false, objects, media});
	Room *r29 = new Room(1231, 3084, media->images.at(34), roomPopup, (void *)(rPup),
										BRIDGE);
	rPup->room = r29;
	rooms.push_back(r29);
#pragma	endregion
#pragma region DOORS
	/* DOORS */
	vector<IsClickable *> doors;
	// Room 1 down door
	DoorClickPars* DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r1D = new Door(2507, 624 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r1D;
	pG->addNode(r1D); //Add the door to the pathfinder graph
	doors.push_back(r1D);

	// Room 2 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r2Dl = new Door(3161, 738 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r2Dl;
	pG->addNode(r2Dl); //Add the door to the pathfinder graph
	doors.push_back(r2Dl);

	// Room 2 bot door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r2Db = new Door(5553, 742 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r2Db;
	pG->addNode(r2Db); //Add the door to the pathfinder graph
	doors.push_back(r2Db);

	// Room 2 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r2Dr = new Door(5747, 738 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r2Dr;
	pG->addNode(r2Dr); //Add the door to the pathfinder graph
	doors.push_back(r2Dr);

	// Room 3 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r3D = new Door(6314, 869 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r3D;
	pG->addNode(r3D); //Add the door to the pathfinder graph
	pG->addEdge(r3D, r2Dr, 1);
	pG->addEdge(r2Dr, r3D, 1);
	doors.push_back(r3D);

	// Room 4 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r4Dl = new Door(1614, 869 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r4Dl;
	pG->addNode(r4Dl); //Add the door to the pathfinder graph
	doors.push_back(r4Dl);

	// Room 4 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r4Dt = new Door(2507, 871 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r4Dt;
	r4Dt->bottom = r1D; //Indicate that this is a vertical door
	pG->addNode(r4Dt); //Add the door to the pathfinder graph
	pG->addEdge(r4Dt, r1D, 1);
	pG->addEdge(r1D, r4Dt, 1);
	r1D->bottom = r4Dt; //Indicate that this is a vertical door
	doors.push_back(r4Dt);

	// Room 4 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r4Dr = new Door(2903, 871 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r4Dr;
	pG->addNode(r4Dr); //Add the door to the pathfinder graph
	pG->addEdge(r4Dr, r2Dl, 1);
	pG->addEdge(r2Dl, r4Dr, 1);
	doors.push_back(r4Dr);

	// Room 5 top left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r5Dtl = new Door(3203, 871 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r5Dtl;
	pG->addNode(r5Dtl); //Add the door to the pathfinder graph
	pG->addEdge(r5Dtl, r4Dr, 1);
	pG->addEdge(r4Dr, r5Dtl, 1);
	pG->addEdge(r5Dtl, r2Dl, 1);
	pG->addEdge(r2Dl, r5Dtl, 1);
	doors.push_back(r5Dtl);

	// Room 5 bottom left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r5Dbl = new Door(3509, 1468 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r5Dbl;
	pG->addNode(r5Dbl); //Add the door to the pathfinder graph
	doors.push_back(r5Dbl);

	// Room 5 top right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r5Dtr = new Door(4227, 1211 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r5Dtr;
	pG->addNode(r5Dtr); //Add the door to the pathfinder graph
	doors.push_back(r5Dtr);

	// Room 5 bottom right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r5Dbr = new Door(4450, 1671 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r5Dbr;
	pG->addNode(r5Dbr); //Add the door to the pathfinder graph
	doors.push_back(r5Dbr);

	// Room 5 bottom bot door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r5Dbb = new Door(4318, 1672 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r5Dbb;
	pG->addNode(r5Dbb); //Add the door to the pathfinder graph
	doors.push_back(r5Dbb);

	//Room 5 Beacon
	GameObject *r5B = new GameObject();
	r5B->x = r5->x + (r5->area.w / 2);
	r5B->y = r5->y + (r5->area.h / 2);
	r5B->image = nullptr;
	pG->addNode(r5B);
	pG->addEdge(r5B, r5Dtr, 1);
	pG->addEdge(r5Dtr, r5B, 1);
	pG->addEdge(r5B, r5Dbr, 1);
	pG->addEdge(r5Dbr, r5B, 1);
	pG->addEdge(r5B, r5Dtl, 1);
	pG->addEdge(r5Dtl, r5B, 1);
	pG->addEdge(r5B, r5Dbl, 1);
	pG->addEdge(r5Dbl, r5B, 1);
	pG->addEdge(r5B, r5Dbb, 1);
	pG->addEdge(r5Dbb, r5B, 1);
	objects->push_back(r5B);
	strcpy(r5B->n, "r5B");

	// Room 6 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r6Dl = new Door(4775, 1426 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r6Dl;
	pG->addNode(r6Dl); //Add the door to the pathfinder graph
	pG->addEdge(r5Dtr, r6Dl, 1);
	pG->addEdge(r6Dl, r5Dtr, 1);
	doors.push_back(r6Dl);

	// Room 6 bot door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r6Db = new Door(4913, 1428 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r6Db;
	pG->addNode(r6Db); //Add the door to the pathfinder graph
	pG->addEdge(r5Dbr, r6Db, 1);
	pG->addEdge(r6Db, r5Dbr, 1);
	doors.push_back(r6Db);

	// Room 6 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r6Dr = new Door(5049, 1428 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r6Dr;
	pG->addNode(r6Dr); //Add the door to the pathfinder graph
	doors.push_back(r6Dr);

	// Room 7 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r7Dl = new Door(5491, 1428 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r7Dl;
	pG->addNode(r7Dl); //Add the door to the pathfinder graph
	pG->addEdge(r6Dr, r7Dl, 1);
	pG->addEdge(r7Dl, r6Dr, 1);
	doors.push_back(r7Dl);

	// Room 7 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r7Dt = new Door(5553, 1428 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r7Dt;
	r7Dt->bottom = r2Db; //Indicate that this is a vertical door
	r2Db->bottom = r7Dt; //Indicate that this is a vertical door
	pG->addNode(r4Dt); //Add the door to the pathfinder graph
	pG->addNode(r7Dt); //Add the door to the pathfinder graph
	pG->addEdge(r2Db, r7Dt, 1);
	pG->addEdge(r7Dt, r2Db, 1);
	doors.push_back(r7Dt);

	// Room 7 bot door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r7Db = new Door(5714, 1428 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r7Db;
	pG->addNode(r7Db); //Add the door to the pathfinder graph
	doors.push_back(r7Db);

	// Room 7 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r7Dr = new Door(6054, 1426 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r7Dr;
	pG->addNode(r7Dr); //Add the door to the pathfinder graph
	doors.push_back(r7Dr);

	// Room 8 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r8D = new Door(6244, 1226 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r8D;
	pG->addNode(r8D); //Add the door to the pathfinder graph
	pG->addEdge(r7Dr, r8D, 1);
	pG->addEdge(r8D, r7Dr, 1);
	doors.push_back(r8D);

	// Room 9 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r9D = new Door(1304, 1242 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r9D;
	pG->addNode(r9D); //Add the door to the pathfinder graph
	pG->addEdge(r4Dl, r9D, 1);
	pG->addEdge(r9D, r4Dl, 1);
	doors.push_back(r9D);

	// Room 10 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r10D = new Door(3006, 1468 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r10D;
	pG->addNode(r10D); //Add the door to the pathfinder graph
	pG->addEdge(r5Dbl, r10D, 1);
	pG->addEdge(r10D, r5Dbl, 1);
	doors.push_back(r10D);

	// Room 11 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r11Dl = new Door(5225, 1671 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r11Dl;
	pG->addNode(r11Dl); //Add the door to the pathfinder graph
	pG->addEdge(r5Dbr, r11Dl, 1);
	pG->addEdge(r11Dl, r5Dbr, 1);
	pG->addEdge(r6Db, r11Dl, 1);
	pG->addEdge(r11Dl, r6Db, 1);
	doors.push_back(r11Dl);


	// Room 11 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r11Dt = new Door(5714, 1672 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r11Dt;
	r11Dt->bottom = r7Db; //Indicate that this is a vertical door
	r7Db->bottom = r11Dt; //Indicate that this is a vertical door
	pG->addNode(r11Dt); //Add the door to the pathfinder graph
	pG->addEdge(r7Db, r11Dt, 1);
	pG->addEdge(r11Dt, r7Db, 1);
	doors.push_back(r11Dt);
	strcpy(r11Dt->n, "r11Dt");

	// Room 11 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r11Dr = new Door(6013, 1672 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r11Dr;
	pG->addNode(r11Dr); //Add the door to the pathfinder graph
	doors.push_back(r11Dr);
	strcpy(r11Dr->n, "r11Dr");

	// Room 12 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r12D = new Door(6244, 1426 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r12D;
	pG->addNode(r12D); //Add the door to the pathfinder graph
	pG->addEdge(r7Dr, r12D, 1);
	pG->addEdge(r12D, r7Dr, 1);
	pG->addEdge(r8D, r12D, 1);
	pG->addEdge(r12D, r8D, 1);
	doors.push_back(r12D);

	// Room 13 bot door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r13Db = new Door(1392, 1584 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r13Db;
	pG->addNode(r13Db); //Add the door to the pathfinder graph
	doors.push_back(r13Db);

	// Room 13 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r13Dt = new Door(2022, 1584 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r13Dt;
	r13Dt->bottom = r9D; //Indicate that this is a vertical door
	pG->addNode(r13Dt); //Add the door to the pathfinder graph
	pG->addEdge(r9D, r13Dt, 1);
	pG->addEdge(r13Dt, r9D, 1);
	pG->addEdge(r4Dl, r13Dt, 1);
	pG->addEdge(r13Dt, r4Dl, 1);
	doors.push_back(r13Dt);

	// Room 13 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r13Dr = new Door(2199, 1580 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r13Dr;
	pG->addNode(r13Dr); //Add the door to the pathfinder graph
	doors.push_back(r13Dr);

	// Room 14 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r14Dl = new Door(3186, 2079 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r14Dl;
	pG->addNode(r14Dl); //Add the door to the pathfinder graph
	pG->addEdge(r13Dr, r14Dl, 1);
	pG->addEdge(r14Dl, r13Dr, 1);
	doors.push_back(r14Dl);

	// Room 14 bot door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r14Db = new Door(3893, 2213 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r14Db;
	pG->addNode(r14Db); //Add the door to the pathfinder graph
	doors.push_back(r14Db);

	// Room 14 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r14Dt = new Door(4318, 2213 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r14Dt;
	r14Dt->bottom = r5Dbb; //Indicate that this is a vertical door
	r5Dbb->bottom = r14Dt; //Indicate that this is a vertical door
	pG->addNode(r14Dt); //Add the door to the pathfinder graph
	pG->addEdge(r5Dbb, r14Dt, 1);
	pG->addEdge(r14Dt, r5Dbb, 1);
	doors.push_back(r14Dt);

	// Room 15 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r15Dl = new Door(6244, 1830 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r15Dl;
	pG->addNode(r15Dl); //Add the door to the pathfinder graph
	pG->addEdge(r11Dr, r15Dl, 1);
	pG->addEdge(r15Dl, r11Dr, 1);
	doors.push_back(r15Dl);
	strcpy(r15Dl->n, "r15Dl");

	// Room 15 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r15Dr = new Door(6366, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r15Dr;
	pG->addNode(r15Dr); //Add the door to the pathfinder graph
	doors.push_back(r15Dr);
	strcpy(r15Dr->n, "r15Dr");

	// Room 16 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r16Dl = new Door(6387, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r16Dl;
	pG->addNode(r16Dl); //Add the door to the pathfinder graph
	pG->addEdge(r15Dr, r16Dl, 1);
	pG->addEdge(r16Dl, r15Dr, 1);
	doors.push_back(r16Dl);
	strcpy(r16Dl->n, "r16Dl");

	// Room 16 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r16Dr = new Door(6578, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r16Dr;
	pG->addNode(r16Dr); //Add the door to the pathfinder graph
	doors.push_back(r16Dr);
	strcpy(r16Dr->n, "r16Dr");

	// Room 17 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r17Dl = new Door(6599, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r17Dl;
	pG->addNode(r17Dl); //Add the door to the pathfinder graph
	pG->addEdge(r16Dr, r17Dl, 1);
	pG->addEdge(r17Dl, r16Dr, 1);
	doors.push_back(r17Dl);
	strcpy(r17Dl->n, "r17Dl");

	// Room 17 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r17Dr = new Door(6790, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r17Dr;
	pG->addNode(r17Dr); //Add the door to the pathfinder graph
	doors.push_back(r17Dr);
	strcpy(r17Dr->n, "r17Dr");

	// Room 18 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r18Dl = new Door(6811, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r18Dl;
	pG->addNode(r18Dl); //Add the door to the pathfinder graph
	pG->addEdge(r17Dr, r18Dl, 1);
	pG->addEdge(r18Dl, r17Dr, 1);
	doors.push_back(r18Dl);
	strcpy(r18Dl->n, "r18Dl");

	// Room 18 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r18Dr = new Door(7002, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r18Dr;
	pG->addNode(r18Dr); //Add the door to the pathfinder graph
	doors.push_back(r18Dr);
	strcpy(r18Dr->n, "r18Dr");

	// Room 19 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r19D = new Door(7021, 1828 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r19D;
	pG->addNode(r19D); //Add the door to the pathfinder graph
	pG->addEdge(r17Dr, r19D, 1);
	pG->addEdge(r19D, r17Dr, 1);
	doors.push_back(r19D);
	strcpy(r19D->n, "r19Dl");

	// Room 20 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r20Dt = new Door(1392, 2801 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r20Dt;
	r20Dt->bottom = r13Db; //Indicate that this is a vertical door
	r13Db->bottom = r20Dt; //Indicate that this is a vertical door
	pG->addNode(r20Dt); //Add the door to the pathfinder graph
	pG->addEdge(r13Db, r20Dt, 1);
	pG->addEdge(r20Dt, r13Db, 1);
	doors.push_back(r20Dt);

	// Room 20 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r20Dr = new Door(2054, 2796 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r20Dr;
	pG->addNode(r20Dr); //Add the door to the pathfinder graph
	doors.push_back(r20Dr);

	// Room 21 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r21D = new Door(3893, 2626 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r21D;
	r21D->bottom = r14Db; //Indicate that this is a vertical door
	r14Db->bottom = r21D; //Indicate that this is a vertical door
	pG->addNode(r21D); //Add the door to the pathfinder graph
	pG->addEdge(r14Db, r21D, 1);
	pG->addEdge(r21D, r14Db, 1);
	doors.push_back(r21D);

	// Room 22 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r22Dl = new Door(6387, 2050 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r22Dl;
	pG->addNode(r22Dl); //Add the door to the pathfinder graph
	pG->addEdge(r11Dr, r22Dl, 1);
	pG->addEdge(r22Dl, r11Dr, 1);
	pG->addEdge(r15Dl, r22Dl, 1);
	pG->addEdge(r22Dl, r15Dl, 1);
	doors.push_back(r22Dl);

	// Room 22 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r22Dr = new Door(6578, 2048 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r22Dr;
	pG->addNode(r22Dr); //Add the door to the pathfinder graph
	doors.push_back(r22Dr);

	// Room 23 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r23Dl = new Door(6599, 2048 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r23Dl;
	pG->addNode(r23Dl); //Add the door to the pathfinder graph
	pG->addEdge(r22Dr, r23Dl, 1);
	pG->addEdge(r23Dl, r22Dr, 1);
	doors.push_back(r23Dl);

	// Room 23 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r23Dr = new Door(6790, 2048 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r23Dr;
	pG->addNode(r23Dr); //Add the door to the pathfinder graph
	doors.push_back(r23Dr);

	// Room 24 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r24Dl = new Door(6811, 2048 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r24Dl;
	pG->addNode(r24Dl); //Add the door to the pathfinder graph
	pG->addEdge(r23Dr, r24Dl, 1);
	pG->addEdge(r24Dl, r23Dr, 1);
	doors.push_back(r24Dl);

	// Room 24 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r24Dr = new Door(7002, 2048 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r24Dr;
	pG->addNode(r24Dr); //Add the door to the pathfinder graph
	doors.push_back(r24Dr);

	// Room 25 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r25D = new Door(7021, 2048 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r25D;
	pG->addNode(r25D); //Add the door to the pathfinder graph
	pG->addEdge(r24Dr, r25D, 1);
	pG->addEdge(r25D, r24Dr, 1);
	doors.push_back(r25D);

	// Room 26 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r26Dl = new Door(2900, 2796 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r26Dl;
	pG->addNode(r26Dl); //Add the door to the pathfinder graph
	pG->addEdge(r20Dr, r26Dl, 1);
	pG->addEdge(r26Dl, r20Dr, 1);
	doors.push_back(r26Dl);

	// Room 26 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r26Dt = new Door(3893, 2801 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r26Dt;
	r26Dt->bottom = r14Db; //Indicate that this is a vertical door
	r14Db->bottom = r26Dt; //Indicate that this is a vertical door
	pG->addNode(r26Dt); //Add the door to the pathfinder graph
	pG->addEdge(r14Db, r26Dt, 1);
	pG->addEdge(r26Dt, r14Db, 1);
	pG->addEdge(r21D, r26Dt, 1);
	pG->addEdge(r26Dt, r21D, 1);
	doors.push_back(r26Dt);

	// Room 26 right door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r26Dr = new Door(4116, 2801 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r26Dr;
	pG->addNode(r26Dr); //Add the door to the pathfinder graph
	doors.push_back(r26Dr);

	// Room 27 left door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(2);
	DCP->closed = media->images.at(3);
	DCP->locked = media->images.at(4);
	Door *r27D = new Door(4385, 2866 - media->images.at(3)->height,
						media->images.at(3), doorClick, DCP);
	DCP->door = r27D;
	pG->addNode(r27D); //Add the door to the pathfinder graph
	pG->addEdge(r26Dr, r27D, 1);
	pG->addEdge(r27D, r26Dr, 1);
	doors.push_back(r27D);

	// Room 28 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r28D = new Door(3893, 2972 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r28D;
	r28D->bottom = r14Db; //Indicate that this is a vertical door
	r14Db->bottom = r28D; //Indicate that this is a vertical door
	pG->addNode(r28D); //Add the door to the pathfinder graph
	pG->addEdge(r14Db, r28D, 1);
	pG->addEdge(r28D, r14Db, 1);
	pG->addEdge(r21D, r28D, 1);
	pG->addEdge(r28D, r21D, 1);
	pG->addEdge(r26Dt, r28D, 1);
	pG->addEdge(r28D, r26Dt, 1);
	doors.push_back(r28D);

	// Room 29 top door
	DCP = new DoorClickPars;
	DCP->mouse = mouse;
	DCP->open = media->images.at(8);
	DCP->closed = media->images.at(9);
	DCP->locked = media->images.at(10);
	Door *r29D = new Door(3893, 3952 - media->images.at(9)->height,
						media->images.at(9), doorClick, DCP);
	DCP->door = r29D;
	r29D->bottom = r14Db; //Indicate that this is a vertical door
	r14Db->bottom = r29D; //Indicate that this is a vertical door
	pG->addNode(r29D); //Add the door to the pathfinder graph
	pG->addEdge(r14Db, r29D, 1);
	pG->addEdge(r29D, r14Db, 1);
	pG->addEdge(r21D, r29D, 1);
	pG->addEdge(r29D, r21D, 1);
	pG->addEdge(r26Dt, r29D, 1);
	pG->addEdge(r29D, r26Dt, 1);
	pG->addEdge(r28D, r29D, 1);
	pG->addEdge(r29D, r28D, 1);
	doors.push_back(r29D);

	//Make all doors in the same room connect to eachother in the path graph
	//TODO: REMEMBER TO CHANGE THIS WHEN BEACONS ARE ADDED TO MORE COMPLEX ROOMS
	for(int i = 0; i < doors.size() - 1; i++){
		for(int j = i+1; j < doors.size(); j++){
			GameObject *d1 = (GameObject *)((Door *) doors.at(i));
			GameObject *d2 = (GameObject *)((Door *) doors.at(j));

			Room *tr1 = whichRoom(&rooms, d1);
			Room *tr2 = whichRoom(&rooms, d2);

			//Skip the rooms in room 5 (TODO: THIS WAS A QUICK FIX)
			if(tr1 == r5 || tr2 == r5) continue;

			if(tr1 == tr2){
				if(tr1 == nullptr)
					printf("Door at x: %d y: %d is not in a room\n",
									d1->x, d1->y);
				pG->addEdge(d1, d2, 1);
				pG->addEdge(d2, d1, 1);
			}
		}
	}
#pragma endregion
#pragma region CHARACTERS
	/* CHARACTERS */
	// Paul
	StressCharacterPars* paulStress = new StressCharacterPars;
	CharacterObject *paul = new CharacterObject(r16->x + 100, 1831 - 100,
		media->images.at(7), stressCharacter, (void *)(paulStress),
		"Paul", intersex, labels->genders->at(0), labels->romance->at(0),
							labels->sexuality->at(0), pilot, TRUSTING|SENSATIVE);
	paulStress->currChar = paul;

	// Paulette
	StressCharacterPars* pauletteStress = new StressCharacterPars;
	CharacterObject *paulette = new CharacterObject(r17->x + 20, 1831 - 50,
		media->images.at(7), stressCharacter,
		(void *)(pauletteStress), "Paulette", intersex,
					labels->genders->at(3), labels->romance->at(1),
							labels->sexuality->at(1), engineer, BIGOT|LIER);
	pauletteStress->currChar = paulette;
	
	// Paulus
	StressCharacterPars* paulusStress = new StressCharacterPars;
	CharacterObject *paulus = new CharacterObject(r18->x + 10, 1831 - 50,
		media->images.at(7), stressCharacter,
		(void *)(paulusStress), "Paulus", female,
		labels->genders->at(2), labels->romance->at(2),
							labels->sexuality->at(2), doctor, PARANOID);
	paulusStress->currChar = paulus;

	// Paulob
	StressCharacterPars* paulobStress = new StressCharacterPars;
	CharacterObject *paulob = new CharacterObject(r19->x + 50, 1831 - 50,
		media->images.at(7), stressCharacter,
		(void *)(paulobStress), "Paulob", male,
		labels->genders->at(2), labels->romance->at(3),
							labels->sexuality->at(3), captain, CARING);
	paulobStress->currChar = paulob;
  
  //Add tasks for paulette:
	
	//paulette->addTask(new Task(kitchen, btnHello, (void*)(new btnHelloParameter{"Paulette Kitchen!"}),1,130,"GOTO Kitchen",AIASSIGNED));
	//paulette->addTask(new Task(bRoom2, btnHello, (void*)(new btnHelloParameter{"Paulette Hello Bedroom number 2!"}),2,0,"GOTO Bedroom n2",AGAINSTFRIENDS ));
	//paul->addTask(new Task(kitchen, btnHello, (void*)(new btnHelloParameter{ "Paul Hello Kitchen!" }), 1, 200, "GOTO Kitchen", AIASSIGNED));
	//paul->addTask(new Task(bRoom2, btnHello, (void*)(new btnHelloParameter{ "Paul Hello Bedroom number 2!" }), 2, 0, "GOTO Bedroom n2", AGAINSTFRIENDS));

	paul->addTask(new Task(r5, btnHello, (void *)(new btnHelloParameter{"done!"}),
				100000, 5, "Room 5 Beacon test", AIASSIGNED));

#pragma endregion

	// Add all clickable elements to the click system
	vector<IsClickable *> uiElements;
	vector<IsClickable *> popupElements;
	buildClickAreas(cc,
		{paul, paulette, paulus, paulob},
		rooms,
		uiElements,
		popupElements,
		doors
	);

	/* Add all objects to the objects list */
	// Rooms
	for(Room *r : rooms){
		objects->push_back(r);
	}
	// Doors
	for(IsClickable *d : doors){
		objects->push_back(dynamic_cast<GameObject*>((Room*)d));
	}
	// Characters
	objects->push_back(paul);
	paul->objsIndex = objects->size()-1;
	objects->push_back(paulette);
	paulette->objsIndex = objects->size() - 1;
	objects->push_back(paulus);
	paulus->objsIndex = objects->size() - 1;
	objects->push_back(paulob);
	paulob->objsIndex = objects->size() - 1;

	printf("Objects done\n");

	return true;
}

/*
	Initialize the things needed for the engine to run
*/
bool init(WindowStruct *window) {
	//Initialize the SDL library and check if it didn't work, otherwise continue
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		printf("SDL couldn't be initialized. Error: %s\n", SDL_GetError());

		return false;
	}

	//Create the program window
	window->window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Couldn't create a window. Error: %s\n", SDL_GetError());

		return false;
	}
	//Initialize the renderer
	window->render = SDL_CreateRenderer(window->window, -1,
							SDL_RENDERER_ACCELERATED);
	if (window->render == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

		return false;
	}

	//Set the background colour of the window
	SDL_SetRenderDrawColor(window->render, 102, 102, 102, 255);

	//Initialize Sound Drivers
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_Mixer could not initialize! SDL_Mixer Error: %s\n",
									Mix_GetError());

		return false;
	}

	//Initialize Fonts
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
									TTF_GetError());

		return false;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n",
									IMG_GetError());

		return false;
	}
	
	return true;
}

void close(WindowStruct *window, Media& media, vector<GameObject*>& objects,
	CurrentClick* cc, Labels *labels){
	for(Image* img : media.images)
	{
		img->~Image();
	}

	for(Music* m : media.music){
		m->~Music();
	}

	for(Sound* s : media.sounds){
		s->~Sound();
	}

	for(Font* f : media.fonts){
		f->~Font();
	}

	cleanClickAreas(cc);

	for(GameObject* obj : objects) {
		delete obj;
	}

	for(char *t : *labels->genders){
		delete t;
	}
	printf("Freed the genders!\n");
	
	for(affectionTrait *t : *labels->romance){
		delete t->name;
		for(int i = 0; i < t->n; i++){
			delete t->genders[i];
		}
		delete t->genders;

		delete t;
	}
	printf("Freed the romances!\n");

	for(affectionTrait *t : *labels->sexuality){
		delete t->name;
		for(int i = 0; i < t->n; i++){
			delete t->genders[i];
		}
		delete t->genders;

		delete t;
	}
	printf("Freed the sexuality!\n");

	SDL_DestroyRenderer(window->render);
	SDL_DestroyWindow(window->window);

	Mix_CloseAudio();

	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool checkStillAlive(vector<CharacterObject *>& characters, CharacterObject *charCheck) {
	for (CharacterObject * cobj : characters) {
		if (cobj == charCheck) return true;
	}
	return false;
}

int main(int argc, char *argv[]){
	WindowStruct window;

	if(!init(&window)){
		exit(1);
	}

	MouseStruct mouse;
	mouse.x = 0;
	mouse.y = 0;
	mouse.scrollRight = 0;
	mouse.scrollUp = 0;
	mouse.buttons[0] = MouseStruct::Button{ false, false, false };
	mouse.buttons[1] = MouseStruct::Button{ false, false, false };
	mouse.buttons[2] = MouseStruct::Button{ false, false, false };

	Media media;
	bool running = loadMedia(&media, "manifest", window.render);

	CurrentClick currClick;
	vector<GameObject*> objects;
	
	Labels labels;
	Graph<CharacterObject *, Relation> *relGraph = nullptr;
	Graph<GameObject *, int> *pathGraph = new Graph<GameObject *, int>();
	vector<CharacterObject *> characters;

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	//default_random_engine generator(seed);
	default_random_engine generator(1);

	if(running){
		labels.genders = loadGender("gender.jpeg");
		labels.romance = loadAffectionTrait("romance.jpeg");
		labels.sexuality = loadAffectionTrait("sexuality.jpeg");

		if(loadLevel(&objects, &media, "", &currClick, &labels, &mouse,
									pathGraph)){
			printf("Game object done!\n");

			for(int i = 0; i < currClick.Characters.size(); i++){
				characters.push_back((CharacterObject *)
							currClick.Characters.at(i));
			}

			//Check for flags and edit chances
			for (CharacterObject* cobj : characters) {
				if (cobj->traitFlags & SENSATIVE) {
					cobj->rec.noChance -= 5;
				}
				if (cobj->traitFlags & BIGOT) {
					cobj->rec.falloutChance += 10;
				}
				if (cobj->traitFlags & LIER) {
					cobj->rec.birthdayChance += 40;
					cobj->rec.cheatingChance += 40;
				}
				if (cobj->traitFlags & CARING) {
					cobj->rec.confessionChance += 40;
					cobj->rec.supportChance += 40;
					cobj->rec.cuddleChance += 40;
					cobj->rec.cheatingChance -= 40;
				}
				if (cobj->sexuality->n == 0) {
					cobj->rec.cheatingChance = 0;
				}
				if (cobj->romance->n == 0) {
					cobj->rec.confessionChance = 0;
				}
			}

			relGraph = initRelations(&characters);
			//pathGraph = initPathfinding(&currClick.rooms);
			//pathGraph->print();
		}
	}

	Camera cam;
	cam.x = 0;
	cam.y = 0;
	cam.zoomLevel = 0.25f;
	cam.wndX = 0;
	cam.wndY = 0;

	// Timing
	unsigned int targetFrequency = 60;
	Uint32 targetTime = 1000 / targetFrequency;
	Uint32 startTime = SDL_GetTicks();

	printf("Initialization done\n");

	// The Loop
	int ticks = 0;

	//TEST CODE TODO: REMOVE
	bool key[6];
	for(bool k : key) k = false;
	
	//Number of ticks until you win!
	int winTimer = targetFrequency * GAME_LENGTH;
	while(running && !characters.empty()) {
		SDL_RenderClear(window.render);

		//Need to make these zero or they will get stuck.
		for(MouseStruct::Button& button : mouse.buttons) button.isReleased = 0;
		mouse.scrollRight = 0;
		mouse.scrollUp = 0;
		mouse.relX = 0;
		mouse.relY = 0;

		//Start handling events.
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0){
			switch(event.type){
				case SDL_QUIT:
					running = false;
					break;

				case SDL_MOUSEMOTION:
					mouse.relX = event.motion.x-mouse.x;
					mouse.relY = event.motion.y-mouse.y;
					mouse.x = event.motion.x;
					mouse.y = event.motion.y;
					//mouse.relX = event.motion.xrel;
					//mouse.relY = event.motion.yrel;
					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					mouse.relX = event.motion.x-mouse.x;
					mouse.relY = event.motion.y-mouse.y;
					mouse.x = event.button.x;
					mouse.y = event.button.y;
					/* In SDL, left button = 1, middle = 2, and
					   right = 3. If we subtract this by one we can
					   use our array setup
					*/
					mouse.buttons[event.button.button - 1]
						.doubleClick = event.button.clicks - 1;
					mouse.buttons[event.button.button - 1]
						.isPressed = event.button.state == 
									SDL_PRESSED;
					mouse.buttons[event.button.button - 1]
						.isReleased = event.button.state == 
									SDL_RELEASED;
					break;
				case SDL_MOUSEWHEEL:
					mouse.scrollRight = event.wheel.x;
					mouse.scrollUp = event.wheel.y;
					break;

				//TEMPORARY. TODO: REMOVE
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_w:
							key[0] = true;
							break;
						case SDLK_a:
							key[1] = true;
							break;
						case SDLK_s:
							key[2] = true;
							break;
						case SDLK_d:
							key[3] = true;
							break;
						case SDLK_UP:
							key[4] = true;
							break;
						case SDLK_DOWN:
							key[5] = true;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym){
						case SDLK_w:
							key[0] = false;
							break;
						case SDLK_a:
							key[1] = false;
							break;
						case SDLK_s:
							key[2] = false;
							break;
						case SDLK_d:
							key[3] = false;
							break;
						case SDLK_UP:
							key[4] = false;
							break;
						case SDLK_DOWN:
							key[5] = false;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		for(CharacterObject *c : characters) {
			bool arrived = updateMovement(c, &currClick.rooms, pathGraph);
			if (arrived && c->currentTask != nullptr) {
				if (c->currentTask->flag & WAITINGFOR) {
					if (checkStillAlive(characters, c->currentTask->waitingFor)) {
						if (whichRoom(&currClick.rooms,
							c->currentTask->waitingFor) !=
							whichRoom(&currClick.rooms, c)) {
							//printf("Waiting for %s\n", c->currentTask->waitingFor->name);
							continue;
						}
					}
					else {
						printf("It seems that the person %s is waiting for is dead, so they will skip this task\n", c->name);
						c->removeTask();
						continue;
					}
				}
				if (c->currentTask->waitTime < 1) {
					if (c->currentTask->function != nullptr) {
						c->currentTask->function(c->currentTask->data);
					}
					c->removeTask();
				}
				else {
					c->currentTask->waitTime--;
				}
			}
		}

		//More advanced simple button test
		if (mouse.buttons[0].isPressed) {
			checkCord(&currClick, mouse, &cam);
		}
		if (mouse.buttons[0].isReleased || mouse.buttons[2].isReleased) {
			IsClickable* clicked = checkCord(&currClick, mouse, &cam);
			if (clicked != nullptr) {
				clicked->function(clicked->data);
			}
			currClick.currentlySelected = nullptr;
		}

		/*SDL_Rect pTemp = translateToCamera(&cam, &characters.at(0)->area);
		mouse.x = pTemp.x;
		mouse.y = pTemp.y;
		if(checkCord(&currClick, mouse, &cam) != nullptr){
			printf("Paul can be found at (%d %d)\n", mouse.x, mouse.y);
		}
		else{
			printf("Paul is in the shadow realm :( !\n");
			for(ClickArea *area : currClick.Game){
				for(IsClickable *ic : area->clicks){
					if(ic == (IsClickable *)characters.at(0)){
						printf("Paul was found!\n");
						printf("\t(%d, %d)\n", characters.at(0)->x, characters.at(0)->y);
						printf("\tIn: (%d, %d)\n", area->area.x, area->area.y);
						break;
					}
				}
			}
		}*/
		/*SDL_Rect pTemp = translateToCamera(&cam, &characters.at(0)->area);
		printf("\t(%d, %d)\n", pTemp.x, pTemp.y);
		printf("\t\t(%d, %d) - (%d, %d)\n", characters.at(0)->x,
				characters.at(0)->y, characters.at(0)->area.x,
				characters.at(0)->area.y);
		*/
		
		if(key[0]) cam.y -= 20;
		if(key[1]) cam.x -= 20;
		if(key[2]) cam.y += 20;
		if(key[3]) cam.x += 20;
		if(key[4]) cam.zoomLevel += 0.01;
		if(key[5]) cam.zoomLevel -= 0.01;
    
		updateClickAreas(&currClick);

		//Simple implementation of events
		/*
			If you want to add more events you have to increase the number
			of events handled, add its chance in the struct
			relationEventChances, and add that in the "chances" array.
			Finally add its corresponding function in the function array
			"functions"
			TODO: Make the addition of events better.
		*/
		uniform_int_distribution<int> d1000(0, 999);
		for (CharacterObject* cobj : characters) {
			//Roll a d1000 to see if *this* character has an event.
			int roll = d1000(generator);
			// Check if the roll was too low to trigger an event
			if (roll < cobj->rec.noChance) continue;

			const int numEvents = 6; //Number of events we check
			int chances[numEvents] = {
				cobj->rec.falloutChance+cobj->stress,
				cobj->rec.confessionChance,
				//If you are dating, you can cheat
				cobj->dating ? cobj->rec.cheatingChance : 0,
				cobj->rec.birthdayChance,
				//If you are dating, you can cuddle.
				cobj->dating ? cobj->rec.cuddleChance : 0,
				cobj->rec.supportChance,
			};

			//Array of functions for each event
			void(*functions[numEvents])(CurrentClick *cc,
				vector<CharacterObject*>&, CharacterObject *,
					Graph<CharacterObject *, Relation>&,
					default_random_engine, vector<Image *> *) = {
				fallout,
				confession,
				cheating,
				birthday,
				cuddles,
				support
			};

			int allChances = 0;
			//Need summarize all chances
			for (int chance : chances) {
				allChances += chance;
			}
			uniform_int_distribution<int> chanceDist(1, allChances);
			
			roll = chanceDist(generator);
			int prev = 0;
			for (int i = 0; i < numEvents; i++) {
				/*
					Check if the roll is in between the max of the
					previous event's chance and this even'ts change,
					with the  first event being checked in the range
					0 to chance-of-first-event
				*/
				if (roll < chances[i] + prev) {
					// Call the apropriate function for the event
					functions[i](&currClick, characters, cobj,
						*relGraph, generator, &media.images);
					printf("\n");
					break;
				}
				prev += chances[i];
			}
		}

		//Check if a character has stressed to death:
		int i = 0;
		for (CharacterObject* cobj : characters) {
			if (cobj->stress > 222) {
				printf("Jeez %s is so stressed they had a heart attack and died,\n what did you do to them?\n", cobj->name);
				objects.at(cobj->objsIndex)->image = nullptr;
				characters.erase(characters.begin() + i);
				currClick.Characters.erase(currClick.Characters.begin() + i);
			}
			i++;
		}
		
		// Render
		render(&window, media.images.at(12), 0, 0, &cam); //Render background
		for(GameObject* obj : objects) {
			if (obj->image == nullptr) continue;
			render(&window, obj, &cam);
		}
		//Render task icons above the character
		for(CharacterObject *c : characters){
			if(c->image == nullptr || c->taskIcon == nullptr) continue;
			render(&window, c->taskIcon, c->x,
					c->y - (c->taskIcon->height + 10), &cam);
		}
		for (vector<GameObject*>* vec : currClick.toRender)
		{
			for (GameObject* obj : *vec) {
				render(&window, obj);
			}
		}

		// If we were too quick, wait until it is time
		Uint32 time = SDL_GetTicks() - startTime;
		if(time < targetTime){
			//printf("This frame took: %dms\n", targetTime - time);
			SDL_Delay(targetTime - time);
		}

		startTime = SDL_GetTicks();		

		SDL_RenderPresent(window.render);
		if (winTimer < 1) break;
		//else winTimer--;
		//printf("Ticks until win: %d\n", winTimer);
	}

	if (characters.empty()) {
		printf("\n#####################");
		printf("\n## GAME OVER MAN!! ##");
		printf("\n#####################");
		printf("\nAll of the crew died.\n\t");
		uniform_int_distribution<int> chanceDist(1, 7);
		switch (chanceDist(generator)) {
		case 1:
			printf("This isn't gonna look good on your record\n");
			break;
		case 2:
			printf("How? How can an AI possibly be this careless.\nUnless.................................\n");
			break;
		case 3:
			printf("Maybe unions aren't such a bad idea\n");
			break;
		case 4:
			printf("Dude that is kinda fucked.\n");
			break;
		case 5:
			printf("The Corporation (Trademarked) isn't gonna like this\n");
			break;
		case 6:
			printf("Eh, it was probably their own fault\n");
			break;
		case 7:
			printf("Mission success\n");
			break;
		default:
			printf("SECRET PROGRAMMER ENDING!\nYou are still an asshole\n");
			break;
		}
		printf("\n");
	}
	else {
		printf("\n#####################");
		printf("\n## YOU WIN!! ##");
		printf("\n#####################");
		printf("\nAtleast one crew member survived.\n\t");
		uniform_int_distribution<int> chanceDist(1, 7);
		switch (chanceDist(generator)) {
		case 1:
			printf("This is gonna look great on your record!\n");
			break;
		case 2:
			printf("How? How can an AI possibly be this great!\nUnless................................. they are really good!\n");
			break;
		case 3:
			printf("This will really show them socialist that unions aren't needed.\n");
			break;
		case 4:
			printf("Wow, you are great!\n");
			break;
		case 5:
			printf("The Corporation (Trademarked) is gonna love this!\n");
			break;
		case 6:
			printf("You really did great considering what a mess the crew was!\n");
			break;
		case 7:
			printf("Mission success\n");
			break;
		default:
			printf("SECRET PROGRAMMER ENDING!\nYou are a nice person\n");
			break;
		}
		printf("\n");
	}

	close(&window, media, objects, &currClick, &labels);

	return 0;
}

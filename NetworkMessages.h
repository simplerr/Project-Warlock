#pragma once

// Network messages.
enum MessageId
{
	//
	//	General events.
	//
	NMSG_WORLD_UPDATE = 150,		// Sent at a fixed rate (~100/sec). Contains the position for all objects. 
	NMSG_TARGET_ADDED,				// Sent when the server has processed the add target command.
	NMSG_OBJECT_REMOVED,			// Sent when an object is removed from the server.
	NMSG_CONNECTION_SUCCESS,		// Sent when the server has accepted a client connection (to 1 client).
	NMSG_CLIENT_CONNECTION_DATA,	// Set after the server has accepted the client connection. 
	NMSG_ADD_PLAYER,				// Sent when a player has connected to all clients.
	NMSG_PLAYER_DISCONNECTED,		// Sent when a player disconnects.
	NMSG_REQUEST_CLIENT_NAMES,		// A client has requested a list of all connected player names.
	NSMG_CONNECTED_CLIENTS,			// Sent when asking the server for all connected clients.

	//
	//	Game events.
	//
	NMSG_PROJECTILE_PLAYER_COLLISION,	// Sent when a projectile collides with a player.
	NMSG_ITEM_ADDED,
	NMSG_ITEM_REMOVED,
	NMSG_GOLD_CHANGE,
	NMSG_PLAYER_ELIMINATED,

	//
	// Skill events.
	//
	NMSG_SKILL_CAST,				// Sent when the server has processed the cast skill command.
	NMSG_ADD_FIREBALL,				// Fireball skill.

	//
	// State events.
	//
	NMSG_STATE_TIMER,
	NMSG_CHANGETO_SHOPPING,
	NMSG_CHANGETO_PLAYING,

	NMSG_ROUND_START,
	NMSG_ROUND_ENDED
};
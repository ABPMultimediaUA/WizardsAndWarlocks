#ifndef ACTIONS_H
#define ACTIONS_H

enum ACTION_ENUM
{
	ACTION_MOVE_UP         		= 0x00,  	// Moverse Arriba
	ACTION_MOVE_DOWN 			= 0x01,		// Moverse Abajo
	ACTION_MOVE_LEFT			= 0x02,		// Moverse a la izquierda
	ACTION_MOVE_RIGHT			= 0x03,		// Moverse a la derecha
	ACTION_RAYCAST				= 0x04,		// Realizar el Raycast (Interact)
	ACTION_JUMP					= 0x05,		// Saltar
	ACTION_USE_OBJECT			= 0x06,		// Utilizar el objeto del inventario
	ACTION_DROP_OBJECT			= 0x07,		// Soltar el objeto del inventario
	ACTION_SHOOT				= 0x08,		// Lanzar el hechizo
	ACTION_DEPLOY_TRAP			= 0x09,		// Desactivar trampa
	ACTION_CHANGE_SPELL_UP 		= 0x10,		// Cambiar el hechizo +1
	ACTION_CHANGE_SPELL_DOWN 	= 0x11		// Cambiar el hechizo -1
};

#endif
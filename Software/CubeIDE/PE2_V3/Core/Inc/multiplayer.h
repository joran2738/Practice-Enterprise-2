/*
 * multiplayer.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_MULTIPLAYER_H_
#define INC_MULTIPLAYER_H_

enum Multiplayer {disabled = 0, enabled};

void check_availability(void);
void toggle_multiplayer(void);
void addCharacterToArray(char);

#endif /* INC_MULTIPLAYER_H_ */
